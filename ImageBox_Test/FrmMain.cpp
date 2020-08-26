//---------------------------------------------------------------------------

#include <vcl.h>
#include <jpeg.hpp>
#include "Util.h"
#pragma hdrstop

#include "FrmMain.h"
#include "resource.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ImageBox"
#pragma resource "*.dfm"
TFormMain *FormMain;
//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
    : TForm(Owner)
{
    imgBuf = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::btnResetZoomClick(TObject *Sender)
{
    imgBox->ResetZoom();
    imgBox->Invalidate();    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnOpenClick(TObject *Sender)
{
    if (dlgOpen->Execute() == false) {
        return;
    }

    String fileName = dlgOpen->FileName;
    String ext = ExtractFileExt(fileName).LowerCase();
    TGraphic* img;
    if (ext == TEXT(".jpg") || ext == TEXT(".jpeg")) {
        img = new TJPEGImage();
    } else if (ext == TEXT(".bmp")) {
        img = new Graphics::TBitmap();
    }

    img->LoadFromFile(fileName);
	Graphics::TBitmap *bmp = new Graphics::TBitmap();
    bmp->Assign(img);

    SetImage(bmp);

    delete bmp;
    delete img;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SetImage(Graphics::TBitmap* bmp) {
    FreeBuffer(&imgBuf);
    BitmapToImageBuffer(bmp, &imgBuf, &bw, &bh, &bytepp);
    imgBox->SetImageBuffer(imgBuf, bw, bh, bytepp, false);
    imgBox->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnLenna8Click(TObject *Sender)
{
    Graphics::TBitmap* bmp = new Graphics::TBitmap();
    bmp->LoadFromResourceID((UINT)MainInstance, IDB_LENNA_8);
    SetImage(bmp);
    delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnLenna24Click(TObject *Sender)
{
    Graphics::TBitmap* bmp = new Graphics::TBitmap();
    bmp->LoadFromResourceID((UINT)MainInstance, IDB_LENNA_24);
    SetImage(bmp);
    delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnChessClick(TObject *Sender)
{
    Graphics::TBitmap* bmp = new Graphics::TBitmap();
    bmp->LoadFromResourceID((UINT)MainInstance, IDB_CHESS);
    SetImage(bmp);
    delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::imgBoxPaint(TObject *Sender)
{
    TCanvas* c = imgBox->Canvas;
    imgBox->DrawLine(c, clRed, 0, 0, 8, 8);
    imgBox->DrawRectangle(c, clRed, 8, 8, 12, 12);
    imgBox->DrawString(c, TEXT("Hello, World"), clLime, 12, 12);
    imgBox->DrawRectangle(c, clRed, 16.5, 16.5, 20.5, 20.5);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            //imgBox->DrawCircle(c, clLime, j, i, 1, false);
            //imgBox->DrawSquare(c, clLime, j, i, 1, false);
            //imgBox->DrawCross(c, clLime, j, i, 1, false);
            //imgBox->DrawPlus(c, clLime, j, i, 1, false);
            //imgBox->DrawCircle(c, clLime, j, i, 8, true);
            //imgBox->DrawSquare(c, clLime, j, i, 8, true);
            //imgBox->DrawCross(c, clLime, j, i, 8, true);
            //imgBox->DrawPlus(c, clLime, j, i, 8, true);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnLenna32Click(TObject *Sender)
{
    Graphics::TBitmap* bmp = new Graphics::TBitmap();
    bmp->LoadFromResourceID((UINT)MainInstance, IDB_LENNA_32);
    SetImage(bmp);
    delete bmp;
}
//---------------------------------------------------------------------------

