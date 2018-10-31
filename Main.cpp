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

void TMainForm::log(UnicodeString & message, bool force = false) {
    /*if (force || LogMessages->checked) {
        OutputTCP->Lines->Add(message);
    }
    */
}

bool TMainForm::SendCommand(UnicodeString command) {
    if (ShowLastCommandOnly->Checked) {
        OutputTCP->Lines->Clear();
    }
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
        try {
	        IdTelnet1->IOHandler->Write(get_command(command));
            OutputTCP->Lines->Add("Zapytanie wys³ane.");
        } catch (EIdSocketError &e) {
             OutputTCP->Lines->Add(e.ToString());
        }
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
        if (DotToComma->Checked) {
            response = StringReplace(response, ".", ",", TReplaceFlags() << rfReplaceAll);
        }
        TStringDynArray measurements = SplitString(response, ";");

        ArkuszPomiarowy->ColCount = 1;
        ArkuszPomiarowy->RowCount = 5;
        ArkuszPomiarowy->Cells[0][0] = "\\";
        ArkuszPomiarowy->Cells[0][1] = "Channel 0";
        ArkuszPomiarowy->Cells[0][2] = "Channel 1";
        ArkuszPomiarowy->Cells[0][3] = "Channel 2";
        ArkuszPomiarowy->Cells[0][4] = "Channel 3";

        // ArkuszPomiarowy->Rows[0]->IndexOf()

        for (int i = 0; i < measurements.Length; i++) {
            TStringDynArray value = SplitString(measurements[i], " ");
            if (MainForm->FindComponent(value[0])) {
                TDdeServerItem * item = (TDdeServerItem *) MainForm->FindComponent(value[0]);
                item->Text = value[1];

                UnicodeString name = value[0], channel = value[0];
                name = name.SubString(0,name.Length()-1);
                // channel =

                if (ArkuszPomiarowy->Rows[0]->IndexOf(name) < 0) {
                    ArkuszPomiarowy->ColCount++;
                    UnicodeString tmp = value[0];
                    ArkuszPomiarowy->Rows[0]->Add(tmp.SubString(0,tmp.Length()-1));
                    ArkuszPomiarowy->Cells[ArkuszPomiarowy->ColCount-1][value[0].SubString(value[0].Length(),1).ToIntDef(0)+1] = value[1];
                } else {
                    ArkuszPomiarowy->Cells[ArkuszPomiarowy->Rows[0]->IndexOf(name)][value[0].SubString(value[0].Length(),1).ToIntDef(0)+1] = value[1];
                }
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
        if (FakeData->Checked == false) {
            if (SendCommand(":HEADER ON;MEAS? " + MeasureQuery->Text) == false) {
                MeasureStop->Click();
            }
        } else {
	        TStringDynArray values = SplitString(MeasureQuery->Text, ",");

            AnsiString fake_sample;

            for (int i = 0; i < values.Length; i++) {
                if (i) fake_sample += ";";
                fake_sample += values[i] + " +" + Random(777) + "." + Random(99) + "E+" + Random(9);
            }
            fake_sample += "\n";

            //char * result = "UFND1 +777.77E+9;UFND2 +777.77E+9;UFND3 +777.77E+9;IFND1 +777.77E+9;IFND2 +777.77E+9;IFND3 +777.77E+9;PTAV0 +00.000E+3;ITAV1 +00.000E+0;ITAV2 +00.000E+0;ITAV3 +00.000E+0\n";
            IdTelnet1DataAvailable(NULL, RawToBytes(fake_sample.c_str(), strlen(fake_sample.c_str())));
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
    ShowLastCommandOnly->Checked = settings->ReadBool("Logging", "ShowLastCommentOnly", ShowLastCommandOnly->Checked);
    OutputTCP->Font->Size = settings->ReadInteger("Logging", "FontSize", OutputTCP->Font->Size);

    MainForm->WindowState = settings->ReadInteger("Window", "State", MainForm->WindowState);
    MainForm->Width = settings->ReadInteger("Window", "Width", MainForm->Width);
    MainForm->Height = settings->ReadInteger("Window", "Height", MainForm->Height);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SaveSettingsClick(TObject *Sender)
{
    settings->WriteString("Connection", "IP", HiokiIP->Text);
    settings->WriteString("Connection", "Port", HiokiPort->Text);
    settings->WriteString("Measurements", "Query", MeasureQuery->Text);
    settings->WriteInteger("Measurements", "Interval", MeasureAction->Interval);
    settings->WriteBool("DDE", "ConvertDotToComma", DotToComma->Checked);
    settings->WriteBool("Logging", "ShowLastCommentOnly", ShowLastCommandOnly->Checked);
    settings->WriteInteger("Logging", "FontSize", OutputTCP->Font->Size);
    settings->WriteInteger("Window", "State", MainForm->WindowState);
    settings->WriteInteger("Window", "Width", MainForm->Width);
    settings->WriteInteger("Window", "Height", MainForm->Height);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    if (IdTelnet1->Connected()) IdTelnet1->Disconnect();
	SaveSettingsClick(this);
}
//---------------------------------------------------------------------------





