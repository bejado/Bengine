namespace BNG
{

	class Material
	{
	public:

		Material( std::wstring shaderPath, std::wstring texturePath );
		virtual void ActivateMaterial() const;
		virtual void DeactivateMaterial() const;
		void ActivateDepthOnlyMaterial() const;

	protected:

		PixelShaderPtr mPixelShader = nullptr;
		PixelShaderPtr mDepthOnlyPixelShader = nullptr;
		SamplerStatePtr mSamplerState = nullptr;
		TexturePtr mTexture = nullptr;

	};

	typedef shared_ptr< Material > MaterialPtr;
}