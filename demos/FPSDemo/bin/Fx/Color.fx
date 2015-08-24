
Texture2D ObjTexture : register(t0);
SamplerState ObjSamplerState : register(s0);

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 vLightDirection;
	float4 vLightColor;
	float4 vOutputColor;
}

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 TextureCoord : TEXCOORD0;
	//float3 Normal : NORMAL;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 TextureCoord : TEXCOORD0;
	//float3 Normal : TEXCOORD0;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
//	output.Normal = mul(float4(input.Normal, 1), World).xyz;
	output.TextureCoord = input.TextureCoord;
	//output.Color = input.Color;

	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
 // input.Normal = normalize(input.Normal);
	//float4 finalColor = 0;

	//do NdotL lighting 
	//finalColor = float4(0.0f, 1.0f, 0.0f, 1.0f);
	//finalColor += saturate(dot((float3)vLightDirection, input.Normal) * vLightColor);
	
	float4 diffuse = ObjTexture.Sample(ObjSamplerState, input.TextureCoord);
	diffuse.a = 1;

  return diffuse;
}
 