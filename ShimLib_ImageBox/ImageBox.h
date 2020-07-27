//---------------------------------------------------------------------------

#ifndef ImageBoxH
#define ImageBoxH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
//---------------------------------------------------------------------------
struct TPointf
{
	float x;
	float y;
	TPointf() {}
	TPointf(float _x, float _y) { x = _x; y = _y; }
};

struct TRectf
{
	float left;
	float top;
	float right;
	float bottom;
	TRectf() {}
	TRectf(const TPointf& TL, const TPointf& BR) { left = TL.x; top = TL.y; right = BR.x; bottom = BR.y; }
	TRectf(float l, float t, float r, float b) { left = l;    top = t;    right = r;    bottom = b; }
	int Width() const { return right - left; }
	int Height() const { return bottom - top; }
	bool operator ==(const TRectf& rc) const
	{
		return left == rc.left  && top == rc.top &&
			right == rc.right && bottom == rc.bottom;
	}
	bool operator !=(const TRectf& rc) const
	{
		return !(rc == *this);
	}
};

class TImageCanvas;

class PACKAGE TImageBox : public TCustomControl
{
private:
    // 이미지 버퍼 정보
    void* imgBuf;
    int imgBw;
    int imgBh;
    int imgBytepp;
    bool isImgbufFloat;

    // 디스플레이 버퍼 정보
    Graphics::TBitmap* dispBmp;

    // 패닝 옵셋(픽셀)
    TPoint ptPan;
    void SetPtPan(TPoint value);
    TPoint GetPtPan();
    __property TPoint PtPan = {read = GetPtPan, write = SetPtPan};

    // 줌 레벨(0 = 1x)
    int zoomLevel;
    void SetZoomLevel(int value);
    int GetZoomLevel();
    __property int ZoomLevel = {read = GetZoomLevel, write = SetZoomLevel};
    void GetZoomFactorComponents(int& exp_num, int& c);
    double GetZoomFactor();
    String GetZoomText();

    // 마우스 좌표
    TPoint ptMove;
    TPoint ptDown;
    bool bDown;

    void DrawPixelValue(TImageCanvas* ic);
    int GetImagePixelValueColorIndex(int ix, int iy);
    String GetImagePixelValueText(int ix, int iy);
    void DrawCenterLine(TImageCanvas* ic);
    void DrawCursorInfo(TCanvas* c, int ofsx, int ofsy);
protected:
    //리사이즈 할때
    DYNAMIC void __fastcall Resize();
    //마우스 다운
    DYNAMIC void __fastcall MouseDown(TMouseButton Button, TShiftState Shift, int X, int Y);
    //마우스 무브
    DYNAMIC void __fastcall MouseMove(TShiftState Shift, int X, int Y);
    //마우스 업
    DYNAMIC void __fastcall MouseUp(TMouseButton Button, TShiftState Shift, int X, int Y);
    //마우스 휠
    DYNAMIC void __fastcall MouseWheelHandler(TMessage &Message);
    //페인트 할때
    virtual void __fastcall Paint();
public:
    __fastcall TImageBox(TComponent* Owner);
    // 이미지 버퍼 설정
    void SetImageBuffer(void* _buf, int _bw, int _bh, int _bytepp, bool _isFloat);
    // 줌 리셋
    void ResetZoom();
    // 이미지 좌표 -> 화면 좌료
    TPoint ImgToDisp(TPointf ptImg);
    TRect ImgToDisp(TRectf rectImg);
    // 화면 좌표 -> 이미지 좌표
    TPointf DispToImg(TPoint ptDisp);
__published:
    __property Color;
    __property Align;
};
//---------------------------------------------------------------------------
#endif
