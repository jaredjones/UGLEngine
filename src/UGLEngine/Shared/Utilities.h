//
//  Utilities.h
//  UGLEngine
//
//  Created by Jared Jones on 4/19/15.
//
//

#ifndef UGLEngine_Utilities_h
#define UGLEngine_Utilities_h

#include "Common.h"

// A function that tells OS X based systems to set their relative
// path to their App Bundle Resource Folder.
static void ApplePathSetup()
{
#ifdef __APPLE__
    // This makes relative paths work in C++ in Xcode by changing directory to the Resources folder inside the .app bundle
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
    }
    CFRelease(resourcesURL);
    
    chdir(path);
    printf("Current Path:%s\n", path);
#endif
}

inline uint64 getMSTime()
{
    using namespace std::chrono;
    
    static const uint64 startTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
    
    uint64 presentTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
    
    return presentTime - startTime;
    
}

inline uint64 getMSTimeDiff(uint64 oldMSTime, uint64 newMSTime)
{
    // getMSTime() have limited data range and this is case when it overflow in this tick
    // However this is very unlikely with a 64bit unsigned int
    if (oldMSTime > newMSTime)
        return (0xFFFFFFFFFFFFFFFF - oldMSTime) + newMSTime;
    else
        return newMSTime - oldMSTime;
}

inline uint64 GetMSTimeDiffToNow(uint64 oldMSTime)
{
    return getMSTimeDiff(oldMSTime, getMSTime());
}

#endif