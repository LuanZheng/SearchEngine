#include "ServerSvcHandler.h"
#include <iostream>
#include "MsgFactory.h"
#include "Message.h"
#include "Utils.h"
#include "cryptopp/hex.h"
#include "cryptopp/modes.h"
#include "cryptopp/aes.h"
using namespace CryptoPP;
using namespace std;

ServerSvcHandler::ServerSvcHandler()
{
	data = Utils::createCharArray(MSG_BUFFER_MAX_SIZE);
}

ServerSvcHandler::~ServerSvcHandler()
{
	if (NULL != data)
	{
		delete[] data;
	}
	data = NULL;
}

int ServerSvcHandler::open(void*)
{
	ACE_DEBUG((LM_DEBUG, "%P|%t, %s", " Connection established.\n")); //打印debug级别的log，%P%t表示进程号和线程号，%s是要打印的消息。  
	ACE_Reactor::instance()->register_handler(this, ACE_Event_Handler::READ_MASK); //注册socket上可读事件的处理器，当当前的socket上有可读事件时，就会触发这个handler的handle_input方法。  
	//char* msg = "Client connected successfully in server.";
	//peer().send(msg, strlen(msg));
	return 0;
}

int ServerSvcHandler::handle_input(ACE_HANDLE)
{
	ssize_t cnt = peer().recv(data, MSG_BUFFER_MAX_SIZE);
	if (cnt <= 0 || (strlen(data) <= 4) || (strlen(data) > MSG_BUFFER_MAX_SIZE))
	{
		ACE_DEBUG((LM_WARNING, " No data received or receiced data invalid.\n")); //打印warn级别日志  
		return -1;
	}
	else
	{
		// Pretty print
		string encodedRawData;
		encodedRawData.clear();
		StringSource(data, true,
			new HexEncoder(
				new StringSink(encodedRawData)
				) // HexEncoder
			); // StringSource
		cout << "encodedRawData: " << encodedRawData << endl;
		//Call split method, split the decodeBuffer and insert into msg queue
		Utils::splitMsg(data);
		//char* msg = "Request is processed successfully";
		//peer().send(msg, strlen(msg));
	}
	return 0;
}
