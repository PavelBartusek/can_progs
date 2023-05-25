// CodeGear C++Builder
// Copyright (c) 1995, 2018 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Comm.pas' rev: 33.00 (Windows)

#ifndef CommHPP
#define CommHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>
#include <SysInit.hpp>
#include <Winapi.Windows.hpp>
#include <Winapi.ShellAPI.hpp>
#include <Winapi.Messages.hpp>
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "Console.hpp"

//-- user supplied -----------------------------------------------------------

namespace Comm
{
//-- forward type declarations -----------------------------------------------
class DELPHICLASS TComm;
//-- type declarations -------------------------------------------------------
typedef System::WordBool MyBOOL;

enum DECLSPEC_DENUM TEvent : unsigned char { T_EV_BREAK, T_EV_CTS, T_EV_DSR, T_EV_ERR, T_EV_RING, T_EV_RLSD };

typedef System::Set<TEvent, TEvent::T_EV_BREAK, TEvent::T_EV_RLSD> SEvent;

class PASCALIMPLEMENTATION TComm : public System::TObject
{
	typedef System::TObject inherited;
	
public:
	void __fastcall FormKeyPress(System::TObject* Sender, char &Key);
	void __fastcall mnConfigClick(System::TObject* Sender);
	void __fastcall mnShowClick(System::TObject* Sender);
	
private:
	int FComPortNr;
	int FBaudrate;
	int FParity;
	int FByteSize;
	int FStopBits;
	bool FDTR;
	bool FRTS;
	bool FRtsFlowControl;
	SEvent FEventMask;
	unsigned FEvMask;
	bool FInDialog;
	int FInCommMonitor;
	System::TDateTime FLastStamp;
	_OVERLAPPED osWrite;
	_OVERLAPPED osRead;
	_OVERLAPPED osPostEvent;
	NativeUInt FCOM_Handle;
	_DCB FDCB;
	System::StaticArray<int, 1024> FMonitorBuffer;
	int FMonitorBufferIn;
	int FMonitorBufferOut;
	System::StaticArray<char, 100> FReadBuffer;
	int FReadBufferLen;
	int FReadBufferPtr;
	Vcl::Extctrls::TTimer* FTimer;
	_NOTIFYICONDATAW FNotifyData;
	Console::TConsoleWnd* FConsoleWnd;
	void __fastcall SetFlags(unsigned Info, unsigned Mask, int Bit);
	void __fastcall SetEventMask(SEvent EventMask);
	
public:
	bool ErrorOccured;
	bool DezMonitor;
	bool HexMonitor;
	bool LFMonitor;
	Console::TPopUpMenuClass* pmLeiste;
	bool UseTimeStamp;
	__fastcall TComm(System::Classes::TComponent* AOwner);
	__fastcall virtual ~TComm();
	void __fastcall MyDestroy();
	void __fastcall ChangeCaption(const System::UnicodeString Titel);
	void __fastcall ExitCOM();
	bool __fastcall COM_INITIALISIERT();
	System::WordBool __fastcall SetComParameters();
	void __fastcall SetComPortNr(int Nr);
	void __fastcall SetBaudrate(int Baud);
	System::WordBool __fastcall InitCOM();
	int __fastcall GetMonitorChar();
	void __fastcall PutMonitorBuffer(int ch);
	int __fastcall ReadFromCOM();
	void __fastcall PutBackToReadBuffer();
	int __fastcall ReadWithTimeOut(int TimeMs);
	void __fastcall ClearBuffer();
	System::WordBool __fastcall WriteToCOM(char ch);
	System::WordBool __fastcall WriteStringToCOM(const System::AnsiString str);
	System::WordBool __fastcall WriteBufferToCOM(char * str, int Len);
	System::WordBool __fastcall WriteCS(const System::AnsiString str);
	System::WordBool __fastcall WriteCS_OK();
	bool __fastcall ParameterDialog();
	int __fastcall GetTimerInterval();
	void __fastcall SetTimerInterval(int NewInterval);
	int __fastcall SendBreak(int Duration);
	int __fastcall GetCommModemStatus();
	unsigned __fastcall WaitCommEvent();
	unsigned __fastcall Event();
	bool __fastcall CTS();
	bool __fastcall DSR();
	bool __fastcall RING();
	bool __fastcall DCD();
	void __fastcall SetDTR(bool NewDTR);
	void __fastcall SetRTS(bool NewRTS);
	void __fastcall SetRtsFlowControl(bool NewRTS);
	void __fastcall SetBits(int NewBits);
	void __fastcall SetStopBits(int NewStopBits);
	void __fastcall SetParity(int NewParity);
	void __fastcall DelayMs(int TimeMs);
	void __fastcall CommMonitor(System::TObject* Sender);
	void __fastcall TimeStamp();
	__property int TimerInterval = {read=GetTimerInterval, write=SetTimerInterval, nodefault};
	void __fastcall NewScreenSize(int x, int y);
	void __fastcall Show();
	void __fastcall WriteChar(System::WideChar ch);
	void __fastcall write(const System::UnicodeString t)/* overload */;
	void __fastcall write(const System::WideChar * t)/* overload */;
	void __fastcall writeln()/* overload */;
	void __fastcall writeln(const System::WideChar * t)/* overload */;
	void __fastcall WriteTime(System::TDateTime Time);
	void __fastcall WriteNow();
	
__published:
	__property int Baudrate = {read=FBaudrate, write=SetBaudrate, nodefault};
	__property int Bits = {read=FByteSize, write=SetBits, nodefault};
	__property int ComPortNr = {read=FComPortNr, write=SetComPortNr, nodefault};
	__property int StopBits = {read=FStopBits, write=SetStopBits, nodefault};
	__property bool DTR = {read=FDTR, write=SetDTR, nodefault};
	__property int Parity = {read=FParity, write=SetParity, nodefault};
	__property bool RTS = {read=FRTS, write=SetRTS, nodefault};
	__property bool RtsFlowControl = {read=FRtsFlowControl, write=SetRtsFlowControl, nodefault};
	__property SEvent EventMask = {read=FEventMask, write=SetEventMask, nodefault};
};


//-- var, const, procedure ---------------------------------------------------
static const unsigned COM_HANDLE_CLOSE = unsigned(0xffffffff);
static const System::Word RXQUEUE = System::Word(0x200);
static const System::Word TXQUEUE = System::Word(0x200);
static const System::Word DefaultBaudRate = System::Word(0x12c0);
static const System::Word _MaxMonitorBuffer = System::Word(0x400);
static const System::Int8 _MaxReadBuffer = System::Int8(0x64);
static const System::Word _NULL_CHAR = System::Word(0x1000);
extern DELPHI_PACKAGE bool HideError;
extern DELPHI_PACKAGE System::WordBool __fastcall MessageLoop(System::WordBool Save);
extern DELPHI_PACKAGE void __fastcall DelayMs(int TimeMs);
extern DELPHI_PACKAGE System::Word __fastcall CRC(char * Buffer, int Length);
}	/* namespace Comm */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_COMM)
using namespace Comm;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// CommHPP
