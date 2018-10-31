object MainForm: TMainForm
  Left = 309
  Top = 225
  Caption = 'Hioki DDE'
  ClientHeight = 447
  ClientWidth = 631
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
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
      Hint = 'Domy'#347'lny port to 3300'
      ParentShowHint = False
      ShowHint = True
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
  object GroupBox3: TGroupBox
    Left = 8
    Top = 95
    Width = 536
    Height = 114
    Caption = 'Pomiar'
    TabOrder = 2
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
      CharCase = ecUpperCase
      TabOrder = 2
      Text = 
        'UFND1,UFND2,UFND3,IFND1,IFND2,IFND3,PMN1,PMN2,PMN3,PTAV0,ITAV1,I' +
        'TAV2,ITAV3'
    end
    object DotToComma: TCheckBox
      Left = 336
      Top = 18
      Width = 185
      Height = 17
      Caption = 'DDE: Zamie'#324' kropk'#281' na przecinek'
      TabOrder = 3
    end
    object ShowLastCommandOnly: TCheckBox
      Left = 336
      Top = 41
      Width = 185
      Height = 17
      Caption = 'Pokazuj tylko ostatni komunikat'
      Checked = True
      State = cbChecked
      TabOrder = 4
    end
  end
  object ClearOutputTCP: TButton
    Left = 550
    Top = 103
    Width = 75
    Height = 53
    Caption = 'Wyczy'#347#263' podgl'#261'd komunikat'#243'w'
    TabOrder = 3
    WordWrap = True
    OnClick = ClearOutputTCPClick
  end
  object SaveSettings: TButton
    Left = 551
    Top = 55
    Width = 75
    Height = 42
    Caption = 'Zachowaj ustawienia'
    TabOrder = 4
    WordWrap = True
    OnClick = SaveSettingsClick
  end
  object RestoreSettings: TButton
    Left = 551
    Top = 8
    Width = 75
    Height = 41
    Caption = 'Przywr'#243#263' ustawienia'
    TabOrder = 5
    WordWrap = True
    OnClick = RestoreSettingsClick
  end
  object FakeData: TCheckBox
    Left = 550
    Top = 162
    Width = 97
    Height = 23
    Caption = 'Testuj DDE'
    TabOrder = 6
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 215
    Width = 631
    Height = 232
    ActivePage = TabSheet2
    Align = alBottom
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 7
    object TabSheet1: TTabSheet
      Caption = 'Podgl'#261'd komunikacji z urz'#261'dzeniem'
      object OutputTCP: TMemo
        Left = 0
        Top = 0
        Width = 623
        Height = 204
        Align = alClient
        Font.Charset = EASTEUROPE_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Courier New'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        ScrollBars = ssVertical
        TabOrder = 0
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Podgl'#261'd odbieranych warto'#347'ci'
      ImageIndex = 1
      object ArkuszPomiarowy: TStringGrid
        Left = 0
        Top = 0
        Width = 623
        Height = 204
        Align = alClient
        DrawingStyle = gdsClassic
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
        TabOrder = 0
        ExplicitLeft = 272
        ExplicitTop = 72
        ExplicitWidth = 320
        ExplicitHeight = 120
      end
    end
  end
  object Pomiary: TDdeServerConv
    Left = 80
    Top = 392
  end
  object IdTelnet1: TIdTelnet
    OnDisconnected = IdTCPClient1Disconnected
    OnConnected = IdTCPClient1Connected
    OnDataAvailable = IdTelnet1DataAvailable
    Terminal = 'dumb'
    Left = 24
    Top = 392
  end
  object MeasureAction: TTimer
    Enabled = False
    OnTimer = MeasureActionTimer
    Left = 136
    Top = 392
  end
end
