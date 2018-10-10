//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.DdeMan.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <IdTelnet.hpp>
#include <IdGlobal.hpp>
#include <System.Win.ScktComp.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TMemo *OutputDDE;
	TDdeServerConv *Time;
	TDdeServerItem *Current;
	TIdTCPClient *IdTCPClient1;
	TGroupBox *GroupBox1;
	TEdit *HiokiIP;
	TEdit *HiokiPort;
	TLabel *Label1;
	TLabel *Label2;
	TGroupBox *GroupBox2;
	TMemo *OutputTCP;
	TComboBox *CommandList;
	TButton *SendRequest;
	TLabel *Label3;
	TIdTelnet *IdTelnet1;
	TClientSocket *ClientSocket1;
	void __fastcall SendRequestClick(TObject *Sender);
	void __fastcall IdTCPClient1Connected(TObject *Sender);
	void __fastcall IdTCPClient1Disconnected(TObject *Sender);
	void __fastcall IdTelnet1TelnetCommand(TIdTelnet *Sender, TIdTelnetCommand Status);
	void __fastcall IdTelnet1DataAvailable(TIdTelnet *Sender, const TIdBytes Buffer);
	void __fastcall ClientSocket1Connect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocket1Disconnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocket1Error(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);





private:	// User declarations
	UnicodeString SendCommand(UnicodeString command);
    UnicodeString WinsockSendCommand(
		const UnicodeString & host,
        const UnicodeString & port,
        const UnicodeString & command
	);
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
