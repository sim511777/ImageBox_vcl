//---------------------------------------------------------------------------

#ifndef ImageCanvasH
#define ImageCanvasH
#include "ImageBox.h"
//---------------------------------------------------------------------------
class TImageCanvas {
private:
    TImageBox* imgBox;
public:
    TCanvas* canvas;
public:
    TImageCanvas(TImageBox* _imgBox, TCanvas* _canvas);
    void DrawLine(TColor col, TPointf pt1, TPointf pt2);
    void DrawLine(TColor col, float x1, float y1, float x2, float y2);
    void DrawString(String s, TColor col, TPointf pt);
    void DrawString(String s, TColor col, float x, float y);
    void DrawEllipse(TColor col, TRectf rect);
    void DrawEllipse(TColor col, float left, float top, float right, float bottom);
    void DrawRectangle(TColor col, TRectf rect);
    void DrawRectangle(TColor col, float left, float top, float right, float bottom);
    void DrawCircle(TColor col, float x, float y, float r);
    void DrawCircle(TColor col, TPointf pt, float r);
    void DrawSquare(TColor col, float x, float y, float r);
    void DrawSquare(TColor col, TPointf pt, float r);
};
//---------------------------------------------------------------------------
#endif
