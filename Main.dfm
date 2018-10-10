object MainForm: TMainForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Hioki DDE'
  ClientHeight = 256
  ClientWidth = 548
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object OutputDDE: TMemo
    Left = 8
    Top = 95
    Width = 265
    Height = 153
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 265
    Height = 81
    Caption = 'Konfiguracja po'#322#261'czenia'
    TabOrder = 1
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
    TabOrder = 2
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
      ItemIndex = 0
      TabOrder = 0
      Text = '*IDN?'
      Items.Strings = (
        '*IDN?'
        '*OPT?'
        ':HEADER ON'
        ':HEADER OFF'
        ':MEAS? U1,I1,P1'
        ':MEAS? I1,I2,I3,P1,P2,P3,U1,U2,U3'
        ':MEAS? ITAV1,ITAV2,ITAV3,PTAV1,PTAV2,PTAV3'
        
          ':MEAS? I1,I2,I3,P1,P2,P3,U1,U2,U3,ITAV1,ITAV2,ITAV3,PTAV1,PTAV2,' +
          'PTAV3')
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
    Left = 279
    Top = 95
    Width = 265
    Height = 153
    ScrollBars = ssVertical
    TabOrder = 3
  end
  object Time: TDdeServerConv
    Left = 16
    Top = 208
  end
  object Current: TDdeServerItem
    ServerConv = Time
    Text = 'TEST'
    Lines.Strings = (
      'TEST')
    Left = 72
    Top = 208
  end
  object IdTCPClient1: TIdTCPClient
    OnDisconnected = IdTCPClient1Disconnected
    OnConnected = IdTCPClient1Connected
    ConnectTimeout = 0
    IPVersion = Id_IPv4
    Port = 0
    ReadTimeout = -1
    Left = 72
    Top = 160
  end
  object IdTelnet1: TIdTelnet
    OnDisconnected = IdTCPClient1Disconnected
    OnConnected = IdTCPClient1Connected
    OnTelnetCommand = IdTelnet1TelnetCommand
    OnDataAvailable = IdTelnet1DataAvailable
    Terminal = 'dumb'
    Left = 16
    Top = 160
  end
end
