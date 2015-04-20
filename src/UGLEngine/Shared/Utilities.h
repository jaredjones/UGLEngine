//
//  Utilities.h
//  UGLEngine
//
//  Created by Jared Jones on 4/19/15.
//
//

#ifndef UGLEngine_Utilities_h
#define UGLEngine_Utilities_h

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

#endif
