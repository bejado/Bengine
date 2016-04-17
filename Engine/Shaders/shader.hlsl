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
};

struct PS_INPUT {
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT output;
	output.Pos = mul( gViewProjection, mul( gObjectToWorld, input.Pos ) );
	output.Normal = input.Normal;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input ) : SV_Target
{
	return float4(input.Normal, 1.0f);
}
