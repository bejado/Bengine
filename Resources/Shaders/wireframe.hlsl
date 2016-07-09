cbuffer CameraConstants : register(b0)
{
	float4x4 gViewProjection;
	float3 gCameraPosition;
}

cbuffer ObjectConstants : register(b1)
{
	float4x4 gObjectToWorld;
	float4x4 gWorldToObject;
}

struct VS_INPUT {
	float4 Pos : POSITION;
};

struct PS_INPUT {
	float4 Pos : SV_POSITION;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT output;

	// Transform vertex into clip space.
	float4 worldPos = mul( gObjectToWorld, input.Pos );
	output.Pos = mul( gViewProjection, worldPos );

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input ) : SV_Target
{
	return float4( 0.f, 1.f, 0.f, 1.f );
}

float4 DepthOnly( PS_INPUT input ) : SV_Target
{
	return float4( 1.f, 1.f, 1.f, 1.f );
}