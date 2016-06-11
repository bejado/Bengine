struct VS_INPUT {
	float4 Pos : POSITION;
	float2 TexCoord : TEXCOORD0;
};

struct PS_INPUT {
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
};

Texture2D gBasePassTexture : register(t1);
SamplerState gBasePassSamplerState : register(s1);

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT output;
	output.Pos = input.Pos;
	output.TexCoord = input.TexCoord;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input ) : SV_Target
{
	float3 theSample = gBasePassTexture.Sample( gBasePassSamplerState, input.TexCoord );
	return float4( theSample / 512.f, 1.f );
}

float4 DepthOnly( PS_INPUT input ) : SV_Target
{
	return float4( 1.f, 1.f, 1.f, 1.f );
}
