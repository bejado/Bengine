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

Texture2D gTexture : register(t0);
SamplerState gSamplerState : register(s0);

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
	projectedCoords.xy = projectedCoords.xy * .5 + .5;
	float closestDepth = gShadowMap.Sample( gShadowMapSamplerState, float2(projectedCoords.x, 1.0 - projectedCoords.y) );
	float currentDepth = projectedCoords.z;
	float bias = 0.0005;
	float shadow = currentDepth - bias > closestDepth ? 1.f : 0.f;

	float3 textureColor = gTexture.Sample( gSamplerState, float2(input.TexCoord.x, 1.0 - input.TexCoord.y) );
	float3 shadowColor = textureColor * 0.3;
	return float4( (1.f - shadow) * textureColor + (shadow) * shadowColor, 1.f );
	float val = closestDepth;
	return float4(val, val, val, 1.f);
}

float4 DepthOnly( PS_INPUT input ) : SV_Target
{
	return float4( 0.f, 1.f, 0.f, 1.f );
}
