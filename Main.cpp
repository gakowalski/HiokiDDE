//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"

//#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;

UnicodeString get_command(UnicodeString command) {
    UnicodeString tmp;
    tmp = StringReplace(command, "\\n", "\n", TReplaceFlags() << rfReplaceAll);
    return StringReplace(tmp, "\\r", "\r", TReplaceFlags() << rfReplaceAll);
}

UnicodeString TMainForm::WinsockSendCommand(
		const UnicodeString & host,
        const UnicodeString & port,
        const UnicodeString & command
	) {
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL, *ptr, hints;
    int iResult;
	const int recvbuflen = 1024;
    char recvbuf[recvbuflen];

    UnicodeString response = "";

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        return "Unable to load Winsock: " + iResult;
    }

    SecureZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(AnsiString(host).c_str(), AnsiString(port).c_str(), &hints, &result);
    if ( iResult != 0 ) {
        WSACleanup();
        return "getaddrinfo failed with error: " + iResult;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr=result; ptr != NULL; ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            WSACleanup();
            throw new Exception("socket failed with error:" + WSAGetLastError());
        }

        // Connect to server.
        if (connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        WSACleanup();
        return "Unable to connect to server!";
    }

    // Send an initial buffer
    AnsiString sendbuf = AnsiString(get_command(command));

    iResult = send( ConnectSocket, sendbuf.c_str(), (int)strlen(sendbuf.c_str()), 0 );
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        WSACleanup();
        throw new Exception("send failed with error:" + WSAGetLastError());
    }

    OutputTCP->Lines->Add("Bytes Sent: " + iResult);

    // shutdown the connection since no more data will be sent
    if (shutdown(ConnectSocket, SD_SEND) == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        WSACleanup();
        throw new Exception("shutdown failed with error:" + WSAGetLastError());
    }

    // Receive until the peer closes the connection
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 ) {
            OutputTCP->Lines->Add("Bytes received: " + iResult);
            response += recvbuf;
        }
        else if ( iResult == 0 )
            OutputTCP->Lines->Add("Connection closed");
        else
            OutputTCP->Lines->Add("recv failed with error: " + WSAGetLastError());

    } while( iResult > 0 );

    // cleanup
    closesocket(ConnectSocket);

    WSACleanup();
    OutputTCP->Lines->Add("Cleanup");

    return response;
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
        /*while (client->IOHandler->InputBufferIsEmpty() == false) {
            response += client->IOHandler->InputBufferAsString();
            OutputTCP->Lines->Add("Odczytano... " + response.Length());
        }
		OutputTCP->Lines->Add("Odczyt zakonczony!");
        client->Disconnect();                       */
	} else {
        response = "Nie mo¿na ustanowiæ po³¹czenia";
    }

          /*
    TClientSocket * client = ClientSocket1;

    if (client->Socket->Connected == false) {
        client->Port = HiokiPort->Text.ToInt();
        client->Host = HiokiIP->Text;
        client->Active = true;
        client->Open();
    }

	if (client->Socket->Connected) {
        client->Socket->SendText(command + "\n");
        response = client->Socket->ReceiveText();
	    client->Active = false;
    	client->Close();
    }
    */

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
    /*OutputTCP->Lines->Add(WinsockSendCommand(
        HiokiIP->Text,
        HiokiPort->Text,
    	CommandList->Text
    ));
    */


    OutputTCP->Lines->Add(SendCommand(
    	CommandList->Text
    ));

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


