#include <iostream>
//#include <iomanip>  
#include <fstream> 
#include <stdio.h>
#include <time.h>
#include <vector>
#include <thread>
#include "Spliter.h"
#include "Searcher.h"
#include "ace/Log_Msg.h"
#include "ace/INET_Addr.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "Message.h"
#include "ServerSvcHandler.h"
#include "cryptopp/aes.h"
#include "cryptopp/filters.h"
#include "cryptopp/modes.h"
#include "cryptopp/hex.h"
#include "GlobalVar.h"
#include "Utils.h"
#include "MessagePrioQueue.h"
#include "AbstractMsgFactory.h"
#include "MsgFactory.h"


#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"



using namespace std;
using namespace CryptoPP;
using namespace rapidjson;

#define TIME_MAX 32

void httpRequest(char* url, int port)
{
	ACE_SOCK_Stream remotePonit;

	if (ACE_SOCK_Connector().connect(remotePonit, ACE_INET_Addr(port, url)) != -1) {
		static const char request[] = "GET / HTTP/1.1\r\n\r\n";
		remotePonit.send(request, sizeof(request) - 1); // "- 1": don't send '\0'
		remotePonit.close_writer();

		char buf[65535]; // maximum size of an IP packet

		while (ssize_t len = remotePonit.recv(buf, sizeof(buf))) { // (len == 0) is EOF
			if (len > 0) {
				cout.write(buf, len);
			}

			else if (errno == EINTR) {
				continue;
			}

			else {
				std::cerr << "fatal error: " << std::strerror(errno) << '\n';
				break;
			}
		}
	}

	else {
		cerr << "could not connect: " << strerror(errno) << endl;
		return;
	}
}

void rapidJsonTest()
{
	const char* str = "{\"employees\": [{ \"firstName\":\"John\" , \"lastName\":\"Doe\" },{ \"firstName\":\"Anna\" , \"lastName\":\"Smith\" },{ \"firstName\":\"Peter\" , \"lastName\":\"Jones\"}]}";

	Document doc;
	doc.Parse(str);

	if (doc.HasParseError() == false)
	{
		const Value& employees = doc["employees"];

		// rapidjson uses SizeType instead of size_t.
		for (SizeType i = 0; i < employees.Size(); i++)
		{
			const Value& temp = employees[i];

			printf("firstName=%sďźlastName=%s \n", temp["firstName"].GetString(), temp["lastName"].GetString());
		}
	}
	else {
		printf("parse error\n");
	}
}



double getTime()
{
	clock_t start, finish;
	double elapsed_time;
	start = clock();
	finish = clock();
	elapsed_time = finish - start;
	return elapsed_time;
}

void get_time(void)
{
	time_t now;
	time(&now);

	// 定义两个变量，存储转换结果
	struct tm tmTmp;
	char stTmp[TIME_MAX];

	// 转换为tm结构
	localtime_s(&tmTmp, &now);

	// 转换为字符串并输出
	asctime_s(stTmp, &tmTmp);
	printf("Current time is: %s\n", stTmp);
}

void FileRead()
{
	char buffer[256];
	//ifstream myfile("E://companyName.txt");
	ifstream myfile("E://1.txt");

	if (!myfile) {
		cout << "Unable to open myfile";
		exit(1); // terminate with error  

	}
	string companyName;
	int i = 0, j = 0;
	while (!myfile.eof())
	{
		memset(buffer, 0, 256);
		myfile.getline(buffer, 256);
		companyName = buffer;
		//cout << companyName << endl;
	}
	myfile.close();
	return;
}

void allLegacyMain()
{
	int charByte = sizeof(char);
	int uInt = sizeof(unsigned int);
	char buffer[256];
	memset(buffer, 0, 256 * sizeof(char));
	buffer[0] = 62;
	buffer[1] = 140;
	buffer[2] = 52;
	buffer[3] = 2;
	buffer[4] = 'h';
	buffer[5] = 'i';
	//Message msg;
	//msg.constructMsg(buffer);
	//msg.printMsgBody();

	cout << "HAHAHA" << endl;
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("Hello, ACE !\n")));
	map<int, list<string>> strIdMap;	                        //正向索引
	map<string, list<int>> strIdReverseList;                    //反向索引
	map<int, string> strIdMapFull;                              //全字索引

	list<string> testCompanyList;

	vector<string> testCompanyVector;

	ifstream myfile("E://companyName.txt");
	//ifstream myfile("E://1.txt");

	if (!myfile) {
		cout << "Unable to open myfile";
		exit(1); // terminate with error  
	}
	//char strArray[80000][16];
	//char (*strArray1)[80000];
	//char **pStr = strArray1;
	//char* test = *(pStr++);
	string companyName;
	int i = 0;
	while (!myfile.eof())
	{
		memset(buffer, 0, sizeof(buffer));
		myfile.getline(buffer, sizeof(buffer));
		companyName = buffer;
		Spliter spliter(i, companyName);
		spliter.split(strIdMap, strIdReverseList, strIdMapFull);
		testCompanyList.push_back(companyName);
		testCompanyVector.push_back(companyName);
		i++;
	}
	myfile.close();

	//don't forget delete the memory
	char **strArray = new char*[i];
	for (int j = 0; j < i; j++)
	{
		strArray[j] = new char[128];
		memset(strArray[j], 0, 128 * sizeof(char));
		string companyName = testCompanyList.front();
		memcpy(strArray[j], companyName.c_str(), strlen(companyName.c_str()));
		testCompanyList.pop_front();
	}

	//Spliter spliter(0, "s华住trin酒店g集团");
	//map<int, list<string>> strIdMap;	                        //正向索引
	//map<string, list<int>> strIdReverseList;                    //反向索引
	//map<int, string> strIdMapFull;                              //全字索引
	//spliter.split(strIdMap, strIdReverseList, strIdMapFull);
	////spliter.generateReverseMapIndex(strIdMapReverse);
	//Spliter spliter2(1, "华为技术股份有限公司团");
	//spliter2.split(strIdMap, strIdReverseList, strIdMapFull);
	//Spliter spliter3(2, "中华住ts房公司");
	//spliter3.split(strIdMap, strIdReverseList, strIdMapFull);

	//Searcher searcher;
	//list<int> resultList;
	//searcher.search("华团", &strIdReverseList, resultList);


	Searcher searcher;

	size_t length = strIdMapFull.size();
	clock_t start, finish;
	start = clock();
	const char* searchKeyWords = "华为";
	for (int j = 0; j < i; j++)
	{
		//string value;
		//int length = strlen(*strArray);
		//memcpy(&value, *strArray, length*sizeof(char));
		char* value = *strArray;
		bool rs = searcher.kmpMapping(value, searchKeyWords);
		//if (rs)
		//	cout << value << endl;
		strArray++;

		//bool rs = searcher.kmpMapping(value, "华为");
	}
	finish = clock();
	double elapse = finish - start;
	cout << elapse << endl;

	get_time();
	for (size_t i = 0; i < length; i++)
	{
		string value = strIdMapFull[i];
		bool rs = searcher.kmpMapping(value, "华为");
		//if (rs)
		//{
		//	cout << value << endl;
		//}
	}
	get_time();

	//for (size_t i = 0; i < length; i++)
	//{
	//	string value = testCompanyList[i];
	//}

	//get_time();
	//list<string>::iterator iter;
	//for (iter = testCompanyList.begin(); iter != testCompanyList.end(); iter++)
	//{
	//	string value = *iter;
	//	bool rs = searcher.kmpMapping(value, "华为");
	//}
	//get_time();


	//get_time();
	//vector<string>::iterator iter2;

	//for (iter2 = testCompanyVector.begin(); iter2 != testCompanyVector.end(); iter2++)
	//{
	//	string value = *iter2;
	//	bool rs = searcher.kmpMapping(value, "华为");
	//}
	//get_time();

	int k = 999;
}


void TestAES()
{
	//3E9C340401026162
	//const byte key[AESKEYLENGTH] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08 };
	//char plain[] = {0x3E, 0x9C, 0x34, 0x04, 0x01, 0x02, 0x61, 0x62, 0xFF, 0xFE, 0x3E, 0x9C, 0x38, 0x04, 0x01, 0x02, 0x61, 0x62, 0xFF, 0xFE, 0x3E, 0x9C, 0x3C, 0x04, 0x01, 0x02, 0x61, 0x62, 0xFF, 0xFE, 0x3E, 0x9C, 0x34, 0x04, 0x01, 0x02, 0x61, 0x62, 0xFF, 0xFE,'\0'};
	char plain[] = { 0x3E, 0x9C, 0x34, 0x04, 0x01, 0x02, 0x61, 0x62, '\0' };
	//string source = "123456789012345";
	//string source2 = "123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345";
	string encodeRs = Utils::aesEncoded(plain);
	//string encodeRs2 = Utils::aesEncoded(source2);
	//char* test = &encodeRs[0];
	string decodeRs = Utils::aesDecoded(encodeRs);
	//string decodeRs2 = Utils::aesDecoded(encodeRs2);
	return;
}

void readMsgQueue()
{
	while (true)
	{
		MsgNode* dequeueMsg = NULL;
		std::unique_lock<std::mutex> lk(cv_mutex);
		bool isMsgQueueFull = MessagePrioQueue::getMsgQueueInstance()->isFull();
		dequeueMsg = MessagePrioQueue::getMsgQueueInstance()->msgDequeue();
		if (isMsgQueueFull)
			cv.notify_all();
		if (NULL != dequeueMsg)
		{
			lk.unlock();
			//处理消息
			char* msgBuf = dequeueMsg->data;
			// Pretty print
			//string dequeuedMsg;
			//dequeuedMsg.clear();
			//StringSource(msgBuf, true,
			//	new HexEncoder(
			//		new StringSink(dequeuedMsg)
			//		) // HexEncoder
			//	); // StringSource
			//cout << "Dequeued Msg:" << dequeuedMsg << endl;
			int prio = dequeueMsg->priority;
			unsigned int msgNum = ((msgBuf[0] << 4)&(0xFF0)) + ((msgBuf[1] >> 4)&(0xF));
			bool basicCheck = false;
			AbstractMsgFactory* msgFactory = NULL;
			AbstractMsg* msg = NULL;
			switch (msgNum)
			{
			case MSG_1001_SEARCH_REQ:
				msgFactory = new Msg1001Factory();
				msg = (Msg_1001_Search_Req*)msgFactory->createMsg(msgBuf);
				basicCheck = msg->basicValidationCheck();
				if (!basicCheck)
					break;
				//handle msg
				//在这里添加具体业务处理代码
				//keyWords = ((Msg_1001_Search_Req*)msg)->getKeyWords();

				break;
			default:
				
				ACE_DEBUG((LM_WARNING, "Wrong msg number, no such message.\n")); //打印warn级别日志  
				break;
			}
			if (NULL != msgBuf)
			{
				delete[] msgBuf;
				msgBuf = NULL;
			}
			if (NULL != dequeueMsg)
			{
				delete dequeueMsg;
				dequeueMsg = NULL;
			}
			if (NULL != msg)
			{
				delete msg;
				msg = NULL;
			}
			if (NULL != msgFactory)
			{
				delete msgFactory;
				msgFactory = NULL;
			}
		}
		else
		{
			//线程阻塞,自动释放锁lk
			cv.wait(lk);
		}
	}
}

void someStudyTest()
{
	//char c[10];
	//int kkk = sizeof(c);
	//int* i = new int[10];
	//for (int kki = 0; kki < 10; kki++)
	//{
	//	*i = kki;
	//	i++;
	//}

	//int jj = sizeof(p);
	//jj = strlen(p);

	//memory leak case
	//int *p = new int;
	//int *q = new int;
	//*q = 5;
	//p = q;  memory leak, old p could not release
	//int kaa = 999999;
	//delete q;
	//For the char new, memset is must. Then for memcpy, pay attention to left the end location '\0'
	//char* test = new char;
	//cout << strlen(test) << endl;
	//memset(test, 0, strlen(test)*sizeof(char));
	//cout << strlen(test) << endl;
	//char aaa[3] = {0x3E, 0x4E};
	//test = aaa;
	//cout << strlen(test) << endl;
}


typedef ACE_Acceptor<ServerSvcHandler, ACE_SOCK_ACCEPTOR> ServerAcceptor;

int ACE_TMAIN(int, ACE_TCHAR *[])
{
	thread readMsgQueueT(readMsgQueue);
	readMsgQueueT.detach();
	ACE_DEBUG((LM_DEBUG, "Test server running...\n"));
	const int PORT_NUM = 7700;
	ACE_INET_Addr addr(PORT_NUM);
	ServerAcceptor acceptor(addr, ACE_Reactor::instance());
	ACE_Reactor::run_event_loop(); //会一直不停的监听注册的事件。
	return 0;
}