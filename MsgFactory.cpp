#include "MsgFactory.h"
#include "Message.h"
#include <iostream>

Msg1001Factory::Msg1001Factory()
{

}

Msg1001Factory::~Msg1001Factory()
{

}

AbstractMsg* Msg1001Factory::createMsg(const char* msgBuffer)
{
	if ((strlen(msgBuffer) <= MSG_HEADER_LEN) || (strlen(msgBuffer) > MSG_PLAIN_MAX_SIZE))
		return NULL;
	AbstractMsg* msg1001 = NULL;
	msg1001 = new Msg_1001_Search_Req(msgBuffer);
	return msg1001;
}