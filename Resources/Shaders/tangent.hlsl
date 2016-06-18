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
	float4 Tangent : TANGENT;
};

struct PS_INPUT {
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	float3 TangentLightPos : TEXCOORD2;
	float3 TangentViewPos : TEXCOORD3;
};

Texture2D gTexture : register(t0);
SamplerState gSamplerState : register(s0);

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT output;

	// Pass along texture coordinates.
	output.TexCoord = input.TexCoord;

	float4x4 worldToObject = transpose( gObjectToWorld );	// assuming affine transformation

	// Transform vertex into clip space.
	float4 worldPos = mul( gObjectToWorld, input.Pos );
	output.Pos = mul( gViewProjection, worldPos );

	// Calculate bitangent.
	float3 bitangent = cross(  input.Tangent.xyz, input.Normal ) * input.Tangent.w;	// Tangent.w stores the handedness of tangent space

	// Create the objectToTangent matrix.
	float3x3 objectToTangent = float3x3( input.Tangent.xyz,
										 bitangent,
										 input.Normal );

	// Transform view vector into tangent space.
	float3 view = gCameraPosition - worldPos;
	float3 objectView = mul( worldToObject, view );
	output.TangentViewPos = mul( objectToTangent, objectView );

	// Transform the light vector into tangent space.
	float3 lightWorldPosition = float3( 3, 3, -5 );
	float3 lightObjectPosition = mul( worldToObject, lightWorldPosition );
	output.TangentLightPos = mul( objectToTangent, lightObjectPosition );

	return output;
}

// Phong reflection model.
float3 Phong( float3 ambient, float3 diffuseColor, float3 specularColor, float specularExp, float3 view, float3 light, float3 normal )
{
	float3 reflected = -1 * normalize( reflect( light, normal ) );

	float3 phong = ambient;

	// Calculate diffuse.
	float diffuse = saturate( dot( light, normal ) );
	if ( diffuse > 0.f ) {
		phong += diffuse * diffuseColor;
	}

	// Specular
	float specular = pow( saturate( dot( reflected, view ) ), specularExp );
	if ( specular > 0.f ) {
		phong += specular * specularColor;
	}

	return phong;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input ) : SV_Target
{
	input.TangentLightPos = normalize( input.TangentLightPos );
	input.TangentViewPos = normalize( input.TangentViewPos );

	float3 ambient = float3( 0, 0, 0 );
	float3 baseColor = float3( .5f, 0.f, .5f );
	float3 specularColor = float3( 1.f, 1.f, 1.f );

	float3 normalSample = gTexture.Sample( gSamplerState, float2(input.TexCoord.x, input.TexCoord.y) );
	float3 normal = float3( -1.f, -1.f, -1.f ) + normalSample * 2.f;

	return float4( Phong( ambient, baseColor, specularColor, 10.f, input.TangentViewPos, input.TangentLightPos, normal ), 1.f );
}

float4 DepthOnly( PS_INPUT input ) : SV_Target
{
	return float4( 1.f, 1.f, 1.f, 1.f );
}