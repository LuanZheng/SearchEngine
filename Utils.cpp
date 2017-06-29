#include "Utils.h"
#include <iostream>
#include "cryptopp/hex.h"
#include "cryptopp/modes.h"
#include "cryptopp/aes.h"
using namespace CryptoPP;

#include "MessagePrioQueue.h"
#include "Searcher.h"


char* Utils::createCharArray(int length)
{
	char* charArrayPointer = NULL;
	charArrayPointer = new char[length+1];  //char array need add an '\0' in the last, so create len should be len+1
	if (NULL != charArrayPointer)
	{
		memset(charArrayPointer, 0, length + 1);
	}	
	return charArrayPointer;
}

char* Utils::createCharArray(int length, char* src, int srcLenth)
{
	char* charArrayPointer = Utils::createCharArray(length);
	if (NULL != charArrayPointer)
	{
		memcpy(charArrayPointer, src, srcLenth);
	}
	return charArrayPointer;
}

//bool Utils::splitMsg(string msgString)
//{
//	size_t msgBufLen = msgString.length();
//	const char* msgSpliterPointer = MSG_SPLITER_TAG;
//	size_t pos = 0;
//	size_t index = msgString.find(msgSpliterPointer, pos);
//	while (string::npos != index)
//	{
//		//Don't know why direct use const char* msg = msgString.substr(pos, index - pos).c_str(); will be empty??????
//		string temMsg = msgString.substr(pos, index - pos);
//		const char* encodeMsg = temMsg.c_str();
//		const char* msg = Utils::aesDecoded(encodeMsg).c_str();
//		// Pretty print
//		//string encoded;
//		//encoded.clear();
//		//StringSource(msg, true,
//		//	new HexEncoder(
//		//		new StringSink(encoded)
//		//		) // HexEncoder
//		//	); // StringSource
//		//cout << "debug text: " << encoded << endl;
//		//should add a basic check for *msg
//		std::unique_lock<std::mutex> lk(cv_mutex);
//		MessagePrioQueue::getMsgQueueInstance()->msgEnqueue((msg[2] >> 2) & (0x3), msg);
//		if (MessagePrioQueue::getMsgQueueInstance()->isFull())
//		{
//			cv.wait(lk);
//		}
//		else
//		{
//			lk.unlock();
//		}
//		pos = index + MSG_SPLITER_LEN;
//		if (pos >= msgBufLen)
//			break;
//		index = msgString.find(msgSpliterPointer, pos);
//	}
//	return true;
//}

int Utils::find(const char* src, const char* key, const int pos)
{
	int srcLen = strlen(src);
	int keyLen = strlen(key);
	if (pos >= srcLen)
	{
		return NPOS;
	}
	const char* searchSrc = &src[pos];
	Searcher searcher;
	int startLoc = NPOS;
	bool rs = searcher.kmpMapping(searchSrc, key, startLoc);
	if (!rs)
	{
		return NPOS;
	}
	return startLoc;
}

bool Utils::splitMsg(char* msgBuffer)
{
	size_t msgBufLen = strlen(msgBuffer);
	const char* msgSpliterPointer = MSG_SPLITER_TAG;
	size_t pos = 0;
	int index = find(msgBuffer, msgSpliterPointer, pos);
	while (NPOS != index)
	{
		char* encodeMsg = NULL;
		index = index + pos;
		//Don't know why direct use const char* msg = msgString.substr(pos, index - pos).c_str(); will be empty??????
		if (MessagePrioQueue::getMsgQueueInstance()->hasConcatPart())
		{
			char* secondMsgPart = new char[index - pos + 1];
			if (NULL != secondMsgPart)
			{
				memset(secondMsgPart, 0, (index - pos + 1)*sizeof(char));
				memcpy(secondMsgPart, &msgBuffer[pos], index - pos);
				char* msgConcatFull = MessagePrioQueue::getMsgQueueInstance()->msgConcat(secondMsgPart);
				if (NULL == msgConcatFull)
				{
					MessagePrioQueue::getMsgQueueInstance()->clearMsgConcatInfo();
				}
				else
				{
					unsigned int msgConcatFullLen = strlen(msgConcatFull);


					encodeMsg = new char[msgConcatFullLen + 1];
					if (NULL != encodeMsg)
					{
						memset(encodeMsg, 0, msgConcatFullLen + 1);
						memcpy(encodeMsg, msgConcatFull, msgConcatFullLen);
					}
					delete[] secondMsgPart;
					secondMsgPart = NULL;
					if (NULL != encodeMsg)
					{
						memset(encodeMsg, 0, msgConcatFullLen + 1);
						memcpy(encodeMsg, msgConcatFull, msgConcatFullLen);
					}
				}
			}
			else
			{
				//could not concat, also clear the first part, and continue
			}
		}
		else
		{
			encodeMsg = new char[index - pos + 1];
			if (NULL != encodeMsg)
			{
				memset(encodeMsg, 0, (index - pos + 1)*sizeof(char));
				memcpy(encodeMsg, &msgBuffer[pos], index - pos);
			}
			else
				return false;
		}
		if (NULL != encodeMsg)
		{
			string tempEncodeStr = Utils::aesDecoded(encodeMsg);
			if (tempEncodeStr.empty())
			{
				delete[] encodeMsg;
				encodeMsg = NULL;
			}
			else
			{
				const char* msg = tempEncodeStr.c_str();
				delete[] encodeMsg;
				encodeMsg = NULL;

				//should add a basic check for *msg
				std::unique_lock<std::mutex> lk(cv_mutex);
				bool isMsgQueueEmpty = MessagePrioQueue::getMsgQueueInstance()->isEmpty();
				MessagePrioQueue::getMsgQueueInstance()->msgEnqueue((msg[2] >> 2) & (0x3), msg);
				if (MessagePrioQueue::getMsgQueueInstance()->hasConcatPart())
				{
					MessagePrioQueue::getMsgQueueInstance()->clearMsgConcatInfo();
				}
				if (MessagePrioQueue::getMsgQueueInstance()->isFull())
				{
					cv.wait(lk);
				}
				else
				{
					lk.unlock();
					if (isMsgQueueEmpty)
						cv.notify_all();
				}
			}
		}

		pos = index + MSG_SPLITER_LEN;
		if (pos >= msgBufLen)
			break;
		index = find(msgBuffer, msgSpliterPointer, pos);
		if (NPOS == index)
		{
			//save the buffer
			int totalLen = strlen(msgBuffer);
			char* encodePartMsg = NULL;
			encodePartMsg = new char[totalLen - pos + 1];
			if (NULL != encodePartMsg)
			{
				memset(encodePartMsg,0,sizeof(char)*(totalLen - pos + 1));
				memcpy(encodePartMsg, &msgBuffer[pos], sizeof(char)*(totalLen - pos));
			}
			else
			{
				continue;
			}
			MessagePrioQueue::getMsgQueueInstance()->setConcatPart(encodePartMsg);
			delete[] encodePartMsg;
			encodePartMsg = NULL;
		}
	}
	return true;
}


string Utils::aesEncoded(string plainStr)
{
	string cipher;
	cipher.clear();
	try
	{
		ECB_Mode<AES>::Encryption e;
		e.SetKey(key, sizeof(key));

		// The StreamTransformationFilter adds padding
		//  as required. ECB and CBC Mode must be padded
		//  to the block size of the cipher.
		StringSource(plainStr, true,
			new StreamTransformationFilter(e,
				new StringSink(cipher)
				) // StreamTransformationFilter      
			); // StringSource
	}
	catch (const CryptoPP::Exception& e)
	{
		cerr << e.what() << endl;
		cipher.clear();
	}
	return cipher;
}

string Utils::aesDecoded(string encodedStr)
{
	// Pretty print
	//string printMsg;
	//printMsg.clear();
	//StringSource(encodedStr, true,
	//	new HexEncoder(
	//		new StringSink(printMsg)
	//		) // HexEncoder
	//	); // StringSource
	//cout << "The original encoded str is:" << printMsg << endl;

	string decodeStr;
	decodeStr.clear();
	try
	{
		ECB_Mode< AES >::Decryption d;
		d.SetKey(key, sizeof(key));

		// The StreamTransformationFilter removes
		//  padding as required.
		StringSource s(encodedStr, true,
			new StreamTransformationFilter(d,
				new StringSink(decodeStr)
				) // StreamTransformationFilter
			); // StringSource
	}
	catch (const CryptoPP::Exception& e)
	{
		cerr << e.what() << endl;
		decodeStr.clear();
	}
	return decodeStr;
}