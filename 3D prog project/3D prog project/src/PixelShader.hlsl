texture2D testTexture : register(t0);
SamplerState testSampler;

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
	float3 pad5;
};

cbuffer constantBuffer : register(b0)
{
	Light light;
};

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 color : COLOR;
	float3 normal : NORMAL;
	float4 worldPos : WORLDPOS;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	//Phong!! 
	input.normal = normalize(input.normal);	

	float3 pixelToLight = normalize(light.pos - input.worldPos);
	float3 diffuse = light.color * (max(0, dot(input.normal, pixelToLight)));

	float3 view = normalize(light.camPos - input.worldPos);
	float3 reflection = normalize(reflect(-pixelToLight, input.normal));
	float3 specular = light.color * pow(max(0, dot(reflection, view)), light.specPower);

	float3 ambient = light.color *light.ambient;
	float3 textColor = testTexture.Sample(testSampler, input.uv).xyz;

	return float4((textColor * (ambient + diffuse) + specular) , 1.0f);
}

//Ljussättning
//Texturering - Shadows, Bump mapping, cube maps, ect.