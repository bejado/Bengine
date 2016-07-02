#include <PrecompiledHeader.h>

namespace ITP485
{
	Material::Material( std::wstring shaderPath, std::wstring texturePath )
	{
		// Load the pixel shader
		vector< char > compiledPixelShader;
		GraphicsDriver::Get()->CompileShaderFromFile( shaderPath.c_str(), "PS", "ps_4_0", compiledPixelShader );
		mPixelShader = GraphicsDriver::Get()->CreatePixelShader( compiledPixelShader );

		// Load the depth only shader
		compiledPixelShader.clear();
		GraphicsDriver::Get()->CompileShaderFromFile( shaderPath.c_str(), "DepthOnly", "ps_4_0", compiledPixelShader );
		mDepthOnlyPixelShader = GraphicsDriver::Get()->CreatePixelShader( compiledPixelShader );

		// Create a sampler state
		mSamplerState = GraphicsDriver::Get()->CreateSamplerState( ETextureAddressMode::EWrap, ETextureAddressMode::EWrap );

		// Load and set the texture
		if ( !texturePath.empty() ) {
			mTexture = GraphicsDriver::Get()->CreateTextureFromFile( texturePath.c_str() );
			mNormalTexture = GraphicsDriver::Get()->CreateTextureFromFile( L"Resources\\Textures\\fighter_normal.dds" );
		}
	}

	void Material::ActivateMaterial() const
	{
		if (mTexture) {
			GraphicsDriver::Get()->SetPSSamplerState( mSamplerState, 0 );
			GraphicsDriver::Get()->SetPSTexture( mTexture, 0 );
			GraphicsDriver::Get()->SetPSTexture( mNormalTexture, 1 );
		}
		GraphicsDriver::Get()->SetPixelShader( mPixelShader );
	}

	void Material::ActivateDepthOnlyMaterial() const
	{
		GraphicsDriver::Get()->SetPixelShader( mDepthOnlyPixelShader );
	}
}
