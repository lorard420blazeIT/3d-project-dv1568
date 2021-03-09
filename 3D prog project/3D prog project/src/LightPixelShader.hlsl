Texture2D colorTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState SamplerTypePoint : register(s0);

struct Light
{
	float3 pos;
	float pad1;

	float3 ambient;
	float pad2;

	float3 color;
	float pad3;

	float3 camPos;
	float pad4;

	float specPower;
	float attConst;
	float attLin;
	float attQuad;
};

cbuffer constantBuffer : register(b0)
{
	Light light;
};

struct LightPixelShaderInput 
{
	float4 position : SV_POSITION;
	float2 uv : UV;
};

float4 main(LightPixelShaderInput input) : SV_TARGET
{
	float4 colors;
	float4 normals;
	//float3 lightDir; //We have point light no spotlight???
	float lightIntensity;
	float4 outputColor;

	colors = colorTexture.Sample(SamplerTypePoint, input.uv);
	normals = normalTexture.Sample(SamplerTypePoint, input.uv);

	lightIntensity = saturate(normals.xyz);
	outputColor = saturate(colors * lightIntensity);

	return outputColor;
}
