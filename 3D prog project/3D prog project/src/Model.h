#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <fstream>
#include <istream>
#include <iostream>
#include <string>

namespace dx = DirectX;


class Model
{
private:
	//Inparameter??
	std::vector<int> subSetIndexStart;
	std::vector<int> subsetMaterialArray;
	int subSetcount;
	bool isRHCoordSystem;
	bool computeNormals;

	std::wstring filename;
	std::vector<unsigned int> indices;
	std::vector<dx::XMFLOAT3> vertPos;
	std::vector<dx::XMFLOAT3> vertNorm;
	std::vector<dx::XMFLOAT2> vertTextCoord;
	std::vector<std::string> meshMaterials;

	std::vector<int> vertPosIndex;
	std::vector<int> vertNormIndex;
	std::vector<int> vertTCIndex;
	
	bool hasTexCoord = false;
	bool hasNorm = false;

	int vertPosIndexTemp;
	int vertNormIndexTemp;
	int vertTCIndexTemp;

	wchar_t	checkChar;
	std::wstring face;
	int vIndex = 0;
	int triangleCount = 0;
	int totatlVerts = 0;
	int meshTriangles = 0;

public:
	Model();
	~Model();
	void LoadOBJ();

};