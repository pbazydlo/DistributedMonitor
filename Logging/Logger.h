#ifndef _LOGGER_
#define _LOGGER_

#define LOG_DEBUG 0
#define LOG_INFO  1

class Logger
{
public:
	void Log(char *message, int logLevel);
};
#endif
