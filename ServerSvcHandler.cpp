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
	ACE_DEBUG((LM_DEBUG, "%P|%t, %s", " Connection established.\n")); //��ӡdebug�����log��%P%t��ʾ���̺ź��̺߳ţ�%s��Ҫ��ӡ����Ϣ��  
	ACE_Reactor::instance()->register_handler(this, ACE_Event_Handler::READ_MASK); //ע��socket�Ͽɶ��¼��Ĵ�����������ǰ��socket���пɶ��¼�ʱ���ͻᴥ�����handler��handle_input������  
	//char* msg = "Client connected successfully in server.";
	//peer().send(msg, strlen(msg));
	return 0;
}

int ServerSvcHandler::handle_input(ACE_HANDLE)
{
	ssize_t cnt = peer().recv(data, MSG_BUFFER_MAX_SIZE);
	if (cnt <= 0 || (strlen(data) <= 4) || (strlen(data) > MSG_BUFFER_MAX_SIZE))
	{
		ACE_DEBUG((LM_WARNING, " No data received or receiced data invalid.\n")); //��ӡwarn������־  
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
