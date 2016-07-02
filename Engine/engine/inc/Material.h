namespace ITP485
{

	class Material
	{
	public:

		Material( std::wstring shaderPath, std::wstring texturePath );
		void ActivateMaterial() const;
		void ActivateDepthOnlyMaterial() const;

	private:

		PixelShaderPtr mPixelShader = nullptr;
		PixelShaderPtr mDepthOnlyPixelShader = nullptr;
		SamplerStatePtr mSamplerState = nullptr;
		TexturePtr mTexture = nullptr;

	};

	typedef shared_ptr< Material > MaterialPtr;
}