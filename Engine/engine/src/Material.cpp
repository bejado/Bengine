#include <PrecompiledHeader.h>

namespace ITP485
{
	Material::Material( std::wstring shaderPath, std::wstring texturePath )
	{
		// Load the pixel shader
		vector< char > compiledPixelShader;
		GraphicsDriver::Get()->CompileShaderFromFile( shaderPath.c_str(), "PS", "ps_4_0", compiledPixelShader );
		mPixelShader = GraphicsDriver::Get()->CreatePixelShader( compiledPixelShader );

		// Create a sampler state
		mSamplerState = GraphicsDriver::Get()->CreateSamplerState();

		// Load and set the texture
		mTexture = GraphicsDriver::Get()->CreateTextureFromFile( texturePath.c_str() );
	}

	void Material::ActivateMaterial()
	{
		GraphicsDriver::Get()->SetPSSamplerState( mSamplerState, 0 );
		GraphicsDriver::Get()->SetPSTexture( mTexture, 0 );
		GraphicsDriver::Get()->SetPixelShader( mPixelShader );
	}
}
