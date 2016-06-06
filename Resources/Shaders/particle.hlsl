cbuffer EmitterConstants : register(b1)
{
	float4 startColor;
	float4 endColor;
	float life;
}

struct PS_INPUT {
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	float InstanceAge : INSTANCEAGE;
};

Texture2D gTexture : register(t0);
SamplerState gSamplerState : register(s0);

float calculateOpacity( float2 texCoord, float instanceAge, float lifeCompletion )
{
	float2 texCoordFromCenter = texCoord - float2( 0.5f, 0.5f );
	float d = saturate( 1.f - distance( texCoordFromCenter, float2(0.f, 0.f) ) * 5.f );
	return d * (1.f - lifeCompletion);
	return d;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input ) : SV_Target
{
	if ( input.InstanceAge < 0.f )
	{
		discard;	// do nothing if the particle is dead
	}

	float lifeCompletion = input.InstanceAge / life;
	float3 color = endColor * lifeCompletion + startColor * ( 1.f - lifeCompletion );
	float opacity = calculateOpacity( input.TexCoord.xy, input.InstanceAge, lifeCompletion );

	return float4( color, opacity );
}

float4 DepthOnly( PS_INPUT input ) : SV_Target
{
	if ( input.InstanceAge < 0.f )
	{
		discard;	// do nothing if the particle is dead
	}

	float lifeCompletion = input.InstanceAge / life;
	float opacity = calculateOpacity( input.TexCoord.xy, input.InstanceAge, lifeCompletion );
	if ( opacity < 0.3 )
	{
		discard;
	}

	return float4( 1.f, 0.f, 0.f, 1.f );
}
