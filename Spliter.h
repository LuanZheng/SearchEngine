#ifndef _SPLITER_H_
#define _SPLITER_H_

#include <string>
#include <list>
#include <map>

using namespace std;

class Spliter
{
public:
	Spliter(string inputStr);
	Spliter(int id, string inputStr);
	void split(map<int, list<string>>& strIdMap, map<string, list<int>>& strIdReverseIndexMap, map<int,string>& strIdMapFull);
	void split(list<string>& strList);
private:
	int inputStringId;
	bool isZhCh(char c);
	string inputString;
};

#endif // !_SPLIT_WORDS_H_

