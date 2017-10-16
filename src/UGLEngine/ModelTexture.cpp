//
//  ModelTexture.cpp
//  UGLEngine
//
//  Created by Jared Jones on 1/16/17.
//
//

#include "ModelTexture.h"

float ModelTexture::GetShineDamper()
{
    return this->shineDamper;
}

float ModelTexture::GetReflectivity()
{
    return this->reflectivity;
}

void ModelTexture::SetShineDamper(float damper)
{
    this->shineDamper = damper;
}

void ModelTexture::SetReflectivity(float reflectivity)
{
    this->reflectivity = reflectivity;
}
