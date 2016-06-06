namespace ITP485
{

	class Material
	{
	public:

		Material( std::wstring shaderPath, std::wstring texturePath, bool depthBufferReadOnly );
		void ActivateMaterial() const;
		void ActivateDepthOnlyMaterial() const;
		bool DepthBufferIsReadOnly() { return mDepthBufferReadOnly; }

	private:

		PixelShaderPtr mPixelShader = nullptr;
		PixelShaderPtr mDepthOnlyPixelShader = nullptr;
		SamplerStatePtr mSamplerState = nullptr;
		TexturePtr mTexture = nullptr;
		bool mDepthBufferReadOnly = false;

	};

	typedef shared_ptr< Material > MaterialPtr;
}