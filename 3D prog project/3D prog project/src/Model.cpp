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
		std::cout << "Could open obj file: " << fileName.c_str() << std::endl;

		while (fileIn)
		{
			checkChar = fileIn.get();

			switch (checkChar)
			{
			case '#':
				checkChar = fileIn.get();
				while (checkChar != '\n')
				{
					checkChar = fileIn.get();
				}
				break;

			case 'v':
				checkChar = fileIn.get();
				if (checkChar == ' ')
				{
					float vz, vy, vx;
					fileIn >> vx >> vy >> vz;
					std::cout << "v " << vx << " " << vy << " " << vz << std::endl;

					if (isRHCoordSys)
					{
						vertPos.push_back(dx::XMFLOAT3(vx, vy, vz * -1.0f));
					}
					else
					{
						vertPos.push_back(dx::XMFLOAT3(vx, vy, vz));
					}
				}

				if (checkChar == 't')
				{
					float vtcu, vtcv;
					fileIn >> vtcu >> vtcv;
					std::cout << "vt " << vtcu << " " << vtcv << std::endl;

					if (isRHCoordSys)
					{
						vertTextCoord.push_back(dx::XMFLOAT2(vtcu, 1.0f - vtcv));
					}
					else
					{
						vertTextCoord.push_back(dx::XMFLOAT2(vtcu, vtcv));
					}
					hasTexCoord = true;
				}

				if (checkChar == 'n')
				{
					float vnx, vny, vnz;
					fileIn >> vnx >> vny >> vnz;
					std::cout << "vn " << vnx << " " << vny << " " << vnz << std::endl;


					if (isRHCoordSys)
					{
						vertNorm.push_back(dx::XMFLOAT3(vnx, vny, vnz * -1.0f));
					}
					else
					{
						vertNorm.push_back(dx::XMFLOAT3(vnx, vny, vnz));
					}
					hasNorm = true;
				}
				break;

			case 'g':
				checkChar = fileIn.get();
				if (checkChar == ' ')
				{
					subSetIndexStart.push_back(vIndex);
					subSetIndexStart.push_back(vIndex);
					subSetcount++;
				}
				break;

				//Loading Faces
			case 'f':
				checkChar = fileIn.get();

				if (checkChar == ' ')
				{
					face = L"";
					std::wstring VertDef;    
					triangleCount = 0;

					checkChar = fileIn.get();
					while (checkChar != '\n')
					{
						face += checkChar;            
						checkChar = fileIn.get();    
						if (checkChar == ' ')
						{
							triangleCount++;
						}
					}

					if (face[face.length() - 1] == ' ')
					{
						triangleCount--;
					}

					triangleCount -= 1;

					std::wstringstream ss(face);

					if (face.length() > 0)
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
										wstringToInt >> vertPosIndexTemp;
										vertPosIndexTemp -= 1;

										if (j == VertDef.length() - 1)
										{
											vertNormIndexTemp = 0;
											vertTCIndexTemp = 0;
										}
									}

									else if (whichPart == 1)
									{
										if (vertPart != L"")
										{
											wstringToInt >> vertTCIndexTemp;
											vertTCIndexTemp -= 1;
										}
										else
										{
											vertTCIndexTemp = 0;
										}

										if (j == VertDef.length() - 1)
										{
											vertNormIndexTemp = 0;
										}
									}

									else if (whichPart == 2)
									{
										std::wistringstream wstringToInt(vertPart);
										wstringToInt >> vertNormIndexTemp;
										vertNormIndexTemp -= 1;
									}

									vertPart = L"";
									whichPart++;
								}
							}

							//Storing faces
							if (subSetcount == 0)
							{
								subSetIndexStart.push_back(vIndex);
								subSetcount++;
							}

							bool vertAlreadyExsists = false;

							if (totalVerts >= 3)
							{
								for (int iCheck = 0; iCheck < totalVerts; iCheck++)
								{
									if (vertPosIndexTemp == vertPosIndex[iCheck] && !vertAlreadyExsists)
									{
										if (vertTCIndexTemp == vertTCIndex[iCheck])
										{
											indices.push_back(iCheck);
											vertAlreadyExsists = true;
										}
									}
								}
							}

							if (!vertAlreadyExsists)
							{
								vertPosIndex.push_back(vertPosIndexTemp);
								vertTCIndex.push_back(vertTCIndexTemp);
								vertNormIndex.push_back(vertNormIndexTemp);
								totalVerts++;
								indices.push_back(totalVerts - 1);
							}

							if (i == 0)
							{
								firstVIndex = indices[vIndex];
							}

							if (i == 2)
							{
								lastVIndex = indices[vIndex];
							}
							vIndex++;
						}

						meshTriangles++;

						//Triangulate
						for (int l = 0; l < triangleCount - 1; l++)
						{
							indices.push_back(firstVIndex);
							vIndex++;

							indices.push_back(lastVIndex);
							vIndex++;

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
										wstringToInt >> vertPosIndexTemp;
										vertPosIndexTemp -= 1;

										if (j == VertDef.length() - 1)
										{
											vertTCIndexTemp = 0;
											vertPosIndexTemp = 0;
										}
									}

									else if (whichPart == 1)
									{
										if (vertPart != L"")
										{
											wstringToInt >> vertTCIndexTemp;
											vertTCIndexTemp -= 1;
										}
										else
										{
											vertTCIndexTemp = 0;
										}

										if (j == VertDef.length() - 1)
										{
											vertNormIndexTemp = 0;
										}
									}

									else if (whichPart == 2)
									{
										std::wistringstream wstringToInt(vertPart);

										wstringToInt >> vertNormIndexTemp;
										vertNormIndexTemp -= 1;
									}

									vertPart = L"";
									whichPart++;
								}
							}

							bool vertAlreadyExsist = false;

							if (totalVerts >= 3)
							{
								for (int iCheck = 0; iCheck < totalVerts; iCheck++)
								{
									if (vertPosIndexTemp == vertPosIndex[iCheck] && !vertAlreadyExsist)
									{
										if (vertTCIndexTemp == vertTCIndex[iCheck])
										{
											indices.push_back(iCheck);
											vertAlreadyExsist = true;
										}
									}
								}
							}

							if (!vertAlreadyExsist)
							{
								vertPosIndex.push_back(vertPosIndexTemp);
								vertTCIndex.push_back(vertTCIndexTemp);
								vertNormIndex.push_back(vertNormIndexTemp);

								totalVerts++;
								indices.push_back(totalVerts - 1);
							}

							lastVIndex = indices[vIndex];
							
							meshTriangles++;
							vIndex++;
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
		std::cout << "Couldnt open obj file: " << fileName.c_str() << std::endl;
		return false;
	}

	//AfterLoading Preperation
	subSetIndexStart.push_back(vIndex);

	if (subSetIndexStart[1] == 0)
	{
		subSetIndexStart.erase(subSetIndexStart.begin() + 1);
		subSetcount--;
	}

	if (!hasNorm)
	{
		vertNorm.push_back(dx::XMFLOAT3(0.0f, 0.0f, 0.0f));
	}
	if (!hasTexCoord)
	{
		vertTextCoord.push_back(dx::XMFLOAT2(0.0f, 0.0f));
	}

	fileIn.close();

	CreateVerticies();

	if (computeNormals)
	{
		ComputeNormals();
	}

	return true;
}

void Model::CreateVerticies()
{
	std::cout << "totalt Verticies: " << totalVerts << std::endl;
	std::cout << "totalt triangles: " << triangleCount << std::endl;
	std::cout << "totalt meshtriangles: " << meshTriangles << std::endl;

	//std::cout << "totalt triangles: " << vertNorm[0].x << std::endl;
	
	SimpleVertex tempvert;

	for (int i = 0; i < totalVerts; i++)
	{
		tempvert.pos = vertPos[vertPosIndex[i]];
		tempvert.norm = vertNorm[vertNormIndex[i]];
		tempvert.uv = vertTextCoord[vertTCIndex[i]];
		tempvert.clr = dx::XMFLOAT3(0, 0, 1);
		verticies.push_back(tempvert);
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
	for (int i = 0; i < meshTriangles; i++)
	{
		vecX = verticies[indices[(i * 3)]].pos.x - verticies[indices[(i * 3) + 2]].pos.x;
		vecY = verticies[indices[(i * 3)]].pos.y - verticies[indices[(i * 3) + 2]].pos.y;
		vecZ = verticies[indices[(i * 3)]].pos.z - verticies[indices[(i * 3) + 2]].pos.z;
		edge1 = dx::XMVectorSet(vecX, vecY, vecZ, 0.0f);

		vecX = verticies[indices[(i * 3)+2]].pos.x - verticies[indices[(i * 3) + 1]].pos.x;
		vecY = verticies[indices[(i * 3)+2]].pos.y - verticies[indices[(i * 3) + 1]].pos.y;
		vecZ = verticies[indices[(i * 3)+2]].pos.z - verticies[indices[(i * 3) + 1]].pos.z;
		edge2 = dx::XMVectorSet(vecX, vecY, vecZ, 0.0f);

		dx::XMStoreFloat3(&unnormolized, dx::XMVector3Cross(edge1, edge2));
		tempNormal.push_back(unnormolized);
	}

	//Computing Vertex Normals
	dx::XMVECTOR normalSum = dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	int faceUsing = 0;
	float tX, tY, tZ;

	for (int i = 0; i < totalVerts; i++)
	{	
		for (int j = 0; j < meshTriangles; j++)
		{
			if (indices[j * 3] == i || indices[(j * 3) + 1] || indices[(j * 3) + 2] == i)
			{
				tX = dx::XMVectorGetX(normalSum) + tempNormal[j].x;
				tY = dx::XMVectorGetY(normalSum) + tempNormal[j].y;
				tZ = dx::XMVectorGetZ(normalSum) + tempNormal[j].z;

				normalSum = dx::XMVectorSet(tX, tY, tZ, 0.0f);
				faceUsing++;
			}
		}

		//normalSum = normalSum / (faceUsing);

		normalSum = dx::XMVector3Normalize(normalSum);

		dx::XMStoreFloat3(&verticies[i].norm, normalSum);
		normalSum = dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		faceUsing = 0;
	}
}
