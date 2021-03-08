#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
}

bool  Model::LoadOBJ(std::wstring fileName, bool isRHCoordSys, bool computeNormals)
{
	/*this->indexBuffer = indexBuff;
	this->vertexBuffer = vertBuff;*/

	std::wifstream fileIn(fileName.c_str());

	if (fileIn)
	{
		std::wcout << "Opened obj file: " << fileName << std::endl;

		while (fileIn)
		{
			this->checkChar = fileIn.get();

			switch (this->checkChar)
			{
			case '#':
				this->checkChar = fileIn.get();
				while (this->checkChar != '\n')
				{
					this->checkChar = fileIn.get();
				}
				break;

			case 'v':
				this->checkChar = fileIn.get();
				if (this->checkChar == ' ')
				{
					float vz, vy, vx;
					fileIn >> vx >> vy >> vz;
					//std::cout << "v " << vx << " " << vy << " " << vz << std::endl;

					if (isRHCoordSys)
					{
						this->vertPos.push_back(dx::XMFLOAT3(vx, vy, vz * -1.0f));
					}
					else
					{
						this->vertPos.push_back(dx::XMFLOAT3(vx, vy, vz));
					}
				}

				if (this->checkChar == 't')
				{
					float vtcu, vtcv;
					fileIn >> vtcu >> vtcv;
					//std::cout << "vt " << vtcu << " " << vtcv << std::endl;

					if (isRHCoordSys)
					{
						this->vertTextCoord.push_back(dx::XMFLOAT2(vtcu, 1.0f - vtcv));
					}
					else
					{
						this->vertTextCoord.push_back(dx::XMFLOAT2(vtcu, vtcv));
					}
					this->hasTexCoord = true;
				}

				if (this->checkChar == 'n')
				{
					float vnx, vny, vnz;
					fileIn >> vnx >> vny >> vnz;
					//std::cout << "vn " << vnx << " " << vny << " " << vnz << std::endl;


					if (isRHCoordSys)
					{
						this->vertNorm.push_back(dx::XMFLOAT3(vnx, vny, vnz * -1.0f));
					}
					else
					{
						this->vertNorm.push_back(dx::XMFLOAT3(vnx, vny, vnz));
					}
					this->hasNorm = true;
				}
				break;

			case 'g':
				this->checkChar = fileIn.get();
				if (this->checkChar == ' ')
				{
					this->subSetIndexStart.push_back(vIndex);
					this->subSetcount++;
				}
				break;

				//Loading Faces
			case 'f':
				this->checkChar = fileIn.get();

				if (this->checkChar == ' ')
				{
					this->face = L"";
					std::wstring VertDef;    
					this->triangleCount = 0;

					this->checkChar = fileIn.get();
					while (this->checkChar != '\n')
					{
						this->face += this->checkChar;

						//std::wcout << checkChar;
						this->checkChar = fileIn.get();
						if (this->checkChar == ' ')
						{
							this->triangleCount++;
						}
					}

					if (this->face[this->face.length() - 1] == ' ')
					{
						this->triangleCount--;
					}

					this->triangleCount -= 1;

					std::wstringstream ss(this->face);

					if (this->face.length() > 0)
					{
						int firstVIndex, lastVIndex;

						for (int i = 0; i < 3; i++)
						{
							ss >> VertDef;

							std::wstring vertPart;
							int whichPart = 0;

							for (int j = 0; j < VertDef.length(); j++)
							{
								if (VertDef[j] != '/')
								{
									vertPart += VertDef[j];
								}

								if (VertDef[j] == '/' || j == VertDef.length() - 1)
								{
									std::wistringstream wstringToInt(vertPart);

									if (whichPart == 0)
									{
										wstringToInt >> this->vertPosIndexTemp;
										this->vertPosIndexTemp -= 1;

										if (j == VertDef.length() - 1)
										{
											this->vertNormIndexTemp = 0;
											this->vertTCIndexTemp = 0;
										}
									}

									else if (whichPart == 1)
									{
										if (vertPart != L"")
										{
											wstringToInt >> this->vertTCIndexTemp;
											this->vertTCIndexTemp -= 1;
										}
										else
										{
											this->vertTCIndexTemp = 0;
										}

										if (j == VertDef.length() - 1)
										{
											this->vertNormIndexTemp = 0;
										}
									}

									else if (whichPart == 2)
									{
										std::wistringstream wstringToInt(vertPart);
										wstringToInt >> this->vertNormIndexTemp;
										this->vertNormIndexTemp -= 1;
									}

									vertPart = L"";
									whichPart++;
								}
							}

							//Storing faces
							if (this->subSetcount == 0)
							{
								this->subSetIndexStart.push_back(this->vIndex);
								this->subSetcount++;
							}

							this->vertPosIndex.push_back(this->vertPosIndexTemp);
							this->vertTCIndex.push_back(this->vertTCIndexTemp);
							this->vertNormIndex.push_back(this->vertNormIndexTemp);

							this->totalVerts++;
						}

						this->meshTriangles++;

						//Triangulate
						/*for (int l = 0; l < this->triangleCount - 1; l++)
						{
							this->indices.push_back(firstVIndex);
							this->vIndex++;

							this->indices.push_back(lastVIndex);
							this->vIndex++;

							ss >> VertDef;

							std::wstring vertPart;
							int whichPart = 0;

							for (int j = 0; j < VertDef.length(); j++)
							{
								if (VertDef[j] != '/')
								{
									vertPart += VertDef[j];
								}

								if (VertDef[j] == '/' || j == VertDef.length() - 1)
								{
									std::wistringstream wstringToInt(vertPart);

									if (whichPart == 0)
									{
										wstringToInt >> this->vertPosIndexTemp;
										this->vertPosIndexTemp -= 1;

										if (j == VertDef.length() - 1)
										{
											this->vertTCIndexTemp = 0;
											this->vertPosIndexTemp = 0;
										}
									}

									else if (whichPart == 1)
									{
										if (vertPart != L"")
										{
											wstringToInt >> this->vertTCIndexTemp;
											this->vertTCIndexTemp -= 1;
										}
										else
										{
											this->vertTCIndexTemp = 0;
										}

										if (j == VertDef.length() - 1)
										{
											this->vertNormIndexTemp = 0;
										}
									}

									else if (whichPart == 2)
									{
										std::wistringstream wstringToInt(vertPart);

										wstringToInt >> this->vertNormIndexTemp;
										this->vertNormIndexTemp -= 1;
									}

									vertPart = L"";
									whichPart++;
								}
							}

							bool vertAlreadyExsist = false;

							if (this->totalVerts >= 3)
							{
								for (int iCheck = 0; iCheck < this->totalVerts; iCheck++)
								{
									if (this->vertPosIndexTemp == this->vertPosIndex[iCheck] && !vertAlreadyExsist)
									{
										if (this->vertTCIndexTemp == this->vertTCIndex[iCheck])
										{
											this->indices.push_back(iCheck);
											vertAlreadyExsist = true;
										}
									}
								}
							}

							if (!vertAlreadyExsist)
							{
								this->vertPosIndex.push_back(this->vertPosIndexTemp);
								this->vertTCIndex.push_back(this->vertTCIndexTemp);
								this->vertNormIndex.push_back(this->vertNormIndexTemp);

								this->totalVerts++;
								this->indices.push_back(this->totalVerts - 1);
							}

							lastVIndex = this->indices[this->vIndex];
							
							this->meshTriangles++;
							this->vIndex++;
						}*/
					}
				}
				break;

			case 'm' : 
				checkChar = fileIn.get();
				if (checkChar == 't')
				{
					checkChar = fileIn.get();
					if (checkChar == 'l')
					{
						checkChar = fileIn.get();
						if (checkChar == 'l')
						{
							checkChar = fileIn.get();
							if (checkChar == 'i')
							{
								checkChar = fileIn.get();
								if (checkChar == 'b')
								{
									checkChar = fileIn.get();
									if (checkChar == ' ')
									{
										fileIn >> meshMathLib;
									}
								}
							}
						}
					}
				}
				break;

			case 'u' :
				checkChar = fileIn.get();
				if (checkChar == 's')
				{
					checkChar = fileIn.get();
					if (checkChar == 'e')
					{
						checkChar = fileIn.get();
						if (checkChar == 'm')
						{
							checkChar = fileIn.get();
							if (checkChar == 't')
							{
								checkChar = fileIn.get();
								if (checkChar == 'l')
								{
									checkChar = fileIn.get();
									if (checkChar == ' ')
									{
										meshMaterialsTemp = L"";
										
										fileIn >> meshMaterialsTemp;
										meshMaterials.push_back(meshMaterialsTemp);
									}
								}
							}
						}
					}
				}
				break;

			default:
				break;
			}
		}
	}
	else
	{
		std::wcout << "Couldnt open obj file: " << fileName.c_str() << std::endl;
		return false;
	}

	//AfterLoading Preperation
	this->subSetIndexStart.push_back(vIndex);

	if (this->subSetIndexStart[1] == 0)
	{
		this->subSetIndexStart.erase(this->subSetIndexStart.begin() + 1);
		this->subSetcount--;
	}

	if (!this->hasNorm)
	{
		this->vertNorm.push_back(dx::XMFLOAT3(0.0f, 0.0f, 0.0f));
	}
	if (!this->hasTexCoord)
	{
		this->vertTextCoord.push_back(dx::XMFLOAT2(0.0f, 0.0f));
	}

	fileIn.close();
	//Materialet
	meshMathLib = L"../3D Models/" + meshMathLib;
	std::wcout << "Mtl filename" << meshMathLib << std::endl;

	fileIn.open(meshMathLib.c_str());


	std::wstring lastStringRead;
	int matCount = material.size();

	bool kdset = false;

	if (fileIn)
	{
		std::wcout << "Opened mtl file: " << meshMathLib << std::endl;

		while (fileIn)
		{
			checkChar = fileIn.get();
			switch (checkChar)
			{

			case '#': //Comment
				checkChar = fileIn.get();
				while (checkChar != '\n')
				{
					checkChar = fileIn.get();
				}
				break;

			case 'K': 
				checkChar = fileIn.get();
				if (checkChar == 'd') //Diffuse
				{
					checkChar = fileIn.get();

					fileIn >> material[matCount - 1].difColor.x;
					fileIn >> material[matCount - 1].difColor.y;
					fileIn >> material[matCount - 1].difColor.z;

					kdset = true;
				}

				if (checkChar == 'a') //Ambient
				{
					checkChar = fileIn.get();
					if (!kdset)
					{
						fileIn >> material[matCount - 1].difColor.x;
						fileIn >> material[matCount - 1].difColor.y;
						fileIn >> material[matCount - 1].difColor.z;

					}
				}
				break;

			case 'm':
				checkChar = fileIn.get();
				if (checkChar == 'a')
				{
					checkChar = fileIn.get();
					if (checkChar == 'p')
					{
						checkChar = fileIn.get();
						if (checkChar == '_')
						{
							checkChar = fileIn.get();
							if (checkChar == 'K')
							{
								checkChar = fileIn.get();
								if (checkChar == 'd')
								{

									fileIn.get();

									bool textFilePathEnd = false;
									while (!textFilePathEnd)
									{
										checkChar = fileIn.get();
										texturePath += checkChar;

										if (checkChar == '.')
										{
											for (int i = 0; i < 3; i++)
											{
												texturePath += fileIn.get();
											}

											textFilePathEnd = true;
										}
									}
									texturePath = L"../3D Models/" + texturePath;
									std::wcout << "model texure: " << texturePath << std::endl;


									//bool alreadyLoaded = false;
									//for (int i = 0; i < textureNameArray.size(); i++)
									//{
									//	if (texturePath == textureNameArray[i])
									//	{
									//		alreadyLoaded = true;
									//		material[matCount - 1].textArrayIndex = i;
									//		material[matCount - 1].hasTexture = true;

									//	}
									//}
									//
									//if (!alreadyLoaded)
									//{
									//CreateTexture(texturePath);
									//	textureNameArray.push_back(texturePath.c_str());
									//}
								}
							}
						}
					}
				}
				break;

			case 'n':
				checkChar = fileIn.get();

				if (checkChar == 'e')
				{
					checkChar = fileIn.get();
					if (checkChar == 'w')
					{
						checkChar = fileIn.get();
						if (checkChar == 'm')
						{
							checkChar = fileIn.get();
							if (checkChar == 't')
							{
								checkChar = fileIn.get();
								if (checkChar == 'l')
								{
									checkChar = fileIn.get();
									if (checkChar == ' ')
									{
										SurfaceMaterial tempMat;
										material.push_back(tempMat);
										fileIn >> material[matCount].matName;
										material[matCount].hasTexture = false;
										material[matCount].textArrayIndex = 0;
										matCount++;
										kdset = false;
									}
								}
							}
						}
					}
				}
				break;

			default:
				break;
			}
		}
	}
	else 
	{
		std::wcout << "Couldnt open mtl file: " << fileName.c_str() << std::endl;
		return false;
	}

	for (int i = 0; i < subSetcount; i++)
	{
		bool hasMat = false;
		for (int j = 0; j < material.size(); j++)
		{
			if (meshMaterials[i] == material[j].matName)
			{
				subsetMaterialArray.push_back(j);
				hasMat = true;
			}
		}
		if (!hasMat) 
		{
			subsetMaterialArray.push_back(0);
		}
	}


	std::wcout << face.c_str() << std::endl;

	CreateVerticies();

	if (computeNormals)
	{
		ComputeNormals();
	}

	std::cout << "triangle Count: "<<triangleCount << std::endl;

	return true;
}

void Model::CreateVerticies()
{
	//std::cout << "totalt Verticies: " << this->totalVerts << std::endl;
	//std::cout << "totalt triangles: " << this->triangleCount << std::endl;
	//std::cout << "totalt meshtriangles: " << this->meshTriangles << std::endl;

	//std::cout << "totalt triangles: " << vertNorm[0].x << std::endl;
	
	// pos/uv/norm

	SimpleVertex tempvert;
	for (int i = 0; i < this->totalVerts; i++)
	{

		tempvert.pos = this->vertPos[vertPosIndex[i]];
		tempvert.uv = this->vertTextCoord[vertTCIndex[i]];
		tempvert.norm = this->vertNorm[vertNormIndex[i]];
		tempvert.clr = dx::XMFLOAT3(0, 0, 1);
		this->verticies.push_back(tempvert);
	}
}

void Model::ComputeNormals()
{
	std::vector<dx::XMFLOAT3> tempNormal;
	dx::XMFLOAT3 unnormolized = dx::XMFLOAT3(0.0f, 0.0f, 0.0f);
	float vecX, vecY, vecZ;

	dx::XMVECTOR edge1 = dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	dx::XMVECTOR edge2 = dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	//Computing face Normals
	for (int i = 0; i < this->meshTriangles; i++)
	{
		vecX = this->verticies[this->indices[(i * 3)]].pos.x - this->verticies[this->indices[(i * 3) + 2]].pos.x;
		vecY = this->verticies[this->indices[(i * 3)]].pos.y - this->verticies[this->indices[(i * 3) + 2]].pos.y;
		vecZ = this->verticies[this->indices[(i * 3)]].pos.z - this->verticies[this->indices[(i * 3) + 2]].pos.z;
		edge1 = dx::XMVectorSet(vecX, vecY, vecZ, 0.0f);

		vecX = this->verticies[this->indices[(i * 3)+2]].pos.x - this->verticies[this->indices[(i * 3) + 1]].pos.x;
		vecY = this->verticies[this->indices[(i * 3)+2]].pos.y - this->verticies[this->indices[(i * 3) + 1]].pos.y;
		vecZ = this->verticies[this->indices[(i * 3)+2]].pos.z - this->verticies[this->indices[(i * 3) + 1]].pos.z;
		edge2 = dx::XMVectorSet(vecX, vecY, vecZ, 0.0f);

		dx::XMStoreFloat3(&unnormolized, dx::XMVector3Cross(edge1, edge2));
		tempNormal.push_back(unnormolized);
	}

	//Computing Vertex Normals
	dx::XMVECTOR normalSum = dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	int faceUsing = 0;
	float tX, tY, tZ;

	for (int i = 0; i < this->totalVerts; i++)
	{	
		for (int j = 0; j < this->meshTriangles; j++)
		{
			if (this->indices[j * 3] == i || this->indices[(j * 3) + 1] || this->indices[(j * 3) + 2] == i)
			{
				tX = dx::XMVectorGetX(normalSum) + tempNormal[j].x;
				tY = dx::XMVectorGetY(normalSum) + tempNormal[j].y;
				tZ = dx::XMVectorGetZ(normalSum) + tempNormal[j].z;

				normalSum = dx::XMVectorSet(tX, tY, tZ, 0.0f);
				faceUsing++;
			}
		}

		//normalSum = normalSum / (faceUsing); //need to fix this

		normalSum = dx::XMVector3Normalize(normalSum);

		dx::XMStoreFloat3(&this->verticies[i].norm, normalSum);
		normalSum = dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		faceUsing = 0;
	}
}

void Model::CreateTexture(std::wstring& fileNamePath)
{
	//CreateTextureFromFile ect ect

	ID3D11ShaderResourceView* tempMeshSRV;
	_bstr_t filePath(fileNamePath.c_str());
	unsigned char* image = stbi_load(filePath, &textureWidth, &textureHeight, &channels, STBI_rgb_alpha);
}

