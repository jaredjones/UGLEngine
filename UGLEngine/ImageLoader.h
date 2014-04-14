//
//  BMPLoader.h
//  UGLEngine
//
//  Created by Jared Jones on 4/13/14.
//  Copyright (c) 2014 Jared Jones. All rights reserved.
//

#ifndef __UGLEngine__BMPLoader__
#define __UGLEngine__BMPLoader__

#include <OpenGL/OpenGL.h>
// Load a .BMP file using our custom loader

GLuint loadBMP_custom(const char * imagepath);
GLuint loadDDS(const char * imagepath);

#endif /* defined(__UGLEngine__BMPLoader__) */
