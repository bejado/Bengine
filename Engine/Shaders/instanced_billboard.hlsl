cbuffer CameraConstants : register(b0)
{
	float4x4 gViewProjection;
	float3 gCameraPosition;
}

struct VS_INPUT {
	float4 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
	float4 InstancePos : INSTANCEPOS;
	float InstanceAge : INSTANCEAGE;
};

struct PS_INPUT {
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	float InstanceAge : INSTANCEAGE;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT output;

	if ( input.InstanceAge < 0.f ) // check if the particle is dead
	{
		output.InstanceAge = input.InstanceAge;
		return output;	// doesn't matter what we return, just let the pixel shader know that the particle is dead
	}

	float3 normalVector = normalize( input.InstancePos - gCameraPosition );
	float3 rightVector = cross( float3( 0.f, 1.f, 0.f ), normalVector );
	float3 upVector = cross( normalVector, rightVector );

	float4x4 rotation = { rightVector.x, upVector.x, normalVector.x, 0.f,
						  rightVector.y, upVector.y, normalVector.y, 0.f,
						  rightVector.z, upVector.z, normalVector.z, 0.f,
						  0.f, 0.f, 0.f, 1.f };
	input.Pos = mul( rotation, input.Pos );
	input.Pos += float4( input.InstancePos.xyz, 0.f );

	output.Pos = mul( gViewProjection, input.Pos );
	output.TexCoord = input.TexCoord;
	output.InstanceAge = input.InstanceAge;

	return output;
}
