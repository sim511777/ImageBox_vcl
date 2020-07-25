//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

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
__fastcall TImageBox::TImageBox(TComponent* Owner)
    : TCustomControl(Owner)
{
    DoubleBuffered = true;
}
//---------------------------------------------------------------------------
void __fastcall TImageBox::Paint(void)
{
    if (ComponentState.Contains(csDesigning)) {
        Canvas->Brush->Color = FColor;
        Canvas->FillRect(ClientRect);
        Canvas->TextOut(3, 3, Name);
        return;
    }

    Canvas->Brush->Color = FColor;
    Canvas->FillRect(ClientRect);
    Canvas->TextOut(3, 3, Name);
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
