#include "MessagePrioQueue.h"
#include <iostream>

#include "cryptopp/hex.h"
#include "cryptopp/modes.h"
#include "cryptopp/aes.h"
using namespace CryptoPP;

using namespace std;

MessagePrioQueue* MessagePrioQueue::msgQueueInstance = NULL;

MessagePrioQueue::MessagePrioQueue()
{
	elemNum = 0;
	hasFirstPart = false;
	if (NULL == concatMsg)
	{
		concatMsg = new char[MSG_PLAIN_MAX_SIZE];
		memset(concatMsg, 0, sizeof(char) * MSG_PLAIN_MAX_SIZE);
	}
}

MessagePrioQueue* MessagePrioQueue::getMsgQueueInstance()
{
	if (NULL == msgQueueInstance)
	{
		msgQueueInstance = new MessagePrioQueue();
	}
	return msgQueueInstance;
}

bool MessagePrioQueue::hasConcatPart()
{
	return hasFirstPart;
}

bool MessagePrioQueue::isFull()
{
	if (MSG_QUEUE_MAX_LEN == elemNum)
	{
		return true;
	}
	return false;
}

bool MessagePrioQueue::isEmpty()
{
	if (0 == elemNum)
		return true;
	return false;
}

void MessagePrioQueue::clearMsgConcatInfo()
{
	hasFirstPart = false;
	memset(concatMsg, 0, sizeof(char) * MSG_PLAIN_MAX_SIZE);
	return;
}

void MessagePrioQueue::setConcatPart(const char* firstPartOfMsg)
{
	if (NULL == concatMsg)
	{
		return;
	}
	unsigned int len = strlen(firstPartOfMsg);
	//memcpy(concatMsg, firstPartOfMsg, len*sizeof(char));
	strcat(concatMsg, firstPartOfMsg);
	hasFirstPart = true;
	return;
}

char* MessagePrioQueue::msgConcat(const char* secondPartOfMsg)
{
	if (NULL == concatMsg)
	{
		return NULL;
	}
	else if (!hasFirstPart)
	{
		memset(concatMsg, 0, sizeof(char) * MSG_PLAIN_MAX_SIZE);
		return NULL;
	}
	if (strlen(concatMsg) + strlen(secondPartOfMsg) > MSG_PLAIN_MAX_SIZE)
	{
		//cout << "should not come here" << endl;
		return NULL;
	}
		
	strcat(concatMsg, secondPartOfMsg);
	return concatMsg;
}

bool MessagePrioQueue::msgEnqueue(unsigned int priority, const char* msgbuffer)
{
	if (isFull())
		return false;
	//cout << msgbuffer <<"Len=" <<strlen(msgbuffer) << endl;
	MsgNode* msg = new MsgNode();
	msg->priority = priority;
	//Maybe touch unsafe memory, should go on research later. For new char, to find how many buffer will be pre-allocate
	msg->data = new char[strlen(msgbuffer)+1];
	memset(msg->data, 0, strlen(msgbuffer)*sizeof(char));
	memcpy(msg->data, &msgbuffer[0], strlen(msgbuffer));
	(msg->data)[strlen(msgbuffer)] = '\0';
	if (isEmpty())
	{
		heapArray[0] = msg;
		elemNum++;
	}
	else
	{
		//elemNum++;
		heapArray[elemNum] = msg;
		int parentIndex = (elemNum - 1) / 2;
		int childIndex = elemNum;
		while (parentIndex >= 0)   //has parent
		{
			bool hasSwap = false;
			if (heapArray[childIndex]->priority < heapArray[parentIndex]->priority)
			{
				MsgNode* tempMsg = heapArray[parentIndex];
				heapArray[parentIndex] = heapArray[childIndex];
				heapArray[childIndex] = tempMsg;
				hasSwap = true;
			}
			if (!hasSwap)
			{
				break;
			}
			else
			{
				childIndex = parentIndex;
				parentIndex = (parentIndex - 1) / 2;
			}
		}
		elemNum++;
	}

	return true;
}

MsgNode* MessagePrioQueue::msgDequeue()
{
	if (isEmpty())
		return NULL;
	MsgNode* msg = heapArray[0];
	if (1 == elemNum)
	{
		elemNum--;
	}
	else
	{
		heapArray[0] = heapArray[elemNum-1];
		elemNum--;
		unsigned int index = 0;
		while (2 * index + 1 < elemNum)  //has left child
		{
			bool hasSwap = false;
			//unsigned int highPrioIndex = index;
			if (2 * index + 2 < elemNum)  //has right child
			{
				if (heapArray[2 * index + 1]->priority <= heapArray[2 * index + 2]->priority)
				{
					if (heapArray[index]->priority <= heapArray[2 * index + 1]->priority)
					{
						break;
					}
					else
					{
						//swap the left child and parent
						MsgNode* tempMsg = heapArray[index];
						heapArray[index] = heapArray[2 * index + 1];
						heapArray[2 * index + 1] = tempMsg;
						hasSwap = true;
						index = index * 2 + 1;
					}
				}
				else
				{
					if (heapArray[index]->priority <= heapArray[2 * index + 2]->priority)
					{
						break;
					}
					else
					{
						//swap the right child and parent
						MsgNode* tempMsg = heapArray[index];
						heapArray[index] = heapArray[2 * index + 2];
						heapArray[2 * index + 2] = tempMsg;
						hasSwap = true;
						index = index * 2 + 2;
					}
				}
			}
			else //only has left child
			{
				if (heapArray[index]->priority <= heapArray[2 * index + 1]->priority)
				{
					break;
				}
				else
				{
					//swap the left child and parent
					MsgNode* tempMsg = heapArray[index];
					heapArray[index] = heapArray[2 * index + 1];
					heapArray[2 * index + 1] = tempMsg;
					hasSwap = true;
					break;
				}
			}
		}
	}
	return msg;
}