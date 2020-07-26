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

class PACKAGE TImageBox : public TCustomControl
{
private:
    // �̹��� ���� ����
    void* imgBuf;
    int imgBw;
    int imgBh;
    int imgBytepp;
    bool isImgbufFloat;

    // ���÷��� ���� ����
    Graphics::TBitmap* dispBmp;

    // �д� �ɼ�(�ȼ�)
    TPoint ptPan;
    void SetPtPan(TPoint value);
    TPoint GetPtPan();
    __property TPoint PtPan = {read = GetPtPan, write = SetPtPan};

    // �� ����(0 = 1x)
    int zoomLevel;
    void SetZoomLevel(int value);
    int GetZoomLevel();
    __property int ZoomLevel = {read = GetZoomLevel, write = SetZoomLevel};
    void GetZoomFactorComponents(int& exp_num, int& c);
    double GetZoomFactor();
    String GetZoomText();

    // ���콺 ��ǥ
    TPoint ptMove;
    TPoint ptDown;
    bool bDown;
protected:
    //�������� �Ҷ�
    DYNAMIC void __fastcall Resize();
    //���콺 �ٿ�
    DYNAMIC void __fastcall MouseDown(TMouseButton Button, TShiftState Shift, int X, int Y);
    //���콺 ����
    DYNAMIC void __fastcall MouseMove(TShiftState Shift, int X, int Y);
    //���콺 ��
    DYNAMIC void __fastcall MouseUp(TMouseButton Button, TShiftState Shift, int X, int Y);
    //���콺 ��
    DYNAMIC void __fastcall MouseWheelHandler(TMessage &Message);
    //����Ʈ �Ҷ�
    virtual void __fastcall Paint();
public:
    __fastcall TImageBox(TComponent* Owner);
    // �̹��� ���� ����
    void SetImageBuffer(void* _buf, int _bw, int _bh, int _bytepp, bool _isFloat);
    // �� ����
    void ResetZoom();
    // �̹��� ��ǥ -> ȭ�� �·�
    TPoint ImgToDisp(TPointf ptImg);
    TRect ImgToDisp(TRectf rectImg);
    // ȭ�� ��ǥ -> �̹��� ��ǥ
    TPointf DispToImg(TPoint ptDisp);
__published:
    __property Color;
    __property Align;
    __property Font;
};
//---------------------------------------------------------------------------
#endif
