cbuffer constantBuffer
{
	float4x4 world;
	float4x4 view;
	float4x4 projection;
};

struct LightVertexShaderInput 
{
	float4 position : POSITION;
	float2 uv: UV;
};

struct LightVertexShaderOutput 
{
	float4 position : POSITION;
	float2 uv: UV;
};

LightVertexShaderOutput main(LightVertexShaderInput input) 
{
	LightVertexShaderOutput output;
	float WVP = mul(world, view);
	WVP = mul(WVP, projection);

	input.position.w = 1.0f;

	output.position = mul(input.position, WVP);
	output.uv = input.uv;

	return output;
}
