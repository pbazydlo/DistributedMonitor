#ifndef _LOGGER_
#define _LOGGER_

#define LOG_DEBUG 0

class Logger
{
public:
	void Log(char *message, int logLevel);
};
#endif
