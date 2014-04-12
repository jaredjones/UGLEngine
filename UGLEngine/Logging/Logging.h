//
//  Logging.h
//  UGLEngine
//
//  Created by Jared Jones on 4/8/14.
//  Copyright (c) 2014 Jared Jones. All rights reserved.
//

#ifndef __UGLEngine__Logging__
#define __UGLEngine__Logging__

#include <iostream>
#include "Common.h"

#define sLog Logging::getInstance()

#define SZLTR "\xe5\xcf\xfe\xed\xf3\xfb\x03\xeb"
#define SZLTR_LENGTH 9
#define TIME_FORMAT "[%m-%d-%Y][%H:%M:%S]"
#define TIME_FORMAT_LENGTH 100

enum LogLevel
{
    LOG_LEVEL_DISABLED                           = 0,
    LOG_LEVEL_TRACE                              = 1,
    LOG_LEVEL_DEBUG                              = 2,
    LOG_LEVEL_INFO                               = 3,
    LOG_LEVEL_WARN                               = 4,
    LOG_LEVEL_ERROR                              = 5,
    LOG_LEVEL_FATAL                              = 6
};

class Logging
{
private:
    FILE* m_normalFile, *m_errorFile;
    void enableColor(std::string);
    void disableColor();
    void outFileSilent(FILE* file, char* msg, const char* source = NULL);
    void outFile(FILE* file, char* msg, const char* source = NULL);
    
    
    
public:
    void Init();
    void OutErrorSilent(const char* str, ...);
    void OutBasic(const char* str, ...);
    void LogBasic(const char* file, int line, const char* fncname, const char* msg, ...);
    void Close();
    
    static Logging& getInstance()
    {
        //C++11 way of creating a thread-safe lazy instantiation for singletons
        static Logging singleObject;
        return singleObject;
    }
    
    
};
#endif /* defined(__UGLEngine__Logging__) */
