#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <fstream>
#include <istream>
#include <iostream>
#include <string>
#include <sstream>
#include "structs.h"
#include "stb_image.h"
#include <comdef.h>


namespace dx = DirectX;

class Model
{
private:
	/*ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;*/

	//Inparameter??
	std::vector<int> subSetIndexStart;
	std::vector<int> subsetMaterialArray;
	std::wstring meshMathLib;

	int subSetcount = 0;

	//"globlas" from tutorial
	int meshSubsets = 0;
	std::vector<int> meshSubsetIndexStart;
	std::vector<int> meshSubsetTexture;
	std::vector<std::wstring> textureNameArray;

	//To store models information
	std::vector<unsigned int> indices;
	std::vector<dx::XMFLOAT3> vertPos;
	std::vector<dx::XMFLOAT3> vertNorm;
	std::vector<dx::XMFLOAT2> vertTextCoord;
	std::vector<std::wstring> meshMaterials;
	std::vector<SurfaceMaterial> material;

	//Vertex definition indicies
	std::vector<int> vertPosIndex;
	std::vector<int> vertNormIndex;
	std::vector<int> vertTCIndex;

	std::vector<SimpleVertex> verticies;
	
	bool hasTexCoord = false;
	bool hasNorm = false;

	//Temp variables to store into vectors
	std::wstring meshMaterialsTemp;
	int vertPosIndexTemp;
	int vertNormIndexTemp;
	int vertTCIndexTemp;

	wchar_t	checkChar;
	std::wstring face;
	int vIndex = 0;
	int triangleCount = 0;
	int totalVerts = 0;
	int meshTriangles = 0;

	//Texture
	int textureWidth = 0;
	int textureHeight = 0;
	int channels = sizeof(textureWidth);
	std::wstring texturePath;


public:
	Model();
	~Model();
	bool LoadOBJ(std::wstring fileName, bool isRHCoordSys, bool computeNormals);
	void CreateVerticies();
	void ComputeNormals();
	void CreateTexture(std::wstring& fileNamePath);
	int getMeshTriangles() const { return this->meshTriangles; }
	int getTotaltVerts() const { return this->totalVerts; }
	std::vector<unsigned int> &getIndicies() { return this->indices; }
	std::vector<SimpleVertex> &getVerticies() { return this->verticies; }
	int getNrofIndices() const { return this->indices.size(); }
	int getSubSetCount() const { return this->subSetcount;  }
	std::vector<int> getSubsetsIndexStart() const { return this->subSetIndexStart; }
	std::vector<SurfaceMaterial>& getMaterials() { return this->material; }
	std::string getTexturePath() { _bstr_t filePath(texturePath.c_str()); return filePath; }
};

struct constantBufferMatrixes {

};