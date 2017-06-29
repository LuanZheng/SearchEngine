#include "AbstractMsg.h"
#include <iostream>
using namespace std;

AbstractMsg::AbstractMsg()
{

}

AbstractMsg::~AbstractMsg()
{
	//cout << "~AbstractMsg" << endl;
	if (NULL != msgBody)
	{
		delete[] msgBody;
		msgBody = NULL;
	}
}