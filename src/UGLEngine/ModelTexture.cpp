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

bool ModelTexture::HasTransparency()
{
    return this->hasTransparency;
}

bool ModelTexture::HasForcedUpwardNormals()
{
    return this->forceUpwardNormals;
}

void ModelTexture::SetShineDamper(float damper)
{
    this->shineDamper = damper;
}

void ModelTexture::SetReflectivity(float reflectivity)
{
    this->reflectivity = reflectivity;
}

void ModelTexture::SetTransparency(bool transparent)
{
    this->hasTransparency = transparent;
}

void ModelTexture::SetForcedUpdatedNormals(bool enabled)
{
    this->forceUpwardNormals = enabled;
}
