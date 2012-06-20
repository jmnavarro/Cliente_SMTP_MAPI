object MainForm: TMainForm
  Left = 368
  Top = 188
  BorderStyle = bsSingle
  Caption = 'MainForm'
  ClientHeight = 387
  ClientWidth = 515
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel
    Left = 30
    Top = 48
    Width = 17
    Height = 13
    Caption = 'De:'
  end
  object Label3: TLabel
    Left = 22
    Top = 80
    Width = 25
    Height = 13
    Caption = 'Para:'
  end
  object Label4: TLabel
    Left = 11
    Top = 160
    Width = 36
    Height = 13
    Caption = 'Asunto:'
  end
  object Label6: TLabel
    Left = 17
    Top = 104
    Width = 30
    Height = 13
    Caption = 'Copia:'
  end
  object Label7: TLabel
    Left = 13
    Top = 128
    Width = 34
    Height = 13
    Caption = 'Oculto:'
  end
  object Label8: TLabel
    Left = 3
    Top = 192
    Width = 44
    Height = 13
    Caption = 'Adjuntos:'
  end
  object Label9: TLabel
    Left = 17
    Top = 248
    Width = 30
    Height = 13
    Caption = 'Texto:'
  end
  object SpeedButton1: TSpeedButton
    Left = 485
    Top = 192
    Width = 20
    Height = 19
    Caption = '+'
    OnClick = SpeedButton1Click
  end
  object SpeedButton2: TSpeedButton
    Left = 485
    Top = 214
    Width = 20
    Height = 19
    Caption = '-'
    OnClick = SpeedButton2Click
  end
  object Label10: TLabel
    Left = 336
    Top = 128
    Width = 44
    Height = 13
    Caption = 'Prioridad:'
  end
  object Label11: TLabel
    Left = 56
    Top = 12
    Width = 77
    Height = 13
    Caption = 'Implementaci'#243'n:'
  end
  object GroupBox1: TGroupBox
    Left = 330
    Top = 11
    Width = 177
    Height = 71
    Caption = ' Servidor SMTP  '
    Enabled = False
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 21
      Width = 25
      Height = 13
      Caption = 'Host:'
    end
    object Label5: TLabel
      Left = 16
      Top = 45
      Width = 34
      Height = 13
      Caption = 'Puerto:'
    end
    object e_host: TEdit
      Left = 56
      Top = 17
      Width = 111
      Height = 21
      Enabled = False
      TabOrder = 0
      Text = 'melon2'
    end
    object e_puerto: TEdit
      Left = 56
      Top = 41
      Width = 111
      Height = 21
      Enabled = False
      TabOrder = 1
      Text = '25'
    end
  end
  object e_de: TEdit
    Left = 56
    Top = 44
    Width = 257
    Height = 21
    TabOrder = 1
    Text = 'perico_de_los_palotes@su_casa.com'
  end
  object e_para: TEdit
    Left = 56
    Top = 76
    Width = 257
    Height = 21
    TabOrder = 2
  end
  object e_copia: TEdit
    Left = 56
    Top = 100
    Width = 257
    Height = 21
    TabOrder = 3
  end
  object e_oculto: TEdit
    Left = 56
    Top = 124
    Width = 257
    Height = 21
    TabOrder = 4
  end
  object e_asunto: TEdit
    Left = 56
    Top = 156
    Width = 449
    Height = 21
    TabOrder = 5
    Text = 'este es un mensaje de prueba'
  end
  object lb_adjuntos: TListBox
    Left = 56
    Top = 192
    Width = 425
    Height = 41
    ItemHeight = 13
    TabOrder = 6
  end
  object e_texto: TMemo
    Left = 56
    Top = 248
    Width = 449
    Height = 89
    Lines.Strings = (
      'Este es un mensaje de prueba.'
      
        'Sirve para demostrar c'#243'mo usar los objetos del sistema de env'#237'o ' +
        'de correo, y adem'#225's ver que '
      'realmente funciona.')
    TabOrder = 7
  end
  object b_enviar: TButton
    Left = 432
    Top = 352
    Width = 75
    Height = 25
    Caption = 'Enviar'
    TabOrder = 8
    OnClick = b_enviarClick
  end
  object cb_prioridad: TComboBox
    Left = 387
    Top = 124
    Width = 119
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 9
    Items.Strings = (
      'Baja'
      'Normal'
      'Alta')
  end
  object cb_tipo: TComboBox
    Left = 141
    Top = 8
    Width = 145
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 10
    OnChange = cb_tipoChange
    Items.Strings = (
      'SMTP'
      'MAPI')
  end
  object GroupBox2: TGroupBox
    Left = 330
    Top = 11
    Width = 177
    Height = 71
    Caption = 'Cliente MAPI'
    Enabled = False
    TabOrder = 11
    Visible = False
    object Label12: TLabel
      Left = 16
      Top = 21
      Width = 26
      Height = 13
      Caption = 'Perfil:'
    end
    object Label13: TLabel
      Left = 16
      Top = 45
      Width = 30
      Height = 13
      Caption = 'Clave:'
    end
    object e_perfil: TEdit
      Left = 56
      Top = 17
      Width = 111
      Height = 21
      Enabled = False
      TabOrder = 0
    end
    object e_clave: TEdit
      Left = 56
      Top = 41
      Width = 111
      Height = 21
      Enabled = False
      TabOrder = 1
    end
  end
  object od: TOpenDialog
    Left = 424
    Top = 192
  end
end
