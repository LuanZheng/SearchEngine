#ifndef _SERVER_SVC_HANDLER_H_
#define _SERVER_SVC_HANDLER_H_

#include "ace/Svc_Handler.h"
#include "ace/SOCK_Stream.h"
#include "Constant.h"

class ServerSvcHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
	ServerSvcHandler();
	~ServerSvcHandler();
	int open(void*);
	int handle_input(ACE_HANDLE);
private:
	char *data = NULL;
};

#endif