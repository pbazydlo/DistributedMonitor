#include "Logger.h"
#include <iostream>
#include <stdlib.h>

void Logger::Log(char *message, int logLevel)
{
 switch(logLevel)
 {
  case LOG_DEBUG:
	std::cout<<"[DEBUG] "<<message<<std::endl;
	break;
  case LOG_INFO:
	std::cout<<"[I] "<<message<<std::endl;
	break;
 };
}
