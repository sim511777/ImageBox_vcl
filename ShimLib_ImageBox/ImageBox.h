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

    // 옵션
    bool FUseDrawPixelValue;
    bool FUseDrawCenterLine;
    bool FUseDrawCursorInfo;

    TNotifyEvent FOnPaint;
    void DrawPixelValue(TImageCanvas* ic);
    int GetImagePixelValueColorIndex(int ix, int iy);
    String GetImagePixelValueText(int ix, int iy);
    void DrawCenterLine(TImageCanvas* ic);
    void DrawCursorInfo(TCanvas* c, int ofsx, int ofsy);
    void __fastcall WmEraseBkgnd(TWMEraseBkgnd& Message);
BEGIN_MESSAGE_MAP
    MESSAGE_HANDLER(WM_ERASEBKGND, TWMEraseBkgnd, WmEraseBkgnd)
END_MESSAGE_MAP(TCustomControl)
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
    // OnPaint에서 사용하기 위한 Canvas 노출
    __property Canvas;
__published:
	__property Align  = {default=0};
	__property Anchors  = {default=3};
	__property Color  = {default=-16777211};
	__property Constraints ;
	__property DragCursor  = {default=-12};
	__property DragKind  = {default=0};
	__property DragMode  = {default=0};
	__property Enabled  = {default=1};
	__property Font ;
	__property ParentColor  = {default=1};
	__property ParentFont  = {default=1};
	__property ParentShowHint  = {default=1};
	__property PopupMenu ;
	__property ShowHint ;
	__property Visible  = {default=1};
	__property OnClick ;
	__property OnContextPopup ;
	__property OnDblClick ;
	__property OnDragDrop ;
	__property OnDragOver ;
	__property OnEndDock ;
	__property OnEndDrag ;
	__property OnMouseActivate ;
	__property OnMouseDown ;
	__property OnMouseEnter ;
	__property OnMouseLeave ;
	__property OnMouseMove ;
	__property OnMouseUp ;
	__property Classes::TNotifyEvent OnPaint = {read=FOnPaint, write=FOnPaint};
	__property OnStartDock ;
	__property OnStartDrag ;
    __property bool UseDrawPixelValue = {read=FUseDrawPixelValue, write=FUseDrawPixelValue, default=true};
    __property bool UseDrawCenterLine = {read=FUseDrawCenterLine, write=FUseDrawCenterLine, default=true};
    __property bool UseDrawCursorInfo = {read=FUseDrawCursorInfo, write=FUseDrawCursorInfo, default=true};
};
//---------------------------------------------------------------------------
#endif
