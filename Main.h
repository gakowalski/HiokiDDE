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
#include <Vcl.ExtCtrls.hpp>
#include <vector>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TDdeServerConv *Pomiary;
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
	TGroupBox *GroupBox3;
	TButton *MeasureStart;
	TButton *MeasureStop;
	TLabel *Label_for_MeasureQuery;
	TTimer *MeasureAction;
	TEdit *MeasureQuery;
	TButton *Button1;
	TButton *Button2;
	TCheckBox *DotToComma;
	void __fastcall SendRequestClick(TObject *Sender);
	void __fastcall IdTCPClient1Connected(TObject *Sender);
	void __fastcall IdTCPClient1Disconnected(TObject *Sender);
	void __fastcall IdTelnet1DataAvailable(TIdTelnet *Sender, const TIdBytes Buffer);
	void __fastcall ClientSocket1Connect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocket1Disconnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocket1Error(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
	void __fastcall MeasureStartClick(TObject *Sender);
	void __fastcall MeasureStopClick(TObject *Sender);
	void __fastcall MeasureActionTimer(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);





private:	// User declarations
	bool SendCommand(UnicodeString command);
    std::vector<TDdeServerItem *> dde_items;

public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
