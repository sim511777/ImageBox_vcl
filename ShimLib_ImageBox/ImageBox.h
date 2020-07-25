//---------------------------------------------------------------------------

#ifndef ImageBoxH
#define ImageBoxH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TImageBox : public TCustomControl
{
private:
    TColor FColor;
protected:
    virtual void __fastcall Paint(void);
public:
    __fastcall TImageBox(TComponent* Owner);
__published:
    __property TColor Color = {read = FColor, write = FColor};
};
//---------------------------------------------------------------------------
#endif
