#ifndef _SEARCHER_H_
#define _SEARCHER_H_

#include <string>
#include <list>
#include <map>
using namespace std;

class Searcher
{
public:
	Searcher();
	~Searcher();
	void search(string searchContent, map<string, list<int>>* searchSpace, list<int>& resultList);
	bool kmpMapping(const char* pSearchSpaceStr, const char* pPatternStr);
	bool kmpMapping(const char* pSearchSpaceStr, const char* pPatternStr, int &startLocation);
	bool kmpMapping(string searchSpaceStr, string patternStr);
private:
	bool isZhCh(char c);
	void zipperMerge(list<list<int>>* contentListList, list<int>& resultList);
	void getNext(const char *pStr, int *nextArr);
	bool allFindInSearchSpace;
	int *nextArray = NULL;
};

#endif