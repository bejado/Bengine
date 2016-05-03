cbuffer CameraConstants : register(b0)
{
	float4x4 gViewProjection;
	float3 gCameraPosition;
}

cbuffer ObjectConstants : register(b1)
{
	float4x4 gObjectToWorld;
}

struct VS_INPUT {
	float4 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
};

struct PS_INPUT {
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	float3 TheNormal : TEXCOORD1;
};

Texture2D gTexture : register(t0);
SamplerState gSamplerState : register(s0);

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT output;
	output.Pos = mul( gViewProjection, mul( gObjectToWorld, input.Pos ) );
	output.TexCoord = input.TexCoord;
	output.TheNormal = input.Normal;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input ) : SV_Target
{
	input.TheNormal = normalize( input.TheNormal );
	if ( input.TheNormal.z > -1.001f && input.TheNormal.z < -.999f )
	{
		input.TheNormal = float3(1.f, 1.f, 0.f);
	}
	return float4( abs(input.TheNormal), .3f );
}
