#ifndef _MSG_FACTORY_H_
#define _MSG_FACTORY_H_
#include "AbstractMsgFactory.h"

class Msg1001Factory : public AbstractMsgFactory
{
public:
	Msg1001Factory();
	~Msg1001Factory();
	AbstractMsg* createMsg(const char* msgBuffer);
};

#endif // !_MSG_FACTORY_H_

