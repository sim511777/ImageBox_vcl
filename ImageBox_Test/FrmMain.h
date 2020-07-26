//---------------------------------------------------------------------------

#ifndef FormMainH
#define FormMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ImageBox.h"
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
    TImageBox *imgBox;
    TPanel *Panel1;
    TButton *btnResetZoom;
    TButton *btnOpen;
    TButton *btnLenna8;
    TButton *btnLenna24;
    TButton *btnChess;
    TOpenPictureDialog *dlgOpen;
    void __fastcall btnResetZoomClick(TObject *Sender);
    void __fastcall btnOpenClick(TObject *Sender);
    void __fastcall btnLenna8Click(TObject *Sender);
    void __fastcall btnLenna24Click(TObject *Sender);
    void __fastcall btnChessClick(TObject *Sender);
private:	// User declarations
    BYTE* imgBuf;
    int bw;
    int bh;
    int bytepp;
    void __fastcall SetImage(Graphics::TBitmap* bmp);
public:		// User declarations
    __fastcall TFormMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
