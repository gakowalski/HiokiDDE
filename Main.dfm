object MainForm: TMainForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Hioki DDE'
  ClientHeight = 371
  ClientWidth = 634
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 265
    Height = 81
    Caption = 'Konfiguracja po'#322#261'czenia'
    TabOrder = 0
    object Label1: TLabel
      Left = 22
      Top = 23
      Width = 100
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Adres IP urz'#261'dzenia:'
      FocusControl = HiokiIP
    end
    object Label2: TLabel
      Left = 43
      Top = 50
      Width = 79
      Height = 13
      Alignment = taRightJustify
      Caption = 'Port urz'#261'dzenia:'
      FocusControl = HiokiPort
    end
    object HiokiIP: TEdit
      Left = 128
      Top = 20
      Width = 121
      Height = 21
      TabOrder = 0
      Text = '192.168.1.1'
    end
    object HiokiPort: TEdit
      Left = 128
      Top = 47
      Width = 121
      Height = 21
      TabOrder = 1
      Text = '3300'
    end
  end
  object GroupBox2: TGroupBox
    Left = 279
    Top = 8
    Width = 265
    Height = 81
    Caption = 'Manualna komunikacja z urz'#261'dzeniem'
    TabOrder = 1
    object Label3: TLabel
      Left = 54
      Top = 23
      Width = 52
      Height = 13
      Alignment = taRightJustify
      Caption = 'Zapytanie:'
    end
    object CommandList: TComboBox
      Left = 112
      Top = 20
      Width = 145
      Height = 21
      TabOrder = 0
      Text = '*IDN?'
      Items.Strings = (
        '*IDN?'
        '*OPT?'
        ':HEADER ON'
        ':HEADER OFF'
        ':MEAS?'
        ':HEADER ON;:MEAS?')
    end
    object SendRequest: TButton
      Left = 112
      Top = 47
      Width = 145
      Height = 25
      Caption = 'Wy'#347'lij do urz'#261'dzenia'
      TabOrder = 1
      OnClick = SendRequestClick
    end
  end
  object OutputTCP: TMemo
    Left = 0
    Top = 215
    Width = 634
    Height = 156
    Align = alBottom
    ScrollBars = ssVertical
    TabOrder = 2
  end
  object GroupBox3: TGroupBox
    Left = 8
    Top = 95
    Width = 536
    Height = 114
    Caption = 'Pomiar'
    TabOrder = 3
    object Label_for_MeasureQuery: TLabel
      Left = 22
      Top = 61
      Width = 269
      Height = 13
      Caption = 'Warto'#347'ci do pomiaru zapytaniem ":HEADER ON;MEAS?":'
    end
    object MeasureStart: TButton
      Left = 16
      Top = 24
      Width = 75
      Height = 25
      Caption = 'Rozpocznij'
      TabOrder = 0
      OnClick = MeasureStartClick
    end
    object MeasureStop: TButton
      Left = 97
      Top = 24
      Width = 75
      Height = 25
      Caption = 'Zatrzymaj'
      Enabled = False
      TabOrder = 1
      OnClick = MeasureStopClick
    end
    object MeasureQuery: TEdit
      Left = 16
      Top = 80
      Width = 505
      Height = 21
      TabOrder = 2
      Text = 
        'UFND1,UFND2,UFND3,IFND1,IFND2,IFND3,PFDN1,PFDN2,PFND3,PTAV0,ITAV' +
        '1,ITAV2,ITAV3'
    end
    object Button2: TButton
      Left = 446
      Top = 49
      Width = 75
      Height = 25
      Caption = 'Test DDE'
      TabOrder = 3
      OnClick = Button2Click
    end
    object DotToComma: TCheckBox
      Left = 336
      Top = 26
      Width = 185
      Height = 17
      Caption = 'DDE: Zamie'#324' kropk'#281' na przecinek'
      Checked = True
      State = cbChecked
      TabOrder = 4
    end
  end
  object Button1: TButton
    Left = 550
    Top = 184
    Width = 75
    Height = 25
    Caption = 'Wyczy'#347#263
    TabOrder = 4
    OnClick = Button1Click
  end
  object Pomiary: TDdeServerConv
    Left = 64
    Top = 224
  end
  object IdTelnet1: TIdTelnet
    OnDisconnected = IdTCPClient1Disconnected
    OnConnected = IdTCPClient1Connected
    OnDataAvailable = IdTelnet1DataAvailable
    Terminal = 'dumb'
    Left = 16
    Top = 224
  end
  object MeasureAction: TTimer
    Enabled = False
    OnTimer = MeasureActionTimer
    Left = 128
    Top = 224
  end
end
