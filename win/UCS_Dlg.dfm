�
 TFORM1 0!  TPF0TForm1Form1Left�Top� Width�Height� Caption
CS_BrueckeColor	clBtnFaceFont.CharsetDEFAULT_CHARSET
Font.ColorclWindowTextFont.Height�	Font.NameMS Sans Serif
Font.Style OldCreateOrderOnCloseQueryFormCloseQueryOnCreate
FormCreate	OnDestroyFormDestroyOnShowFormShowPixelsPerInch`
TextHeight TPageControlpcMainLeft Top Width�Height� 
ActivePagetsStartAlignalClientTabIndex TabOrder  	TTabSheettsStartCaptionStart TLabellbCommLeftTop Width7HeightCaptionComfortSoft  TLabellbDeviceLeftTopHWidth6HeightCaption
Verbindung  TLabellbRaspiLeftTopdWidthQHeightCaptionIP-Adresse Raspi  TLabellbSecondComLeftTop� WidthEHeightCaption2. Schnittstelle  TLabellbExplanationLeftTop� WidthHeightFont.CharsetDEFAULT_CHARSET
Font.ColorclRedFont.Height�	Font.NameMS Sans Serif
Font.Style 
ParentFont  TLabel
lblUSB2CANLeftTopdWidth"HeightCaptionDevice  	TComboBoxcbCommLeft� TopWidth}HeightStylecsOwnerDrawFixed
ItemHeight	ItemIndexTabOrder TextCOM4Items.StringsCOM1COM2COM3COM4COM5COM6COM7COM8COM9   	TComboBoxcbDeviceLeft� TopDWidth}HeightStylecsOwnerDrawFixed
ItemHeightTabOrderOnChangecbDeviceChangeItems.Strings
NULL-Modem
can_servercan232/USBtin
Simulationusb2can (8devices)can232 remote   TEditedRaspiLeft� Top`Width}HeightTabOrderText192.168.178.36  	TComboBoxcbSecondComLeft� Top|Width}HeightStylecsOwnerDrawFixed
ItemHeight	ItemIndexTabOrderTextCOM5Items.StringsCOM1COM2COM3COM4COM5COM6COM7COM8COM9   TButtonbtnStartLeftTopkWidthKHeightCaptionStartTabOrderOnClickbtnStartClick  	TCheckBoxcbxBinaryProtocolLeftTop� Width|Height	AlignmenttaLeftJustifyCaption   binäres ProtokollTabOrder  TEdit	edUSB2CANLeft� Top`Width}HeightTabOrderTextED000200   	TTabSheettsFilterCaptionFilter
ImageIndex 	TCheckBoxcbxUseElsterTableLeftTopWidth� HeightCaptionElster-TabelleChecked	State	cbCheckedTabOrder OnClickcbxUseElsterTableClick  	TCheckBoxcbxCS_TelegramLeftTop$Width� HeightCaptionCS SendetelegrammTabOrderOnClickcbxCS_TelegramClick  	TCheckBoxcbxCanTelegramLeftTopLWidth� HeightCaptionCAN SendetelegrammTabOrderOnClickcbxCanTelegramClick  	TCheckBoxcbxRecvTelegramLeftTop`Width� HeightCaptionCAN EmpfangstelegrammTabOrderOnClickcbxRecvTelegramClick  	TCheckBoxcbxLogLeftTop� WidthqHeightCaptionLoggen in DateiTabOrderOnClickcbxLogClick  TEditedLogLeft� Top� Width� HeightTabOrderText
cs_log.txt  TButtonbtnFileOpenLeftpTop� WidthHeightCaption...TabOrderOnClickbtnFileOpenClick  	TCheckBoxcbxNotChangeLeftToptWidth� HeightCaption(   WP Daten können nicht verändert werdenChecked	State	cbCheckedTabOrderOnClickcbxNotChangeClick  	TCheckBox	cbxRecvCSLeftTop8Width� HeightCaptionCS EmpfangstelegrammTabOrderOnClickcbxRecvCSClick   	TTabSheettsScanCaptionScan-Tabelle
ImageIndex TLabel
lbScanFileLeftTopWidth?HeightCaptionScan-Tabelle  TEdit
edScanFileLeft� TopWidth� HeightTabOrder Text..\scan_data.inc  TButtonbtnLoadScanLeftpTopWidthHeightCaption...TabOrderOnClickbtnLoadScanClick  TButtonbtnLoadLeftTopkWidthcHeightCaptionTabelle ladenTabOrderOnClickbtnLoadClick  TButtonbtnSaveLeftTopkWidthcHeightCaptionTabelle speichernTabOrderOnClickbtnSaveClick   	TTabSheettsExtrasCaptionExtras
ImageIndex TLabel	lbTimeoutLeftTopWidthaHeightCaptionSocket Timeout [ms]  TLabel	lbVersionLeftTop4Width� HeightCaption.Use this version (insted of the real version)   TLabellbUSBtinLeftToppWidthRHeightCaptionUSBtin simulation  TEdit	edTimeoutLeft� TopWidthyHeightBiDiModebdLeftToRightParentBiDiModeTabOrder Text2000  TEdit	edVersionLeft� Top0WidthyHeightBiDiModebdLeftToRightParentBiDiModeTabOrderText$5555  	TComboBox	cbxUSBtinLeft� ToplWidthyHeightStylecsOwnerDrawFixed
ItemHeightTabOrderItems.Strings COM1COM2COM3COM4COM5COM6COM7COM8COM9   TEditedSendLefthTop� Width� HeightBiDiModebdLeftToRightParentBiDiModeTabOrderText500.0808.00bc  TButtonbtnSendLeftTop� WidthKHeightCaptionUSBtin sendTabOrderOnClickbtnSendClick    TOpenDialog
OpenDialogLeft    