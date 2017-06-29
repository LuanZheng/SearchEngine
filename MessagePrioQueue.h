#ifndef _MESSAGE_PRIO_QUEUE_H_
#define _MESSAGE_PRIO_QUEUE_H_
#include <mutex>
#include <condition_variable>
#include "Constant.h"


struct MsgNode
{
	unsigned int priority;               
	char* data;                 //data
};

class MessagePrioQueue
{
public:
	static MessagePrioQueue* getMsgQueueInstance();
	bool msgEnqueue(unsigned int priority, const char* msgbuffer);
	MsgNode* msgDequeue();
	bool isFull();
	bool isEmpty();
	bool hasConcatPart();
	void setConcatPart(const char* firstPartOfMsg);
	char* msgConcat(const char* secondPartOfMsg);
	void clearMsgConcatInfo();
private:
	MessagePrioQueue();
	static MessagePrioQueue* msgQueueInstance;
	MsgNode* heapArray[MSG_QUEUE_MAX_LEN];
	unsigned int elemNum;
	bool hasFirstPart;
	char* concatMsg = NULL;
};

#endif
