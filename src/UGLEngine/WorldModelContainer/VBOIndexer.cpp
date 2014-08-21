//
//  VBOIndexer.cpp
//  UGLEngine
//
//  Created by Jared Jones on 4/17/14.
//  Copyright (c) 2014 Jared Jones. All rights reserved.
//

#include <glm.hpp>
#include <map>
#include "VBOIndexer.h"

//using namespace VBOIndexer;

//Function that returns true iff the vertices are considered equal.
bool isNear(float v1, float v2)
{
    return fabs(v1-v2) < 0.01f;
}

// Searches through all already-exported vertices
// for a similar one.
// Similar = same position + same UVs + same normal
bool getSimilarVertexIndex(
                           glm::vec3 &vertex,
                           glm::vec2 &uv,
                           glm::vec3 &normal,
                           vec3Storage &outVertices,
                           vec2Storage &outUvs,
                           vec3Storage &outNormals,
                           unsigned short & result
                           ){
	// Lame linear search
	for ( unsigned int i=0; i < outVertices.size(); i++ )
    {
		if (
			isNear( vertex.x , outVertices[i].x ) &&
			isNear( vertex.y , outVertices[i].y ) &&
			isNear( vertex.z , outVertices[i].z ) &&
			isNear( uv.x     , outUvs     [i].x ) &&
			isNear( uv.y     , outUvs     [i].y ) &&
			isNear( normal.x , outNormals [i].x ) &&
			isNear( normal.y , outNormals [i].y ) &&
			isNear( normal.z , outNormals [i].z )
            ){
			result = i;
			return true;
		}
	}
	// No other vertex could be used instead.
	// Looks like we'll have to add it to the VBO.
	return false;
}

bool getSimilarVertexIndexFast(PackedVertex &pVertex, std::map<PackedVertex, unsigned short> &vToOutIndex, unsigned short &result)
{
    std::map<PackedVertex, unsigned short>::iterator it = vToOutIndex.find(pVertex);
    
    if (it == vToOutIndex.end())
        return false;
    else
    {
        result = it->second;
        return true;
    }
}

void indexVBO_slow(vec3Storage &vertices, vec2Storage &uvs, vec3Storage &normals,
                   uShortStorage &outIndices, vec3Storage &outVertices, vec2Storage &outUvs, vec3Storage &outNormals)
{
	// For each input vertex
	for (unsigned int i=0; i < vertices.size(); i++)
    {
		// Try to find a similar vertex in out_XXXX
		unsigned short index;
		bool found = getSimilarVertexIndex(vertices[i], uvs[i], normals[i], outVertices, outUvs, outNormals, index);
        
		if ( found ) // A similar vertex is already in the VBO, use it instead !
			outIndices.push_back(index);
		else
        { // If not, it needs to be added in the output data.
			outVertices.push_back( vertices[i]);
			outUvs     .push_back( uvs[i]);
			outNormals .push_back( normals[i]);
			outIndices .push_back( (unsigned short)outVertices.size() - 1 );
		}
	}
}

void indexVBO(vec3Storage &vertices, vec2Storage &uvs, vec3Storage &normals,
              uShortStorage &outIndices, vec3Storage &outVertices, vec2Storage &outUvs, vec3Storage &outNormals)
{
    std::map<PackedVertex, unsigned short> vertexToOutIndex;
    
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        PackedVertex pVertex = {vertices[i], uvs[i], normals[i]};
        
        unsigned short index;
        bool found = getSimilarVertexIndexFast(pVertex, vertexToOutIndex, index);
        
        if (found)
            outIndices.push_back(index);
        else
        {
            outVertices.push_back(vertices[i]);
            outUvs.push_back(uvs[i]);
            outNormals.push_back(normals[i]);
            unsigned short newIndex = (unsigned short)outVertices.size() - 1;
            outIndices.push_back(newIndex);
            vertexToOutIndex[pVertex] = newIndex;
        }
    }
}

void indexVBOandTB(vec3Storage &vertices, vec2Storage &uvs, vec3Storage &normals, vec3Storage &tangents, vec3Storage &bitangents,
                   uShortStorage &outIndices, vec3Storage &outVertices, vec2Storage &outUvs, vec3Storage &outNormals, vec3Storage &outTangents, vec3Storage &outBItangents)
{
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        unsigned short index;
        
        bool found = getSimilarVertexIndex(vertices[i], uvs[i], normals[i], outVertices, outUvs, outNormals, index);
        
        if (found)
        {
            outIndices.push_back(index);
            
            outTangents[index] += tangents[i];
            outBItangents[index] += bitangents[i];
        }
        else
        {
            outVertices.push_back(vertices[i]);
            outUvs.push_back(uvs[i]);
            outNormals.push_back(normals[i]);
            outTangents.push_back(tangents[i]);
            outBItangents.push_back(bitangents[i]);
            outIndices.push_back((unsigned short)outVertices.size() - 1);
        }
        
    }
}
