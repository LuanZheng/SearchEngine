#include "Spliter.h"
#include <iostream>
using namespace std;

Spliter::Spliter(string inputStr)
{
	this->inputStringId = -1;
	this->inputString = inputStr;
}

Spliter::Spliter(int id, string inputStr)
{
	this->inputStringId = id;
	this->inputString = inputStr;
	//this->inputString = "s华住trin酒店g集团";
}

void Spliter::split(list<string>& strList)
{
	if (-1 != inputStringId)
	{
		return;
	}
	size_t length = inputString.length();
	size_t i = 0;
	while (i < length)
	{
		if (isZhCh(inputString[i]))
		{
			string splitWord = inputString.substr(i, 2);
			i += 2;
			strList.push_back(splitWord);
		}
		else
		{
			string splitWord = inputString.substr(i, 1);
			i += 1;
			strList.push_back(splitWord);
		}
	}
}

void Spliter::split(map<int, list<string>>& strIdMap, map<string, list<int>>& strIdReverseIndexMap, map<int,string>& strIntMapFull)
{
	strIntMapFull.insert(std::pair<int, string>(inputStringId, inputString));

	list<string> strList;
	list<int> stridList;
	stridList.push_back(inputStringId);
	size_t length = inputString.length();

	size_t i = 0;
	while (i < length)
	{
		if (isZhCh(inputString[i]))
		{
			string splitWord = inputString.substr(i, 2);
			i += 2;
			strList.push_back(splitWord);
			//if split word already in map key, find out the map, only add the stridList
			size_t length = strIdReverseIndexMap.size();
			bool keyAlreadyIn = false;
			if (strIdReverseIndexMap.count(splitWord) > 0)
			{
				strIdReverseIndexMap[splitWord].push_back(inputStringId);
				keyAlreadyIn = true;
			}
			if (!keyAlreadyIn)
			{
				strIdReverseIndexMap.insert(std::pair<string, list<int>>(splitWord, stridList));
			}
		}
		else
		{
			string splitWord = inputString.substr(i, 1);
			i += 1;
			strList.push_back(splitWord);
			//if split word already in map key, find out the map, only add the stridList
			size_t length = strIdReverseIndexMap.size();
			bool keyAlreadyIn = false;
			if (strIdReverseIndexMap.count(splitWord) > 0)
			{
				strIdReverseIndexMap[splitWord].push_back(inputStringId);
				keyAlreadyIn = true;
			}
			if (!keyAlreadyIn)
			{
				strIdReverseIndexMap.insert(std::pair<string, list<int>>(splitWord, stridList));
			}
		}
	}
	strIdMap.insert(std::pair<int, list<string>>(inputStringId, strList));
}


bool Spliter::isZhCh(char c)
{
	if (c&0x80)
	{
		return true;//代表是汉字
	}

	return false;
}
