#ifndef _ABSTRACT_MSG_H_
#define _ABSTRACT_MSG_H_
#include "Constant.h"

struct  MessageHeader
{
	unsigned int msgnum : 12;
	unsigned int src : 4;
	unsigned int dest : 4;
	unsigned int pri : 2;
	unsigned int reserved : 2;
	unsigned int length : 8;
};


class AbstractMsg
{
public:
	AbstractMsg();
	virtual ~AbstractMsg() = 0;
	virtual bool basicValidationCheck() = 0;
protected:
	MessageHeader header;
	char *msgBody = NULL;
};

#endif // !_ABSTRACT_MSG_H_

