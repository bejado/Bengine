cbuffer CameraConstants : register(b0)
{
	float4x4 gViewProjection;
	float3 gCameraPosition;
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
};

Texture2D gTexture : register(t0);
SamplerState gSamplerState : register(s0);

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT output;
	output.WorldPos = mul( gObjectToWorld, input.Pos );
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
	float3 lightWorldPosition = float3(0.f, 10.f, 0.f);
	float3 ambient = float3(.15f, .15f, .15f);

	float3 textureColor = gTexture.Sample( gSamplerState, float2(input.TexCoord.x, 1.f - input.TexCoord.y) );
	float3 light = normalize( lightWorldPosition - input.WorldPos );
	float3 view = normalize( gCameraPosition - input.WorldPos );
	float3 normal = normalize( input.WorldNormal );
	float3 reflected = -1 * normalize( reflect( light, normal ) );

	float3 phong = ambient;

	// Diffuse
	float diffuse = saturate( dot( light, input.WorldNormal ) );
	if (diffuse > 0.f) {
		phong += diffuse * float3(1.0f, 1.0f, 1.0f);
	}

	// Specular
	float specular = pow( saturate( dot( reflected, view ) ), 1.f );
	if (specular > 0.f) {
		phong += specular * float3(1.0f, 1.0f, 1.0f);
	}

	return float4( textureColor * phong, 1.0f );
}
