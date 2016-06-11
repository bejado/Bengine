cbuffer CameraConstants : register(b0)
{
	float4x4 gViewProjection;
	float3 gCameraPosition;
	float padding;
	float4x4 gLightMatrix;
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
	float3 WorldNormal : NORMAL;
	float2 TexCoord : TEXCOORD0;
	float4 WorldPos : TEXCOORD1;
	float4 LightPos : TEXCOORD2;
};

Texture2D gShadowMap : register(t1);
SamplerState gShadowMapSamplerState : register(s1);

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT output;
	output.WorldPos = mul( gObjectToWorld, input.Pos );
	output.LightPos = mul( gLightMatrix, output.WorldPos );
	output.Pos = mul( gViewProjection, output.WorldPos );
	output.WorldNormal = mul( gObjectToWorld, input.Normal );
	output.TexCoord = input.TexCoord;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input ) : SV_Target
{
	// Shadow calculation
	float3 projectedCoords = input.LightPos.xyz / input.LightPos.w;
	float currentDepth = projectedCoords.z;

	float shadow = 0.0f;
	float bias = 0.0005;
	float smWidth, smHeight;
	gShadowMap.GetDimensions( smWidth, smHeight );
	float2 texelSize = float2( 1.f, 1.f ) / float2( smWidth, smHeight );

	int samples = 1;
	if ( projectedCoords.z <= 1.0 && projectedCoords.x >= -1.f && projectedCoords.x <= 1.f &&
									 projectedCoords.y >= -1.f && projectedCoords.y <= 1.f )		// check to make sure the coords are inside the light's clip space
	{
		// adjust the projected coords so we can use them for uv lookups
		projectedCoords.xy = projectedCoords.xy * .5 + .5;

		for ( int x = -samples; x <= samples; x++ )
		{
			for ( int y = -samples; y <= samples; y++ )
			{
				float2 offset = float2( x, y ) * texelSize;
				float pcfDepth = gShadowMap.Sample( gShadowMapSamplerState, float2(projectedCoords.x, 1.0 - projectedCoords.y) + offset );
				shadow += currentDepth - bias > pcfDepth ? 1.f : 0.f;
			}
		}
	}

	shadow /= pow( ( samples * 2 + 1 ), 2 );
	float shadowColor = ( 1.f - shadow );

	return float4( shadowColor, shadowColor, shadowColor, 1.f );
}

float4 DepthOnly( PS_INPUT input ) : SV_Target
{
	return float4( 0.f, 1.f, 0.f, 1.f );
}
