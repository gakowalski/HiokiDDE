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

bool TMainForm::SendCommand(UnicodeString command) {
	OutputTCP->Lines->Add("Wysy³am zapytanie do miernika: " + command);

    if (IdTelnet1->Connected() == false) {
        try {
			IdTelnet1->Connect(HiokiIP->Text, HiokiPort->Text.ToInt());
        } catch (EIdSocketError &e) {
          	OutputTCP->Lines->Add(e.ToString());
            return false;
        }
    }

 	if (IdTelnet1->Connected()) {
        IdTelnet1->IOHandler->Write(get_command(command));
        OutputTCP->Lines->Add("Zapytanie wys³ane.");
	} else {
        OutputTCP->Lines->Add("Utracono po³¹czenie!");
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
    Application->ShowHint = true;
	settings = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    RestoreSettingsClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SendRequestClick(TObject *Sender)
{
    SendCommand(CommandList->Text);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::IdTCPClient1Connected(TObject *Sender)
{
    OutputTCP->Lines->Add("Po³¹czono");
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::IdTCPClient1Disconnected(TObject *Sender)
{
	OutputTCP->Lines->Add("Roz³¹czono");
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::IdTelnet1DataAvailable(TIdTelnet *Sender, const TIdBytes Buffer)

{
    UnicodeString response = BytesToString(Buffer);
    response = response.TrimRight();

    OutputTCP->Lines->Add("OdpowiedŸ z miernika:");
	OutputTCP->Lines->Add(response);
    OutputTCP->Lines->Add("Koniec odpowiedzi.");

    if (MeasureStart->Enabled == false) {
        TStringDynArray measurements = SplitString(response, ";");
        for (int i = 0; i < measurements.Length; i++) {
            TStringDynArray value = SplitString(measurements[i], " ");
            if (MainForm->FindComponent(value[0])) {
                TDdeServerItem * item = (TDdeServerItem *) MainForm->FindComponent(value[0]);
                item->Text = value[1];
            } else {
                OutputTCP->Lines->Add("Nie moge wys³aæ wartoœci " + value[0] + " przez DDE");
            }
        }
    }
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


void __fastcall TMainForm::MeasureStartClick(TObject *Sender)
{
	TStringDynArray values = SplitString(MeasureQuery->Text, ",");
    for (int i = 0; i < values.Length; i++) {
        OutputTCP->Lines->Add(
        	values[i]
        );
        if (MainForm->FindComponent(values[i]) == NULL) {
            TDdeServerItem * item = new TDdeServerItem(MainForm);
            item->Name = values[i];
            item->ServerConv = Pomiary;
            item->Text = values[i];
        }
    }

	MeasureStart->Enabled = false;
    SendRequest->Enabled = false;
    MeasureStop->Enabled = true;
    MeasureAction->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MeasureStopClick(TObject *Sender)
{
    MeasureAction->Enabled = false;
	MeasureStop->Enabled = false;
    MeasureStart->Enabled = true;
    SendRequest->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MeasureActionTimer(TObject *Sender)
{
    if (MeasureStop->Enabled) {
    	OutputTCP->Lines->Add(Now().TimeString());
        if (SendCommand(":HEADER ON;MEAS? " + MeasureQuery->Text) == false) {
        	MeasureStop->Click();
        }
    }
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::ClearOutputTCPClick(TObject *Sender)
{
    OutputTCP->Lines->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TestDDEClick(TObject *Sender)
{
    TStringDynArray values = SplitString(MeasureQuery->Text, ",");
    for (int i = 0; i < values.Length; i++) {
        OutputTCP->Lines->Add(
        	values[i]
        );
        if (MainForm->FindComponent(values[i]) == NULL) {
            TDdeServerItem * item = new TDdeServerItem(MainForm);
            item->Name = values[i];
            item->ServerConv = Pomiary;
            item->Text = values[i];
        }
    }

    UnicodeString response = "UFND1 +777.77E+9;UFND2 +777.77E+9;UFND3 +777.77E+9;IFND1 +777.77E+9;IFND2 +777.77E+9;IFND3 +777.77E+9;PTAV0 +00.000E+3;ITAV1 +00.000E+0;ITAV2 +00.000E+0;ITAV3 +00.000E+0\n";

    TStringDynArray measurements = SplitString(response, ";");
    for (int i = 0; i < measurements.Length; i++) {
		TStringDynArray value = SplitString(measurements[i], " ");
        if (MainForm->FindComponent(value[0])) {
            TDdeServerItem * item = (TDdeServerItem *) MainForm->FindComponent(value[0]);
            item->Text = value[1];
        } else {
            OutputTCP->Lines->Add("Nie moge wys³aæ wartoœci " + value[0] + " przez DDE");
        }
    }

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::RestoreSettingsClick(TObject *Sender)
{
    HiokiIP->Text = settings->ReadString("Connection", "IP", HiokiIP->Text);
    HiokiPort->Text = settings->ReadString("Connection", "Port", HiokiPort->Text);

    MeasureQuery->Text = settings->ReadString("Measurements", "Query", MeasureQuery->Text);
    MeasureAction->Interval = settings->ReadInteger("Measurements", "Interval", MeasureAction->Interval);

    DotToComma->Checked = settings->ReadBool("DDE", "ConvertDotToComma", DotToComma->Checked);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SaveSettingsClick(TObject *Sender)
{
    settings->WriteString("Connection", "IP", HiokiIP->Text);
    settings->WriteString("Connection", "Port", HiokiPort->Text);
    settings->WriteString("Measurements", "Query", MeasureQuery->Text);
    settings->WriteInteger("Measurements", "Interval", MeasureAction->Interval);
    settings->WriteBool("DDE", "ConvertDotToComma", DotToComma->Checked);
}
//---------------------------------------------------------------------------

