struct PS_INPUT {
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	float InstanceAge : INSTANCEAGE;
};

Texture2D gTexture : register(t0);
SamplerState gSamplerState : register(s0);

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input ) : SV_Target
{
	if ( input.InstanceAge >= 0.f )
	{
		float2 texCoordFromCenter = input.TexCoord.xy - float2( 0.5f, 0.5f );
		float d = saturate( 1.f - distance( texCoordFromCenter, float2(0.f, 0.f) ) * 2.f );
		return float4( 1.f, 0.f, 0.f, d * ( 1.f - input.InstanceAge / 10.f ) );
	}
	return float4( 0.f, 0.f, 0.f, 0.f );	// do nothing if the particle is dead
}
