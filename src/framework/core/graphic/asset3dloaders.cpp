/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential

*  Brief       : Various model loaders
*******************************************************************************/

#include "precompiled.h"
#include "core/graphic/asset3dloaders.h"

namespace Framework
{
    bool Asset3DLoaders::LoadOBJ(Asset3D &aAsset, const string &aFileName)
    {
        vector<glm::vec3> lVertices;
        vector<glm::vec3> lNormals;
        vector<glm::vec2> lUVCoords;
        map<string, Material>::iterator it;
        bool lResult = true;
        uint32_t i;
        uint32_t lNumFaces = 0;
        vector<bool> positionSet;

        map<string, vector<uint32_t> > indices;
        map<string, Material> lMaterials;
        map<string, Texture> lTextures;

        vector<uint32_t> *activeIndices = NULL;

        activeIndices = &indices["Default"];

        /* Open the geometry file */
        FILE *lFile = fopen(aFileName.c_str(), "r");
        if (lFile == NULL)
        {
            CRASH("ERROR cannot open file %s\n", aFileName.c_str());
            return false;
        }

        for (;;) 
        {
            char lLine[512];
            int lRes = fscanf(lFile, "%[^\r\n]\r\n", lLine);
            if (lRes == EOF)
            {
                break;
            }

            /* Load material */
            if (strncmp(lLine, "mtllib ", 7) == 0)
            {
                string lMatFile = lLine + 7;
                string lMatFilePath = Utils::GetFilePath(aFileName);
                LoadMTL(lMatFilePath, lMatFile, lMaterials, lTextures);
            }

            /* Vertices */
            if (lLine[0] == 'v' && lLine[1] == ' ') {
                glm::vec3 vertex;
                lRes = sscanf(lLine + 2, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
                if (lRes != 3)
                {
                    CRASH("ERROR reading v format from OBJ file\n");
                }
                lVertices.push_back(vertex);
            }
            /* Normals */
            else if (lLine[0] == 'v' && lLine[1] == 'n' && lLine[2] == ' ') 
            {
                glm::vec3 normal;
                lRes = sscanf(lLine + 3, "%f %f %f", &normal.x, &normal.y, &normal.z);
                if (lRes != 3)
                {
                    CRASH("ERROR reading vn format from OBJ file\n");
                }
                lNormals.push_back(normal);
            }
            /* Texture coordinates */
            else if (lLine[0] == 'v' && lLine[1] == 't' && lLine[2] == ' ') 
            {
                glm::vec2 uv;
                lRes = sscanf(lLine + 3, "%f %f", &uv.x, &uv.y);
                if (lRes != 2)
                {
                    CRASH("ERROR reading vt format from OBJ file\n");
                }
                /* Adjust uv.y as OBJ defines (0,0) to be top-left corner while
                * OpenGL uses (0,0) for bottom left corner. No idea what DirectX
                * interpretation is. For now just fix it for OpenGL */
                uv.y = 1.0f - uv.y;
                lUVCoords.push_back(uv);
            }
        }

        /* Rewind the file */
        fseek(lFile, SEEK_SET, 0);

        /* Allocate size for the final data */
        aAsset.mVertexData.resize(lVertices.size());
        positionSet.resize(lVertices.size(), false);

        /* Now parse the groups and the faces */
        for (;;)
        {
            char lLine[512];
            int lRes = fscanf(lFile, "%[^\r\n]\r\n", lLine);
            if (lRes == EOF) 
            {
                break;
            }

            /* Material group */
            if (strncmp(lLine, "usemtl ", 7) == 0) 
            {
                string lMaterialName = lLine + 7;
                if (lMaterials.find(lMaterialName) == lMaterials.end()) 
                {
                    CRASH("ERROR referenced material %s not found in material list\n", lMaterialName.c_str());
                    lResult = false;
                    break;
                }

                activeIndices = &indices[lMaterialName];
            }

            /* Faces */
            if (lLine[0] == 'f' && lLine[1] == ' ')
            {
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = sscanf(lLine + 2, "%d/%d/%d %d/%d/%d %d/%d/%d", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1],
                    &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                if (matches != 9)
                {
                    CRASH("ERROR OBJ file format is not correct for this loader %d\n", matches);
                    lResult = false;
                    goto error_exit;
                }

                /* Fill the indices for each triangle */
                for (i = 0; i < 3; ++i)
                {
                    uint32_t vertexIdx = vertexIndex[i] - 1;
                    uint32_t normalIdx = normalIndex[i] - 1;
                    uint32_t uvIdx = uvIndex[i] - 1;
                    uint32_t dataIdx = vertexIdx;

                    if (positionSet[dataIdx] == true) {
                        if (aAsset.mVertexData[dataIdx].mNormal != lNormals[normalIdx] || aAsset.mVertexData[dataIdx].mUVCoord != lUVCoords[uvIdx]) {
                            dataIdx = static_cast<uint32_t>(aAsset.mVertexData.size());
                            aAsset.mVertexData.resize(aAsset.mVertexData.size() + 1);
                            positionSet.resize(positionSet.size() + 1, false);
                        }
                    }

                    if (positionSet[dataIdx] == false) 
                    {
                        aAsset.mVertexData[dataIdx].mVertex = lVertices[vertexIdx];
                        aAsset.mVertexData[dataIdx].mNormal = lNormals[normalIdx];
                        aAsset.mVertexData[dataIdx].mUVCoord = lUVCoords[uvIdx];

                        positionSet[vertexIdx] = true;
                    }

                    /* Push the index to the geometry group and
                    * the data to the global data buffer */
                    activeIndices->push_back(dataIdx);
                }

                lNumFaces++;
            }
        }

        /* Now consolidate the data */
        for (it = lMaterials.begin(); it != lMaterials.end(); ++it)
        {
            aAsset.mMaterials.push_back(it->second);
            aAsset.mTextures.push_back( std::move(lTextures[it->first]) );

            vector<uint32_t> *idx = &indices[it->first];

            aAsset.mIndicesOffsets.push_back( static_cast<uint32_t>(aAsset.mVertexIndices.size()) );

            /* Append to the final indices vector */
            aAsset.mVertexIndices.reserve(aAsset.mVertexIndices.size() + idx->size());
            aAsset.mVertexIndices.insert(aAsset.mVertexIndices.end(), idx->begin(), idx->end());

            aAsset.mIndicesCount.push_back( static_cast<uint32_t>(idx->size()) );
        }

        /* And finally normalize the lVertices */
        aAsset.Normalize();

        INFO(LogLevel::eLEVEL2, "Loaded %s with %zu lVertices and %zu faces\n", aFileName.c_str(), aAsset.mVertexData.size(), aAsset.mVertexIndices.size() / 3);

    error_exit:
        fclose(lFile);
        return lResult;
    }


    bool Asset3DLoaders::LoadMTL(const string &aBasePath, const string &aFileName, map<string, Material>& aMaterials, map<string, Texture>& aTextures)
    {
        int lResult = true;
        
        ///* Open the materials file */
        string lMaterialFile = aBasePath + Utils::GetPathSeparator() + aFileName;
        FILE *lFile = fopen(lMaterialFile.c_str(), "r");
        if (lFile == NULL)
        {
            CRASH("ERROR cannot open material file %s\n", lMaterialFile.c_str());
            return false;
        }

        /* Set the default group in case the OBJ file does not
        * contain materials */
        //indices["Default"] = vector<uint32_t>();
        //materials["Default"] = Material();

        for (;;) 
        {
            char lLine[512];
            lResult = fscanf(lFile, "%[^\r\n]\r\n", lLine);
            if (lResult == EOF)
            {
                break;
            }

            if (strncmp(lLine, "newmtl ", 7) == 0)
            {
                string lMaterialName = lLine + 7;
                aMaterials[lMaterialName] = Material();
                Material lCurrMaterial = aMaterials[lMaterialName];

                glm::vec3 lAmbient, lDiffuse, lSpecular;
                float lShininess, lAlpha;

                /* Read the components */
                for (;;) 
                {
                    int lResult = fscanf(lFile, "%[^\r\n]\r\n", lLine);
                    if (lResult == EOF)
                    {
                        break;
                    }

                    /* Ka - Ambient */
                    if (lLine[0] == 'K' && lLine[1] == 'a')
                    {
                        lResult = sscanf(lLine + 2, "%f %f %f", &lAmbient.r, &lAmbient.g, &lAmbient.b);
                        if (lResult != 3)
                        {
                            lResult = false;
                            CRASH("ERROR reading 'Ka' format from OBJ file\n");
                        }
                        lCurrMaterial.SetAmbient(lAmbient);
                        continue;
                    }
                    /* Kd - Diffuse */
                    if (lLine[0] == 'K' && lLine[1] == 'd')
                    {
                        lResult = sscanf(lLine + 2, "%f %f %f", &lDiffuse.r, &lDiffuse.g, &lDiffuse.b);
                        if (lResult != 3) 
                        {
                            lResult = false;
                            CRASH("ERROR reading 'Kd' format from OBJ file\n");
                        }
                        lCurrMaterial.SetDiffuse(lDiffuse);
                        continue;
                    }
                    /* Ks - Specular */
                    if (lLine[0] == 'K' && lLine[1] == 's')
                    {
                        lResult = sscanf(lLine + 2, "%f %f %f", &lSpecular.r, &lSpecular.g, &lSpecular.b);
                        if (lResult != 3)
                        {
                            lResult = false;
                            CRASH("ERROR reading 'Ks' format from OBJ file\n");
                        }
                        lCurrMaterial.SetSpecular(lSpecular);
                        continue;
                    }
                    /* Ns - Shininess*/
                    if (lLine[0] == 'N' && lLine[1] == 's')
                    {
                        lResult = sscanf(lLine + 2, "%f", &lShininess);
                        if (lResult != 1)
                        {
                            CRASH("ERROR reading 'Ns' format from OBJ file\n");
                        }
                        lCurrMaterial.SetShininess(lShininess);
                        continue;
                    }

                    /* Dissolve - Alpha*/ 
                    if (lLine[0] == 'd')
                    {
                        lResult = sscanf(lLine + 1, "%f", &lAlpha);
                        if (lResult != 1)
                        {
                            CRASH("ERROR reading 'd' format from OBJ file\n");
                        }
                        continue;
                    }

                    /* map_Kd - Diffuse texture*/
                    if (strncmp(lLine, "map_Kd ", 7) == 0)
                    {
                        Texture lTexture;
                        string lTextureFile;
                        
                        if (strcmp(lLine + 7, "null") != 0)
                        {
                            if (Utils::GetFilePath(string(lLine + 7)).empty())
                                lTextureFile.assign(aBasePath + Utils::GetPathSeparator() + string(lLine + 7));
                            else
                                lTextureFile.assign(string(lLine + 7));

                            /* Load the texture */
                            if ( !lTexture.Load(lTextureFile) )
                            {
                                CRASH("ERROR loading PNG texture %s\n", lTextureFile.c_str());
                                lResult = false;
                                goto error_exit;
                            }
                        }
                        /* Add the new material */
                        //indices[lMaterialName] = vector<uint32_t>();
                        aMaterials[lMaterialName] = Material(lAmbient, lDiffuse, lSpecular, lAlpha, lShininess);
                        aTextures[lMaterialName] = std::move(lTexture);

                        break;
                    }
                } /* Material components read */
            }
        }

        fclose(lFile);

    error_exit:
        fclose(lFile);
        return lResult;
    }
}
