//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#include <algorithm>

#pragma hdrstop

#define MAX(x,y)      ( (x)>(y)?(x):(y) ) // 최대 값
#define MIN(x,y)      ( (x)<(y)?(x):(y) ) // 최소 값
#define ROUND(x) (floor(x + 0.5))

#include "ImageBox.h"
#include "ImageCanvas.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TImageBox *) {
    new TImageBox(NULL);
}
//---------------------------------------------------------------------------

String FormatString(const TCHAR* format, ...) {
    va_list args;
    va_start (args, format);

    String text;
    text.vprintf(format, args);

    va_end (args);

    return text;
}

// 이미지 버퍼를 디스플레이 버퍼에 복사
void CopyImageBufferZoom(void* imgBuf, int imgBw, int imgBh, int bytepp, bool bufIsFloat, Graphics::TBitmap* dispBmp, int panx, int pany, double zoom, int bgColor) {
    int dispBh = dispBmp->Height;
    int dispBw = dispBmp->Width;
    // 인덱스 버퍼 생성
    int* siys = new int[dispBh];
    int* sixs = new int[dispBw];
    for (int y = 0; y < dispBh; y++) {
        int siy = (int)floor((y - pany) / zoom);
        siys[y] = (imgBuf == NULL || siy < 0 || siy >= imgBh) ? -1 : siy;
    }
    for (int x = 0; x < dispBw; x++) {
        int six = (int)floor((x - panx) / zoom);
        sixs[x] = (imgBuf == NULL || six < 0 || six >= imgBw) ? -1 : six;
    }

    for (int y = 0; y < dispBh; y++) {
        int siy = siys[y];
        BYTE* sptr = (BYTE*)imgBuf + (INT64)imgBw * siy * bytepp;
        int* dp = (int*)dispBmp->ScanLine[y];
        for (int x = 0; x < dispBw; x++, dp++) {
            int six = sixs[x];
            if (siy == -1 || six == -1) {       // out of boundary of image
                *dp = bgColor;
            } else {
                BYTE* sp = &sptr[six * bytepp];
                if (bufIsFloat) {
                    if (bytepp == 4) {          // 4byte float gray
                        int v = (int)*(float*)sp;
                        *dp = v | v << 8 | v << 16 | 0xff << 24;
                    } else if (bytepp == 8) {   // 8byte double gray
                        int v = (int)*(double*)sp;
                        *dp = v | v << 8 | v << 16 | 0xff << 24;
                    }
                } else {
                    if (bytepp == 1) {          // 1byte gray
                        int v = sp[0];
                        *dp = v | v << 8 | v << 16 | 0xff << 24;
                    } else if (bytepp == 2) {   // 2byte gray (*.hra)
                        int v = sp[0];
                        *dp = v | v << 8 | v << 16 | 0xff << 24;
                    } else if (bytepp == 3) {   // 3byte bgr
                        *dp = sp[0] | sp[1] << 8 | sp[2] << 16 | 0xff << 24;
                    } else if (bytepp == 4) {   // rbyte bgra
                        *dp = sp[0] | sp[1] << 8 | sp[2] << 16 | 0xff << 24;
                    }
                }
            }
        }
    }

    delete[] sixs;
    delete[] siys;
}
//---------------------------------------------------------------------------
int TColorToBGRA(TColor color) {
    int r = color & 0xff;
    int g = (color >> 8) & 0xff;
    int b = (color >> 16) & 0xff;
    return (b | g << 8 | r << 16 | 0xff << 24);
}
//---------------------------------------------------------------------------

__fastcall TImageBox::TImageBox(TComponent* Owner) : TCustomControl(Owner) {
    DoubleBuffered = true;
        this->Align;

    imgBuf = NULL;
    imgBw = 0;
    imgBh = 0;
    imgBytepp = 0;
    isImgbufFloat = false;
    dispBmp = new Graphics::TBitmap;
    dispBmp->PixelFormat = pf32bit;

    ptPan = TPoint(2, 2);
    zoomLevel = 0;

    ptMove = TPoint(-1, -1);
    ptDown;
    bDown = false;
}
//---------------------------------------------------------------------------
TPoint TImageBox::GetPtPan() {
    return ptPan;
}
//---------------------------------------------------------------------------
void TImageBox::SetPtPan(TPoint value) {
    if (imgBuf == NULL)
        ptPan = TPoint(2, 2);
    else {
        double zoom = GetZoomFactor();
        int x = MAX(MIN(value.x, 2), -(int)(imgBw * zoom) + 2);
        int y = MAX(MIN(value.y, 2), -(int)(imgBh * zoom) + 2);
        ptPan = TPoint(x, y);
    }
} 
//---------------------------------------------------------------------------
int TImageBox::GetZoomLevel() {
    return zoomLevel;
}
//---------------------------------------------------------------------------
void TImageBox::SetZoomLevel(int value) { zoomLevel = MAX(MIN(value, 16), -16); }
//---------------------------------------------------------------------------
void TImageBox::GetZoomFactorComponents(int& exp_num, int& c) {
    exp_num = (ZoomLevel >= 0) ? ZoomLevel / 2 : (ZoomLevel - 1) / 2;
    if (ZoomLevel % 2 != 0)
        exp_num--;
    c = (ZoomLevel % 2 != 0) ? 3 : 1;
}
//---------------------------------------------------------------------------
double TImageBox::GetZoomFactor() {
    int exp_num;
    int c;
    GetZoomFactorComponents(exp_num, c);
    return c * pow(2, exp_num);
}
//---------------------------------------------------------------------------
String TImageBox::GetZoomText() {
    int exp_num;
    int c;
    GetZoomFactorComponents(exp_num, c);
    return (exp_num >= 0) ? IntToStr(c * (int)pow(2, exp_num)) : IntToStr(c) + "/" + IntToStr((int)pow(2, -exp_num));
}
//---------------------------------------------------------------------------
void TImageBox::SetImageBuffer(void* _buf, int _bw, int _bh, int _bytepp, bool _isFloat) {
    imgBuf = _buf;
    imgBw = _bw;
    imgBh = _bh;
    imgBytepp = _bytepp;
    isImgbufFloat = _isFloat;
}
//---------------------------------------------------------------------------
void TImageBox::ResetZoom() {
    PtPan = TPoint(2, 2);
    ZoomLevel = 0;
}
//---------------------------------------------------------------------------
TPoint TImageBox::ImgToDisp(TPointf ptImg) {
    double zoom = GetZoomFactor();
    int dispX = (int)floor((ptImg.x + 0.5) * zoom + PtPan.x);
    int dispY = (int)floor((ptImg.y + 0.5) * zoom + PtPan.y);
    return TPoint(dispX, dispY);
}
//---------------------------------------------------------------------------
TRect TImageBox::ImgToDisp(TRectf rectImg) {
    double zoom = GetZoomFactor();
    int dispL = (int)floor((rectImg.left + 0.5) * zoom + PtPan.x);
    int dispR = (int)floor((rectImg.right + 0.5) * zoom + PtPan.y);
    int dispT = (int)floor((rectImg.top + 0.5) * zoom + PtPan.x);
    int dispB = (int)floor((rectImg.bottom + 0.5) * zoom + PtPan.y);
    return TRect(dispL, dispR, dispT, dispB);
}
//---------------------------------------------------------------------------
TPointf TImageBox::DispToImg(TPoint ptDisp) {
    double zoom = GetZoomFactor();
    float imgX = (float)((ptDisp.x - PtPan.x) / zoom - 0.5);
    float imgY = (float)((ptDisp.y - PtPan.y) / zoom - 0.5);
    return TPointf(imgX, imgY);
}
//---------------------------------------------------------------------------
void __fastcall TImageBox::Resize() {
    dispBmp->Width = MAX(ClientWidth, 64);
    dispBmp->Height = MAX(ClientHeight, 64);
    TCustomControl::Resize();
}
//---------------------------------------------------------------------------
void __fastcall TImageBox::MouseDown(TMouseButton Button, TShiftState Shift, int X, int Y) {
    ptDown = TPoint(X, Y);
    bDown = true;
    TCustomControl::MouseDown(Button, Shift, X, Y);
}
//---------------------------------------------------------------------------
void __fastcall TImageBox::MouseMove(TShiftState Shift, int X, int Y) {
    ptMove = TPoint(X, Y);
    if (bDown) {
        PtPan = TPoint(PtPan.x + X - ptDown.x, PtPan.y + Y - ptDown.y);
        ptDown = TPoint(X, Y);
        Invalidate();
    } else {
//        Graphics::TBitmap* bmp = new Graphics::TBitmap();
//        bmp->Width = 200;
//        bmp->Height = Font->Height;
//            using (Graphics bg = Graphics.FromImage(bmp)) {
//                DrawCursorInfo(bg, 0, 0);
//            }
//            using (Graphics g = CreateGraphics()) {
//                g.DrawImage(bmp, 2, 2);
//            }
//        delete bmp;
    }
    TCustomControl::MouseMove(Shift, X, Y);
}
//---------------------------------------------------------------------------
void __fastcall TImageBox::MouseUp(TMouseButton Button, TShiftState Shift, int X, int Y) {
    bDown = false;
    TCustomControl::MouseUp(Button, Shift, X, Y);
}
//---------------------------------------------------------------------------
void __fastcall TImageBox::MouseWheelHandler(TMessage &Message) {
    TWMMouseWheel mw = *(TWMMouseWheel*)&Message;
    TPoint MousePos = this->ScreenToClient(TPoint(mw.XPos, mw.YPos));
    int WheelDelta = mw.WheelDelta;
    TShiftState ShiftState = *(TShiftState*)&mw.Keys;

    TPointf ptImg = DispToImg(MousePos);
    ZoomLevel += (WheelDelta > 0) ? 1 : -1;
    TPoint ptDisp = ImgToDisp(ptImg);
        PtPan = TPoint(PtPan.x + MousePos.x - ptDisp.x, PtPan.y + MousePos.y - ptDisp.y);

    Invalidate();
    TCustomControl::MouseWheelHandler(Message);
}
//---------------------------------------------------------------------------
void __fastcall TImageBox::Paint(void) {
    if (ComponentState.Contains(csDesigning)) {
        Canvas->Brush->Color = Color;
        Canvas->Font = Font;
        Canvas->FillRect(ClientRect);
        Canvas->TextOut(3, 3, Name);
        return;
    }

    Canvas->Font = Font;
    TImageCanvas ic(this, Canvas);
    double zoom = GetZoomFactor();
    CopyImageBufferZoom(imgBuf, imgBw, imgBh, imgBytepp, isImgbufFloat, dispBmp, ptPan.x, ptPan.y, zoom, TColorToBGRA(Color));
    Canvas->Draw(0, 0, dispBmp);
    DrawPixelValue(&ic);
    DrawCenterLine(&ic);
}
//---------------------------------------------------------------------------

// 픽셀값 표시 컬러
// 픽셀값 표시 컬러
TColor pseudo[8] = {
    clWhite,    // 0~31
    clAqua,     // 32~63
    clBlue,     // 63~95
    clYellow,   // 96~127
    clMaroon,   // 128~159
    clFuchsia,  // 160~191
    clRed,      // 192~223
    clBlack,    // 224~255
};

void TImageBox::DrawPixelValue(TImageCanvas* ic) {
    if (imgBuf == NULL)
        return;
    double zoom = GetZoomFactor();
    if (zoom < 16 || (imgBytepp != 1 && zoom < 48))
        return;

    int fontSize = 0;
    if (imgBytepp == 1) {
        if (zoom <= 17) fontSize = 6;
        else if (zoom <= 25) fontSize = 8;
        else fontSize = 10;
    } else {
        if (zoom <= 49) fontSize = 6;
        else if (zoom <= 65) fontSize = 8;
        else fontSize = 10;
    }

    String oldFontName = ic->canvas->Font->Name;
    int oldFontSize = ic->canvas->Font->Size;
    TBrushStyle oldBrushStyle = ic->canvas->Brush->Style;
    ic->canvas->Font->Name = TEXT("arial");
    ic->canvas->Font->Size = fontSize;
    ic->canvas->Brush->Style = bsClear;
    TPointf ptImgLT = DispToImg(TPoint(0, 0));
    TPointf ptImgRB = DispToImg(TPoint(Width, Height));
    int ix1 = (int)ROUND(ptImgLT.x);
    int iy1 = (int)ROUND(ptImgLT.y);
    int ix2 = (int)ROUND(ptImgRB.x);
    int iy2 = (int)ROUND(ptImgRB.y);
    ix1 = MAX(ix1, 0);
    iy1 = MAX(iy1, 0);
    ix2 = MIN(ix2, imgBw - 1);
    iy2 = MIN(iy2, imgBh - 1);
    for (int iy = iy1; iy <= iy2; iy++) {
        for (int ix = ix1; ix <= ix2; ix++) {
            String pixelValueText = GetImagePixelValueText(ix, iy);
            int colIdx = GetImagePixelValueColorIndex(ix, iy);
            ic->DrawString(pixelValueText, pseudo[colIdx], ix - 0.5f, iy - 0.5f);
        }
    }
    ic->canvas->Font->Name = oldFontName;
    ic->canvas->Font->Size = oldFontSize;
    ic->canvas->Brush->Style = oldBrushStyle;
}
//---------------------------------------------------------------------------

// 픽셀 표지 문자열
String TImageBox::GetImagePixelValueText(int ix, int iy) {
    if (imgBuf == NULL || ix < 0 || ix >= imgBw || iy < 0 || iy >= imgBh)
        return EmptyStr;
    BYTE* ptr = (BYTE*)imgBuf + ((INT64)imgBw * iy + ix) * imgBytepp;
    if (imgBytepp == 1) {
        return IntToStr(*ptr);
    } else {
        if (isImgbufFloat) {
            if (imgBytepp == 4)
                return FormatString(TEXT("%.2f"), *(float*)ptr);
            else
                return FormatString(TEXT("%.2f"), *(double*)ptr);
        } else {
            return FormatString(TEXT("%d,%d,%d"), ptr[2], ptr[1], ptr[0]);
        }
    }
}

// 픽셀 표시 컬러 인덱스
int TImageBox::GetImagePixelValueColorIndex(int ix, int iy) {
    if (imgBuf == NULL || ix < 0 || ix >= imgBw || iy < 0 || iy >= imgBh)
        return 0;
    BYTE* ptr = (BYTE*)imgBuf + ((INT64)imgBw * iy + ix) * imgBytepp;
    if (imgBytepp == 1) {
        return (*ptr) / 32;
    } else {
        if (isImgbufFloat) {
            if (imgBytepp == 4)
                return MIN(MAX((int)*(float*)ptr, 0), 255) / 32;
            else
                return MIN(MAX((int)*(double*)ptr, 0), 255) / 32;
        } else {
            return (ptr[2] + ptr[1] + ptr[0]) / 96;
        }
    }
}

// 중심선 표시
void TImageBox::DrawCenterLine(TImageCanvas* ic) {
    if (imgBuf == NULL)
        return;
    ic->canvas->Pen->Style = psDot;
    ic->DrawLine(clYellow, imgBw / 2.0f - 0.5f, -0.5f, imgBw / 2.0f - 0.5f, imgBh - 0.5f);
    ic->DrawLine(clYellow, -0.5f, imgBh / 2.0f - 0.5f, imgBw - 0.5f, imgBh / 2.0f - 0.5f);
}

namespace Imagebox {
    void __fastcall PACKAGE Register() {
         TComponentClass classes[1] = {__classid(TImageBox)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
