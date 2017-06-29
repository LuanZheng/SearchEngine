#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#include "Constant.h"
#include "AbstractMsg.h"

class Msg_1001_Search_Req : public AbstractMsg
{
public:
	Msg_1001_Search_Req(const char* buffer);
	~Msg_1001_Search_Req();
	char* getKeyWords();
	virtual bool basicValidationCheck();
private:
	static const int KEYWORDS_LEN_LOCATION = 2;
	char* keyWords = NULL;
};

#endif
