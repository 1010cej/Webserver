#pragma once

#include<cstdarg>
#include<cstdio>
#include<ctime>

#define DEBUG   0
#define NORMAL  1
#define WARNING 2
#define ERROR   3
#define FATAL   4

static const char *gLevelMap[] = {
    "DEBUG",
    "NORMAL",
    "WARNING",
    "ERROR",
    "FATAL"
};

void logMessage(int level, const char* format, ...)
{
#ifndef DEBUG_SHOW
    if(level == 0) return;
#endif

    time_t timeStamp = time(nullptr);
    char stdBuffer[1024];
    snprintf(stdBuffer, sizeof stdBuffer, "[%s] [%ld]", gLevelMap[level], timeStamp, __FILE__, __LINE__);

    char logBuffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(logBuffer, sizeof logBuffer, format, args);
    va_end(args);

    printf("%s %s", stdBuffer, logBuffer);
    // FILE* ptr = fopen("log.txt", "w");
    // fprintf(ptr, "%s %s", stdBuffer, logBuffer);
    // fclose(ptr);
}