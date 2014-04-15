//
//  OBJLoader.h
//  UGLEngine
//
//  Created by Jared Jones on 4/15/14.
//  Copyright (c) 2014 Jared Jones. All rights reserved.
//

#ifndef __UGLEngine__OBJLoader__
#define __UGLEngine__OBJLoader__

#include <glm/glm.hpp>
#include <vector>
#include <iostream>

typedef std::vector <glm::vec3> vec3Storage;
typedef std::vector <glm::vec2> vec2Storage;

bool loadOBJ(const char *path, vec3Storage &out_verticies, vec2Storage &out_uvs, vec3Storage &out_normals);

#endif /* defined(__UGLEngine__OBJLoader__) */
