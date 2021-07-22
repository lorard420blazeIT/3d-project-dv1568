#pragma once
#include "pch.h"
#include "Camera.h"
#include "structs.h"

void UpdateBuffer(ID3D11Buffer*& cbPerObj, ID3D11Buffer*& cbLight,  ID3D11DeviceContext* immediateConxtex, cbFrameObj* frameBuffer, float &offsetX, Light *lightBuffer, Camera &cam);
bool SetupPipeline(ID3D11Device* device, ID3D11Buffer*& vertexBuffer, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, ID3D11InputLayout*& inputLayer,
	ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& textureRSV, ID3D11SamplerState*& sampler, std::string filePath, ID3D11Buffer*& constantBufferObj, ID3D11Buffer*& constantBufferLight, ID3D11Buffer*& indexBuffer, ID3D11DeviceContext*& immidateContext);