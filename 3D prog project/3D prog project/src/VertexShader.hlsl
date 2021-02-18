cbuffer constantBuffer
{
	float4x4 WVP;	//world view projection
	float4x4 world;
};

struct VertexShaderInput 
{
	float3 position : POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 color : COLOR;
};

struct VertexShaderOutput 
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 color : COLOR;
	float4 worldPos : WORLDPOS;
};

VertexShaderOutput main(VertexShaderInput input) 
{
	VertexShaderOutput output;
	output.position = mul(float4(input.position, 1.0f), WVP);
	output.uv = input.uv;
	output.color = input.color;
	output.normal = mul(float4(input.normal, 0.0f), world);
	output.worldPos = mul(float4(input.position, 1.0f), world);

	return output;
}