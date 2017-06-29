#ifndef _UTILS_H_
#define _UTILS_H_
#include "GlobalVar.h"
#include <iostream>
#include "Constant.h"


using namespace std;

class Utils
{
public:
	static char* createCharArray(int length);
	static char* createCharArray(int length, char* src, int srcLenth);
	//static bool splitMsg(string msgString);
	static bool splitMsg(char* msgBuffer);
	static string aesEncoded(string plainStr);
	static string aesDecoded(string encodeStr);
	static int find(const char* src, const char* key, int pos);
};

#endif // !_UTILS_H_

