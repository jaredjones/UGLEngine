//
//  Model3D.h
//  UGLEngine
//
//  Created by Jared Jones on 5/3/14.
//  Copyright (c) 2014 Jared Jones. All rights reserved.
//

#ifndef __UGLEngine__Model3D__
#define __UGLEngine__Model3D__

#include <iostream>

class Model3D
{
public:
    Model3D(std::string path, bool vboIndexing = true);
    ~Model3D();
    
    
private:
    //bool _quadBased;
    std::string _path;
};

#endif /* defined(__UGLEngine__Model3D__) */
