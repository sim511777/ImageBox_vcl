object FormMain: TFormMain
  Left = 0
  Top = 0
  Width = 870
  Height = 638
  Caption = 'FormMain'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 629
    Top = 0
    Width = 225
    Height = 599
    Align = alRight
    TabOrder = 1
    object btnResetZoom: TButton
      Left = 22
      Top = 24
      Width = 115
      Height = 25
      Caption = 'Reset Zoom'
      TabOrder = 0
      OnClick = btnResetZoomClick
    end
    object btnOpen: TButton
      Left = 22
      Top = 55
      Width = 115
      Height = 25
      Caption = 'Open...'
      TabOrder = 1
      OnClick = btnOpenClick
    end
    object btnLenna8: TButton
      Left = 22
      Top = 104
      Width = 115
      Height = 25
      Caption = 'Lenna_8'
      TabOrder = 2
      OnClick = btnLenna8Click
    end
    object btnLenna24: TButton
      Left = 22
      Top = 135
      Width = 115
      Height = 25
      Caption = 'Lenna_24'
      TabOrder = 3
      OnClick = btnLenna24Click
    end
    object btnChess: TButton
      Left = 22
      Top = 198
      Width = 115
      Height = 25
      Caption = 'Chess'
      TabOrder = 4
      OnClick = btnChessClick
    end
    object btnLenna32: TButton
      Left = 22
      Top = 167
      Width = 115
      Height = 25
      Caption = 'Lenna_32'
      TabOrder = 5
      OnClick = btnLenna32Click
    end
  end
  object imgBox: TImageBox
    Left = 0
    Top = 0
    Width = 629
    Height = 599
    Align = alClient
    Color = clGray
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    OnPaint = imgBoxPaint
  end
  object dlgOpen: TOpenPictureDialog
    Left = 768
    Top = 56
  end
end
