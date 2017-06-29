#ifndef _ABSTRACT_MSG_FACTORY_H_
#define _ABSTRACT_MSG_FACTORY_H_

#include "AbstractMsg.h"

class AbstractMsgFactory
{
public:
	AbstractMsgFactory();
	virtual ~AbstractMsgFactory();
	virtual AbstractMsg* createMsg(const char* msgBuffer) = 0;
protected:
	char* msgBuffer;
};

#endif
