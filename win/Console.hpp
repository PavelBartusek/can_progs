// CodeGear C++Builder
// Copyright (c) 1995, 2018 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Console.pas' rev: 33.00 (Windows)

#ifndef ConsoleHPP
#define ConsoleHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>
#include <SysInit.hpp>
#include <Winapi.Windows.hpp>
#include <Winapi.Messages.hpp>
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <System.UITypes.hpp>
#include <System.Types.hpp>

//-- user supplied -----------------------------------------------------------

namespace Console
{
//-- forward type declarations -----------------------------------------------
class DELPHICLASS TConsoleWnd;
class DELPHICLASS TBasicMenuClass;
class DELPHICLASS TMainMenuClass;
class DELPHICLASS TPopUpMenuClass;
//-- type declarations -------------------------------------------------------
typedef System::Uitypes::TColor *PColor;

typedef System::StaticArray<System::Types::TPoint, 5> TMinMaxInfo;

typedef TMinMaxInfo *PMinMaxInfo;

typedef void __cdecl (*TConsoleCallBack)(int x, int y, void * ptr);

typedef void __fastcall (*TCommCallBack)(int x, int y, void * ptr);

class PASCALIMPLEMENTATION TConsoleWnd : public Vcl::Forms::TForm
{
	typedef Vcl::Forms::TForm inherited;
	
__published:
	void __fastcall FormResize(System::TObject* Sender);
	void __fastcall FormShow(System::TObject* Sender);
	void __fastcall FormKeyDown(System::TObject* Sender, System::Word &Key, System::Classes::TShiftState Shift);
	void __fastcall FormKeyPress(System::TObject* Sender, System::WideChar &Key);
	void __fastcall FormMouseDown(System::TObject* Sender, System::Uitypes::TMouseButton Button, System::Classes::TShiftState Shift, int X, int Y);
	void __fastcall FormMouseWheel(System::TObject* Sender, System::Classes::TShiftState Shift, int WheelDelta, const System::Types::TPoint &MousePos, bool &Handled);
	
private:
	System::Types::TPoint FScreenSize;
	System::Types::TPoint FCursor;
	System::Types::TPoint FWindMin;
	System::Types::TPoint FWindMax;
	int FScrollY;
	System::Word FTabWidth;
	bool FChangeOemToAnsi;
	bool FNewResize;
	System::Uitypes::TColor FForeground;
	System::Uitypes::TColor FBackground;
	System::Uitypes::TColor FOuterBackground;
	System::Types::TPoint FCharSize;
	System::Uitypes::TColor OldBackground;
	System::Uitypes::TColor OldForeground;
	bool FAutoTracking;
	bool FCheckEOF;
	bool FCheckBreak;
	bool FReading;
	bool FUseHalt;
	HDC DC;
	HFONT SaveFont;
	int FKeyCount;
	System::StaticArray<char, 64> FKeyBuffer;
	char *ScreenBuffer;
	System::Uitypes::TColor *ScreenForeground;
	System::Uitypes::TColor *ScreenBackground;
	void __fastcall ScrollTo(int X, int Y);
	void __fastcall CursorTo(int x, int y);
	void __fastcall ShowText(int L, int R);
	void __fastcall TextOutput(int X, int Y, int Laenge);
	void __fastcall InitDeviceContext(bool FPainting);
	void __fastcall DoneDeviceContext(bool FPainting);
	void __fastcall TrackCursor();
	void __fastcall UpdateCursor();
	char * __fastcall ScreenPtr(int X, int Y);
	bool __fastcall SaveChar(int X, int Y, char c);
	int __fastcall ColorLength(PColor PColorRef, int Len);
	void __fastcall SetScreenSize(const System::Types::TPoint &NewSize);
	int __fastcall ScreenIndex(int X, int Y);
	int __fastcall GetScreenSizeX();
	int __fastcall GetScreenSizeY();
	void __fastcall WindowScroll(bool Horz, int Action);
	void __fastcall InvalidateRect(const System::Types::TRect &Rect, bool EraseBackground);
	MESSAGE void __fastcall WindowPaint(Winapi::Messages::TWMPaint &_Message);
	MESSAGE void __fastcall WM_ConsMsg(Winapi::Messages::TMessage &_Message);
	MESSAGE void __fastcall WM_MonMsg(Winapi::Messages::TMessage &_Message);
	
__published:
	__property bool AutoTracking = {read=FAutoTracking, write=FAutoTracking, nodefault};
	__property bool CheckBreak = {read=FCheckBreak, write=FCheckBreak, nodefault};
	__property bool CheckEOF = {read=FCheckEOF, write=FCheckEOF, nodefault};
	__property System::Uitypes::TColor Foreground = {read=FForeground, write=FForeground, nodefault};
	__property System::Uitypes::TColor Background = {read=FBackground, write=FBackground, nodefault};
	__property int ScreenSizeX = {read=GetScreenSizeX, nodefault};
	__property int ScreenSizeY = {read=GetScreenSizeY, nodefault};
	__property System::Word TabWidth = {read=FTabWidth, write=FTabWidth, nodefault};
	__property bool ChangeOemToAnsi = {read=FChangeOemToAnsi, write=FChangeOemToAnsi, nodefault};
	__property bool UseHalt = {read=FUseHalt, write=FUseHalt, nodefault};
	
public:
	bool FAutoScroll;
	bool FCursorTracking;
	System::Types::TPoint FDblClickPoint;
	bool FSingleClick;
	void *FCallingC;
	void *FCallingDelphi;
	void __fastcall MyDestroy();
	__fastcall virtual TConsoleWnd(System::Classes::TComponent* AOwner);
	__fastcall virtual ~TConsoleWnd();
	void __fastcall WriteBuf(char * Buffer, System::Word Count);
	System::Word __fastcall ReadBuf(char * Buffer, System::Word Count);
	void __fastcall ClrScr();
	void __fastcall NewScreenSize(int x, int y);
	void __fastcall WriteChar(char Ch);
	void __fastcall GotoXY(int x, int y);
	bool __fastcall KeyPressed();
	char __fastcall ReadKey();
	void __fastcall TextBackground(System::Uitypes::TColor Color);
	void __fastcall TextColor(System::Uitypes::TColor Color);
	void __fastcall LowVideo();
	void __fastcall HighVideo();
	void __fastcall ShowCursor();
	void __fastcall HideCursor();
	void __fastcall Window(int XMin, int YMin, int XMax, int YMax);
	void __fastcall SetNewColor(System::Uitypes::TColor ForeGrnd, System::Uitypes::TColor BackGrnd);
	int __fastcall WhereX();
	int __fastcall WhereY();
	void __fastcall ClrWindow();
	void __fastcall ClrEol();
	void __fastcall write(const System::UnicodeString t)/* overload */;
	void __fastcall write(void * p)/* overload */;
	void __fastcall write(System::Extended d)/* overload */;
	void __fastcall write(int i)/* overload */;
	void __fastcall write(const System::UnicodeString FormatStr, const System::TVarRec *Args, const int Args_High)/* overload */;
	void __fastcall write(const System::TVarRec *Args, const int Args_High)/* overload */;
	void __fastcall writeln()/* overload */;
	void __fastcall writeln(const System::UnicodeString t)/* overload */;
	void __fastcall writeln(const System::UnicodeString FormatStr, const System::TVarRec *Args, const int Args_High)/* overload */;
	void __fastcall writeln(const System::TVarRec *Args, const int Args_High)/* overload */;
	void __fastcall WriteHexByte(int b);
	void __fastcall WriteHexWord(int w);
	void __fastcall TimeStamp();
	void __fastcall CopyTextToClipboard();
	void __fastcall CharCoordinates(System::Types::TPoint &Coord, int x, int y);
	__property System::Types::TPoint Cursor = {read=FCursor};
public:
	/* TCustomForm.CreateNew */ inline __fastcall virtual TConsoleWnd(System::Classes::TComponent* AOwner, int Dummy) : Vcl::Forms::TForm(AOwner, Dummy) { }
	
public:
	/* TWinControl.CreateParented */ inline __fastcall TConsoleWnd(HWND ParentWindow) : Vcl::Forms::TForm(ParentWindow) { }
	
};


typedef void __fastcall (*TMenuHandlerProc)(Vcl::Menus::TMenuItem* Sender, int IndexItem, int IndexSubItem);

#pragma pack(push,4)
class PASCALIMPLEMENTATION TBasicMenuClass : public System::TObject
{
	typedef System::TObject inherited;
	
private:
	Vcl::Menus::TMenuItem* FOwner;
	TMenuHandlerProc FHandlerProc;
	void __fastcall MenuHandler(System::TObject* Sender);
	
public:
	__fastcall TBasicMenuClass(Vcl::Menus::TMenuItem* AOwner);
	void __fastcall SetHandlerProc(TMenuHandlerProc Proc);
	Vcl::Menus::TMenuItem* __fastcall AddItem(const System::UnicodeString Title, bool NotSubItem);
	bool __fastcall Delete(int Index);
	Vcl::Menus::TMenuItem* __fastcall AddToItem(const System::UnicodeString Title, Vcl::Menus::TMenuItem* AddTo);
public:
	/* TObject.Destroy */ inline __fastcall virtual ~TBasicMenuClass() { }
	
};

#pragma pack(pop)

#pragma pack(push,4)
class PASCALIMPLEMENTATION TMainMenuClass : public TBasicMenuClass
{
	typedef TBasicMenuClass inherited;
	
public:
	__fastcall TMainMenuClass(Vcl::Forms::TCustomForm* AOwner);
public:
	/* TObject.Destroy */ inline __fastcall virtual ~TMainMenuClass() { }
	
};

#pragma pack(pop)

#pragma pack(push,4)
class PASCALIMPLEMENTATION TPopUpMenuClass : public TBasicMenuClass
{
	typedef TBasicMenuClass inherited;
	
public:
	Vcl::Menus::TPopupMenu* PopUpMenu;
	__fastcall TPopUpMenuClass(Vcl::Forms::TCustomForm* AOwner);
	__fastcall virtual ~TPopUpMenuClass();
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------
static const System::Int8 Black = System::Int8(0x0);
static const int Blue = int(0x7f0000);
static const System::Word Green = System::Word(0x7f00);
static const int Cyan = int(0x7f7f00);
static const System::Int8 Red = System::Int8(0x7f);
static const int Magenta = int(0x7f00ff);
static const System::Word Brown = System::Word(0x7f7f);
static const int LightGray = int(0xc0c0c0);
static const int DarkGray = int(0x3f3f3f);
static const int LightBlue = int(0xff0000);
static const System::Word LightGreen = System::Word(0xff00);
static const int LightCyan = int(0xffff3f);
static const System::Byte LightRed = System::Byte(0xff);
static const int LightMagenta = int(0xff00ff);
static const System::Word Yellow = System::Word(0xffff);
static const int White = int(0xffffff);
static const System::Word MonWm_NOTIFY = System::Word(0x8064);
extern DELPHI_PACKAGE TConsoleCallBack ConsoleCallBack;
extern DELPHI_PACKAGE TCommCallBack CommCallBack;
extern DELPHI_PACKAGE System::AnsiString __fastcall TimeToMs(double t);
}	/* namespace Console */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_CONSOLE)
using namespace Console;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// ConsoleHPP
