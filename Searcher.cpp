#include "Searcher.h"
#include <iostream>
#include "Spliter.h"

using namespace std;

Searcher::Searcher()
{
	this->allFindInSearchSpace = true;
}

Searcher::~Searcher()
{
	if (NULL != nextArray)
	{
		delete[] nextArray;
		nextArray = NULL;
	}
}

void Searcher::search(string serchContent, map<string, list<int>>* searchSpace, list<int>& resultList)
{
	list<list<int>> contentListList;
	list<string> strList;
	strList.clear();
	Spliter spliter(serchContent);
	spliter.split(strList);
	if (strList.empty())
		return;
	else
	{
		list<string>::iterator iter;
		for (iter = strList.begin(); iter != strList.end(); iter++)
		{
			if (searchSpace->count(*iter) > 0)
			{
				//Find the keywords in serchspace
				list<int> contentList = (*searchSpace)[*iter];
				contentListList.push_back(contentList);
			}
			else
			{
				allFindInSearchSpace = false;
				break;
			}
		}
	}

	if (allFindInSearchSpace)
	{
		zipperMerge(&contentListList, resultList);
	}

	return;
}

void Searcher::zipperMerge(list<list<int>>* contentListList, list<int>& resultList)
{
	size_t contentListListLength = contentListList->size();
	list<int>* contentListPointerArray = new list<int>[contentListListLength];
	list<list<int>>::iterator iter;
	size_t i = 0;
	for (iter = contentListList->begin(); iter != contentListList->end(); iter++, i++)
	{
		contentListPointerArray[i] = *iter;
	}

	int startValue = contentListPointerArray[0].front();

	for (i = 0; i < contentListListLength; i++)
	{
		int value = contentListPointerArray[i].front();
		if (value == startValue)
		{
			if (i == contentListListLength - 1)
			{
				//find out, the value should be saved, all pointer move to next
				resultList.push_back(value);
				for (size_t k = 0; k < contentListListLength; k++)
				{
					contentListPointerArray[k].pop_front();
					if (contentListPointerArray[k].empty())
					{
						return;
					}
				}
				i = -1;  //Need optimize
				continue;
			}
			else
				continue;
		}
		else if (value < startValue)
		{
			contentListPointerArray[i].pop_front();
			if (contentListPointerArray[i].empty())
			{
				return;
			}
			i--;
		}
		else
		{
			for (size_t j = 0; j < i; j++)
			{
				contentListPointerArray[j].pop_front();
				if (contentListPointerArray[j].empty())
				{
					return;
				}
			}
			i = -1;
			startValue = contentListPointerArray[0].front();
		}
	}
}


//nextArr means while current position compare failed, the compare should start at which position
void Searcher::getNext(const char *pStr, int *nextArr)
{
	int i = 0, k = -1, pLen = strlen(pStr);
	nextArr[i] = k;
	int mLen = pLen - 1;
	while (i < mLen)
	{
		if (k == -1 || pStr[i] == pStr[k])
		{
			i++;
			k++;
			if (pStr[i] == pStr[k])
			{
				nextArr[i] = nextArr[k];
			}
			else
			{
				nextArr[i] = k;
			}
		}
		else 
			k = nextArr[k];
	}
	//for (int j = 0; j < pLen; j++)
	//{
	//	cout << nextArr[j] << endl;
	//}
}

bool Searcher::kmpMapping(string searchSpaceStr, string patternStr)
{
	const char* pSearchSpaceStr = searchSpaceStr.c_str();
	const char* pPatternStr = patternStr.c_str();

	if (NULL == nextArray)
	{
		nextArray = new int[strlen(pPatternStr)];
		getNext(pPatternStr, nextArray);
	}

	int i = 0; 
	int j = 0;

	int searchSpaceLength = searchSpaceStr.length();
	int patternStrLength = patternStr.length();

	while (i < searchSpaceLength && j < patternStrLength)
	{
		if (j == -1 || pSearchSpaceStr[i] == pPatternStr[j])
		{
			i++;
			j++;
		}
		else
		{
			j = nextArray[j];
		}
	}

	if (j == patternStrLength)
	{
		return true;
	}
	return false;
}

bool Searcher::kmpMapping(const char* pSearchSpaceStr, const char* pPatternStr)
{
	if (NULL == nextArray)
	{
		nextArray = new int[strlen(pPatternStr)];
		getNext(pPatternStr, nextArray);
	}

	int i = 0;
	int j = 0;

	int searchSpaceLength = strlen(pSearchSpaceStr);
	int patternStrLength = strlen(pPatternStr);

	while (i < searchSpaceLength && j < patternStrLength)
	{
		if (j == -1 || pSearchSpaceStr[i] == pPatternStr[j])
		{
			i++;
			j++;
		}
		else
		{
			j = nextArray[j];
		}
	}

	if (j == patternStrLength)
	{
		return true;
	}
	return false;
}

bool Searcher::kmpMapping(const char* pSearchSpaceStr, const char* pPatternStr, int &startLocation)
{
	if (NULL == nextArray)
	{
		nextArray = new int[strlen(pPatternStr)];
		getNext(pPatternStr, nextArray);
	}

	int i = 0;
	int j = 0;

	int searchSpaceLength = strlen(pSearchSpaceStr);
	int patternStrLength = strlen(pPatternStr);

	while (i < searchSpaceLength && j < patternStrLength)
	{
		if (j == -1 || pSearchSpaceStr[i] == pPatternStr[j])
		{
			i++;
			j++;
		}
		else
		{
			j = nextArray[j];
		}
	}

	if (j == patternStrLength)
	{
		startLocation = i-j;  //start pos
		return true;
	}
	return false;
}

bool Searcher::isZhCh(char c)
{
	if (c & 0x80)
	{
		return true;//´ú±íÊÇºº×Ö
	}

	return false;
}