//
//  Terrain.cpp
//  uglengine
//
//  Created by Jared Jones on 11/5/17.
//

#include "Terrain.h"

float Terrain::GetX()
{
    return this->x;
}

float Terrain::GetZ()
{
    return this->z;
}

RawModel * Terrain::GetModel()
{
    return this->model;
}

ModelTexture * Terrain::GetTexture()
{
    return this->texture;
}
