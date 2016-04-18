cbuffer MyConstants : register(b0)
{
	float4x4 gViewProjection;
}

cbuffer MyConstants : register(b1)
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
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
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
	output.Normal = input.Normal;
	output.TexCoord = input.TexCoord;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input ) : SV_Target
{
	float3 textureColor = gTexture.Sample( gSamplerState, input.TexCoord );
	return float4(textureColor, 1.0f);
}
