//
//  Logging.cpp
//  UGLEngine
//
//  Created by Jared Jones on 4/8/14.
//  Copyright (c) 2014 Jared Jones. All rights reserved.
//

#include "Logging.h"
#include <cstdarg>

time_t UNIXTIME;

void Logging::Init()
{
    const char* logNormalFilename = NULL;
    const char* logErrorFilename = NULL;
    
    logNormalFilename = "UGLEngine-Normal.log";
    logErrorFilename = "UGLEngine-Error.log";
    
    
    m_normalFile = fopen(logNormalFilename, "a");
    if (m_normalFile == NULL)
        fprintf(stderr, "%s: Error opening '%s': %s\n", __FUNCTION__, logNormalFilename, strerror(errno));
    else
    {
        struct tm *now = localtime(&UNIXTIME);
        OutBasic("Unix Clock Seed:[%-4d-%02d-%02d %02d:%02d:%02d] ", now->tm_year + UNIXSTARTYEAR, now->tm_mon + 1,
                 now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
    }
    
    m_errorFile = fopen(logErrorFilename, "a");
	if(m_errorFile == NULL)
		fprintf(stderr, "%s: Error opening '%s': %s\n", __FUNCTION__, logErrorFilename, strerror(errno));
	else
	{
		struct tm *now = localtime(&UNIXTIME);
		// We don't echo time and date again because outBasic above just echoed them.
		OutErrorSilent("[%-4d-%02d-%02d %02d:%02d:%02d] ", now->tm_year + UNIXSTARTYEAR, now->tm_mon + 1,
                       now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
	}
    
}

void Time(char* buffer)
{
	time_t now;
	struct tm* timeinfo = NULL;
    
	time(&now);
	timeinfo = localtime(&now);
    
	if(timeinfo != NULL)
	{
		strftime(buffer, TIME_FORMAT_LENGTH, TIME_FORMAT, timeinfo);
	}
	else
	{
		buffer[0] = '\0';
	}
}

void Logging::outFile(FILE* file, char* msg, const char* source)
{
	char time_buffer[TIME_FORMAT_LENGTH];
	char szltr_buffer[SZLTR_LENGTH];
	Time(time_buffer);
	snprintf(szltr_buffer, sizeof(szltr_buffer), "[UGLEngine]");
    
	if(source != NULL)
	{
		fprintf(file, "%s%s%s: %s\n", time_buffer, szltr_buffer, source, msg);
		printf("%s%s%s: %s\n", time_buffer, szltr_buffer, source, msg);
	}
	else
	{
		fprintf(file, "%s%s%s\n", time_buffer, szltr_buffer, msg);
		printf("%s%s%s\n", time_buffer, szltr_buffer, msg);
	}
}

void Logging::outFileSilent(FILE* file, char* msg, const char* source)
{
	char time_buffer[TIME_FORMAT_LENGTH];
	char szltr_buffer[SZLTR_LENGTH];
	Time(time_buffer);
	snprintf(szltr_buffer, sizeof(szltr_buffer), "[TUMORS]");
    
	if(source != NULL)
	{
		fprintf(file, "%s%s%s: %s\n", time_buffer, szltr_buffer, source, msg);
	}
	else
	{
		fprintf(file, "%s%s%s\n", time_buffer, szltr_buffer, msg);
	}
}

void Logging::OutErrorSilent(const char* str, ...)
{
    if (m_errorFile == NULL)
        return;
    char buf[32768];
    va_list vaList;
    
    va_start(vaList, str);
    vsnprintf(buf, sizeof(buf), str, vaList);
    va_end(vaList);
    
    outFileSilent(m_normalFile, buf);
}

void Logging::OutBasic(const char* str, ...)
{
    if (m_normalFile == NULL)
        return;
    char buf[32768];
    va_list vaList;
    
    va_start(vaList, str);
    vsnprintf(buf, sizeof(buf), str, vaList);
    va_end(vaList);
    
    outFile(m_normalFile, buf);
}

void Logging::LogBasic(const char* /*file*/, int /*line*/, const char* /*fncname*/, const char* /*msg*/, ...)
{
    
}

void Logging::enableColor(std::string msg)
{
#ifndef DEBUG
    std::cout << msg;
#endif
}
void Logging::disableColor()
{
#ifdef DEBUG
#else
    //std::cout << RESET;
#endif
}

void Logging::Close()
{
    if (m_normalFile != NULL)
    {
        fflush(m_normalFile);
        fclose(m_normalFile);
        free(m_normalFile);
        m_normalFile = NULL;
    }
    if (m_errorFile != NULL)
    {
        fflush(m_errorFile);
        fclose(m_errorFile);
        free(m_errorFile);
        m_errorFile = NULL;
    }
}