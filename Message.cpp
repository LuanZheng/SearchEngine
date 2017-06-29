#include "Message.h"
#include <iostream>
#include "Utils.h"
using namespace std;

Msg_1001_Search_Req::Msg_1001_Search_Req(const char* buffer)
{
	if ((strlen(buffer) <= MSG_HEADER_LEN) || (strlen(buffer) > MSG_PLAIN_MAX_SIZE))   //When come here, the msg should be already AES Decoded, so max len should 255
	{
		return;
	}
	memset(&header, 0, sizeof(MessageHeader));
	unsigned int header0 = buffer[0];
	unsigned int header1 = buffer[1];
	unsigned int header2 = buffer[2];
	unsigned int header3 = buffer[3];
	header.msgnum = ((header0 << 4)&(0xFF0)) + ((header1 >> 4)&(0xF));
	header.src = header1 & 0xF;
	header.dest = (header2 >> 4) & (0xF);
	header.pri = (header2 >> 2) & (0x3);
	header.length = header3;



	unsigned int bodySize = header.length * sizeof(char);
	if (NULL == msgBody)
	{
		msgBody = Utils::createCharArray(header.length);
		memcpy(msgBody, &buffer[MSG_HEADER_LEN], bodySize);
	}
	else
	{
		delete[] msgBody;
		msgBody = NULL;
		msgBody = Utils::createCharArray(header.length);
	}

	unsigned int tag = msgBody[0];
	unsigned int len = msgBody[1];
	if (NULL == keyWords)
	{
		keyWords = Utils::createCharArray(len);
		memcpy(keyWords, &(msgBody[2]), len*sizeof(char));
	}


	return;
}

Msg_1001_Search_Req::~Msg_1001_Search_Req()
{
	//cout << "~Msg_1001_Search_Req" << endl;
	if (NULL != keyWords)
	{
		delete[] keyWords;
		keyWords = NULL;
	}
}

char* Msg_1001_Search_Req::getKeyWords()
{
	return keyWords;
}

bool Msg_1001_Search_Req::basicValidationCheck()
{
	if (NULL == msgBody)
		return false;
	unsigned int msgBodyLen = header.length;
	unsigned int tag1ElemLen = msgBody[1];
	if ((msgBodyLen > MSG_PLAIN_MAX_SIZE - MSG_HEADER_LEN) || (tag1ElemLen > MSG_PLAIN_MAX_SIZE - MSG_HEADER_LEN - KEYWORDS_LEN_LOCATION))
	{
		return false;
	}
	return true;
}