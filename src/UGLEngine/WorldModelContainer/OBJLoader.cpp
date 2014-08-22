//
//  OBJLoader.cpp
//  UGLEngine
//
//  Created by Jared Jones on 4/15/14.
//  Copyright (c) 2014 Jared Jones. All rights reserved.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "OBJLoader.h"


bool loadOBJ(const char *path, vec3Storage &out_verticies, vec2Storage &out_uvs, vec3Storage &out_normals, bool &hasQuads)
{
    std::vector<unsigned int> vIndices, uvIndices, normalIndices;
    vec3Storage tmpVerts;
    vec2Storage tmpUvs;
    vec3Storage tmpNorms;
	int dummyTextureCoordVectorIndex = -1; //this will be -1 until one is created, if one is created.
    
    std::ifstream tmpFile;
    if (!tmpFile.good())
    {
        printf("The OBJ file does not exist!\n");
        return false;
    }
    
    tmpFile.open(path, std::ios::in);
    if (!tmpFile.is_open())
    {
        printf("The OBJ file exists but failed to open!\n");
        return false;
    }
    
    std::string line;
	int lineCounter = 0;
    while (std::getline(tmpFile, line))
    {
		lineCounter++; //most text editors count lines starting at one
        if (line[0] == '#')
            continue;
        
        std::istringstream lStream(line);
        
        std::string word;
        lStream >> word;
        
        if (word == "v")
        {
            float x, y, z;
            lStream >> x >> y >> z;
            tmpVerts.push_back(glm::vec3(x, y, z));
        }
        
        else if (word == "vt")
        {
            float u,v;
            lStream >> u >> v;
            tmpUvs.push_back(glm::vec2(u, v));
        }
        
		else if (word == "vn")
        {
            float x, y, z;
            lStream >> x >> y >> z;
            tmpNorms.push_back(glm::vec3(x, y, z));
        }
        
		else if (word == "f")
        {
			std::vector<unsigned int> fvIndices, fvtIndices, fvnIndices; //face data
			bool abortLine = false;
			while (lStream.good()){ //load from string into above arrays
				std::string triad;
				lStream >> triad;

				int indices[3] = { -1, -1, -1 };

				int mark = 0;
				int current = 0;
				//std::cout << triad << ' '; //COUT
				for (int i = 0; i < triad.length();i++){
					if (triad[i] == '/'){
						if (mark != i){
							indices[current] = atoi(triad.substr(mark, (i-mark)).c_str());
							//std::cout << triad.substr(mark, (i - mark)) << ' '; //COUT
						}
						mark = i+1;
						current++;
					}
				}
				indices[current] = atoi(triad.substr(mark, (triad.length())-mark).c_str()); //get last element
				//std::cout << triad.substr(mark, (triad.length()) - mark) << std::endl; //COUT

				if (indices[0] != -1) fvIndices.push_back(indices[0]);
				if (indices[1] != -1) fvtIndices.push_back(indices[1]);
				if (indices[2] != -1) fvnIndices.push_back(indices[2]);

				//std::cout << "Indices: " << indices[0] << ' ' << indices[1] << ' ' << indices[2] << ' ' << std::endl; //COUT
				
			}
			if (abortLine) continue;
			

			if ((fvtIndices.size() > 0 && fvtIndices.size() != fvIndices.size()) ||
				(fvnIndices.size() > 0 && fvnIndices.size() != fvIndices.size()) ){
				std::cout << "Face with malformed vertices on line " << lineCounter << " in " << path << ": " << line << std::endl;
				continue;
			}

			
			if (fvIndices.size() < 3){
				std::cout << "Face with less than 3 verticies on line " << lineCounter << " in " << path << ": " << line << std::endl;
				continue;
			}
			else if (fvIndices.size() >  3){ //we have an n-gon
				std::vector<unsigned int> newfvIndices;
				std::vector<unsigned int> newfvtIndices;
				std::vector<unsigned int> newfvnIndices;
				for (int i = 2; i < fvIndices.size(); i++){
					newfvIndices.push_back(fvIndices[0]);
					newfvIndices.push_back(fvIndices[i - 1]);
					newfvIndices.push_back(fvIndices[i]);
				}
				if (fvtIndices.size() > 3) for(int i = 2; i < fvtIndices.size(); i++){
					newfvtIndices.push_back(fvtIndices[0]);
					newfvtIndices.push_back(fvtIndices[i - 1]);
					newfvtIndices.push_back(fvtIndices[i]);
				}
				if (fvnIndices.size() > 3)for (int i = 2; i < fvnIndices.size(); i++){
					newfvnIndices.push_back(fvnIndices[0]);
					newfvnIndices.push_back(fvnIndices[i - 1]);
					newfvnIndices.push_back(fvnIndices[i]);
				}
				fvIndices = newfvIndices;
			}
			if (fvtIndices.size() == 0){ //no texture coords were provided
				if (dummyTextureCoordVectorIndex == -1){
					tmpUvs.push_back(glm::vec2(1, 1));
					dummyTextureCoordVectorIndex = tmpUvs.size(); //remember, all indexes are +1
				}
				for (int i = 0; i < fvIndices.size(); i++){
					fvtIndices.push_back(dummyTextureCoordVectorIndex);
				}
			}

			if (fvnIndices.size() == 0){ //no normals were provided
				glm::vec3 calculatedFaceNormal =
					glm::cross(tmpVerts[fvIndices[1] - 1] - tmpVerts[fvIndices[0] - 1],
					tmpVerts[fvIndices[2] - 1] - tmpVerts[fvIndices[0] - 1]);
				tmpNorms.push_back(calculatedFaceNormal);
				int index = tmpNorms.size();
				for (int i = 0; i < fvIndices.size(); i++){
					fvnIndices.push_back(index);
				}
			}
			//push face verts to model
			for (unsigned int i = 0; i < fvIndices.size(); i++){
				vIndices.push_back(fvIndices[i]);
			}
			for (unsigned int i = 0; i < fvtIndices.size(); i++){
				uvIndices.push_back(fvtIndices[i]);
			}
			for (unsigned int i = 0; i < fvnIndices.size(); i++){
				normalIndices.push_back(fvnIndices[i]);
			}

        }

		else
		{
			std::cout << "Unsupported wavefront command on line " << lineCounter << " in " << path << ": " << line << std::endl;
		}
        
    }
    if (vIndices.size() >= 65535)
        printf("WARNING: This mesh is over UINT16 in size, rendering issues may arrise if you use VBOIndexing! This warning isn't checking to see if VBOIndexing is used. However if this warning ever fires please contact Jared immediately, as it is likely an appropriate time to fix the weakness in the engine.\n");
	for (unsigned int i = 0; i < vIndices.size(); i++){
		out_verticies.push_back(tmpVerts[vIndices[i] - 1]);
	}
	for (unsigned int i = 0; i < uvIndices.size(); i++){
		out_uvs.push_back(tmpUvs[uvIndices[i] - 1]);
	}
	for (unsigned int i = 0; i < normalIndices.size(); i++){
		out_normals.push_back(tmpNorms[normalIndices[i] - 1]);
    }
    return true;
}