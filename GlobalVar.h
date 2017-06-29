#ifndef _GLOBAL_VAR_H_
#define _GLOBAL_VAR_H_

#include <iostream>
#include <condition_variable>
#include <mutex>

extern std::condition_variable cv;
extern std::mutex cv_mutex;

#endif // !_GLOBAL_VAR_H_

