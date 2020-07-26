//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#include <algorithm>

#pragma hdrstop

#define MAX(x,y)      ( (x)>(y)?(x):(y) ) // 최대 값
#define MIN(x,y)      ( (x)<(y)?(x):(y) ) // 최소 값

#include "ImageBox.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TImageBox *)
{
    new TImageBox(NULL);
}
//---------------------------------------------------------------------------

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

__fastcall TImageBox::TImageBox(TComponent* Owner)
    : TCustomControl(Owner)
{
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
void __fastcall TImageBox::Resize()
{
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
void __fastcall TImageBox::Paint(void)
{
    if (ComponentState.Contains(csDesigning)) {
        Canvas->Brush->Color = Color;
        Canvas->FillRect(ClientRect);
        Canvas->TextOut(3, 3, Name);
        return;
    }

    CopyImageBufferZoom(imgBuf, imgBw, imgBh, imgBytepp, isImgbufFloat, dispBmp, ptPan.x, ptPan.y, GetZoomFactor(), TColorToBGRA(Color));
    Canvas->Draw(0, 0, dispBmp);
    
    TCustomControl::Paint();
}
//---------------------------------------------------------------------------

namespace Imagebox
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TImageBox)};
         RegisterComponents("Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
