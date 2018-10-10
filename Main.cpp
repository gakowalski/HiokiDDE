//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;

UnicodeString get_command(UnicodeString command) {
    UnicodeString tmp;
    tmp = StringReplace(command, "\\n", "\n", TReplaceFlags() << rfReplaceAll) + "\r\n";
    return StringReplace(tmp, "\\r", "\r", TReplaceFlags() << rfReplaceAll);
}

UnicodeString TMainForm::SendCommand(UnicodeString command) {
    UnicodeString response;

    OutputTCP->Lines->Add("Sending " + command);


    TIdTCPClientCustom * client;

    //client = IdTCPClient1;
    client = IdTelnet1;

    if (client->Connected() == false) {
        //client->Host = HiokiIP->Text;
        //client->Port = HiokiPort->Text.ToInt();
        //client->Connect();
        try {
			client->Connect(HiokiIP->Text, HiokiPort->Text.ToInt());
        } catch (EIdSocketError &e) {
          	OutputTCP->Lines->Add(e.ToString());
        }
    }

 	if (client->Connected()) {
        client->IOHandler->Write(get_command(command));
	} else {
        response = "Nie mo¿na ustanowiæ po³¹czenia";
    }

    return response;
}

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SendRequestClick(TObject *Sender)
{
    OutputTCP->Lines->Add(SendCommand(CommandList->Text));
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::IdTCPClient1Connected(TObject *Sender)
{
    OutputTCP->Lines->Add("Po³¹czono");
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::IdTCPClient1Disconnected(TObject *Sender)
{
	//OutputTCP->Lines->Add("Roz³¹czono");
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::IdTelnet1TelnetCommand(TIdTelnet *Sender, TIdTelnetCommand Status)
{
    UnicodeString status;

    switch (Status) {
        case TIdTelnetCommand::tncNoLocalEcho:
            status = "The NVT will not echo characters locally.";
            break;
        case TIdTelnetCommand::tncLocalEcho:
        	status = "The NVT will echo characters locally.";
            break;
        case TIdTelnetCommand::tncEcho:
            status = "The NVT will allow the remote host to echo characters. ";
            break;
        default:
            status = "Unknown status";
    }

	OutputTCP->Lines->Add(Status);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::IdTelnet1DataAvailable(TIdTelnet *Sender, const TIdBytes Buffer)

{
    OutputTCP->Lines->Add("DATA DUMP BEGINS");
	OutputTCP->Lines->Add(BytesToString(Buffer));
    OutputTCP->Lines->Add("DATA DUMP ENDS");
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ClientSocket1Connect(TObject *Sender, TCustomWinSocket *Socket)

{
	OutputTCP->Lines->Add("Po³¹czono");
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ClientSocket1Disconnect(TObject *Sender, TCustomWinSocket *Socket)

{
      OutputTCP->Lines->Add("Roz³¹czono");
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ClientSocket1Error(TObject *Sender, TCustomWinSocket *Socket,
          TErrorEvent ErrorEvent, int &ErrorCode)
{
         OutputTCP->Lines->Add("B³¹d nr " + ErrorCode);
}
//---------------------------------------------------------------------------


