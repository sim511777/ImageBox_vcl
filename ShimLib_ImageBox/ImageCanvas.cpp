//---------------------------------------------------------------------------


#pragma hdrstop

#include "ImageCanvas.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
TImageCanvas::TImageCanvas(TImageBox* _imgBox, TCanvas* _canvas) {
    imgBox = _imgBox;
    canvas = _canvas;
}

void TImageCanvas::DrawLine(TColor col, TPointf pt1, TPointf pt2) {
    canvas->Pen->Color = col;
    TPoint dispPt1 = imgBox->ImgToDisp(pt1);
    TPoint dispPt2 = imgBox->ImgToDisp(pt2);
    canvas->MoveTo(dispPt1.x, dispPt1.y);
    canvas->LineTo(dispPt2.x, dispPt2.y);
}
//---------------------------------------------------------------------------
void TImageCanvas::DrawLine(TColor col, float x1, float y1, float x2, float y2) {
    this->DrawLine(col, TPointf(x1, y1), TPointf(x2, y2));
}
//---------------------------------------------------------------------------
void TImageCanvas::DrawString(String s, TColor col, TPointf pt) {
    canvas->Font->Color = col;
    TPoint dispPt = imgBox->ImgToDisp(pt);
    canvas->TextOutA(dispPt.x, dispPt.y, s);
}
//---------------------------------------------------------------------------
void TImageCanvas::DrawString(String s, TColor col, float x, float y) {
    this->DrawString(s, col, TPointf(x, y));
}
//---------------------------------------------------------------------------
void TImageCanvas::DrawEllipse(TColor col, TRectf rect) {
    canvas->Pen->Color = col;
    TRect dispRect = imgBox->ImgToDisp(rect);
    canvas->Ellipse(dispRect);
}
//---------------------------------------------------------------------------
void TImageCanvas::DrawEllipse(TColor col, float left, float top, float right, float bottom) {
    this->DrawEllipse(col, TRectf(left, top, right, bottom));
}
//---------------------------------------------------------------------------
void TImageCanvas::DrawRectangle(TColor col, TRectf rect) {
    canvas->Pen->Color = col;
    TRect dispRect = imgBox->ImgToDisp(rect);
    canvas->Rectangle(dispRect);
}
//---------------------------------------------------------------------------
void TImageCanvas::DrawRectangle(TColor col, float left, float top, float right, float bottom) {
    this->DrawRectangle(col, TRectf(left, top, right, bottom));
}
//---------------------------------------------------------------------------
void TImageCanvas::DrawCircle(TColor col, float x, float y, float r) {
    float left = x - r;
    float top = y - r;
    float right = x + r;
    float bottom = y + r;
    this->DrawEllipse(col, left, top, right, bottom);
}
//---------------------------------------------------------------------------
void TImageCanvas::DrawCircle(TColor col, TPointf pt, float r) {
    this->DrawCircle(col, pt.x, pt.y, r);
}
//---------------------------------------------------------------------------
void TImageCanvas::DrawSquare(TColor col, float x, float y, float r) {
    float left = x - r;
    float top = y - r;
    float right = x + r;
    float bottom = y + r;
    this->DrawRectangle(col, left, top, right, bottom);
}
//---------------------------------------------------------------------------
void TImageCanvas::DrawSquare(TColor col, TPointf pt, float r) {
    this->DrawSquare(col, pt.x, pt.y, r);
}
//---------------------------------------------------------------------------

