struct PS_INPUT {
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
};

Texture2D gTexture : register(t0);
SamplerState gSamplerState : register(s0);

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input ) : SV_Target
{
	float3 textureColor = gTexture.Sample( gSamplerState, float2(input.TexCoord.x, input.TexCoord.y) );
	return float4( textureColor, 1.f );
}
