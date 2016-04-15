cbuffer MyConstants : register(b0)
{
	float4x4 gViewProjection;
}

cbuffer MyConstants : register(b1)
{
	float4x4 gObjectToWorld;
}

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
float4 VS( float4 Pos : POSITION ) : SV_POSITION
{
	return mul( gViewProjection, Pos );
	// return mul( gViewProjection, mul( gObjectToWorld, Pos ) );
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( float4 Pos : SV_POSITION ) : SV_Target
{
    return float4( 1.0f, 1.0f, 0.0f, 1.0f );    // Yellow, with Alpha = 1
}
