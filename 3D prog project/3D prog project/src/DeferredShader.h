#pragma once


#include <d3d11.h>
#include <fstream>
#include "structs.h"
using namespace std;

class DeferredShaderClass
{
private:
	cbFrameObj shaders;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* lauout;
	ID3D11SamplerState* samplerStateWrap;
	ID3D11Buffer* matrixBuffer;
	bool initilaizeShader(ID3D11Device* HMWND, WCHAR* vsFilename, WCHAR*psFileName);
	void Release();
	

public:



};