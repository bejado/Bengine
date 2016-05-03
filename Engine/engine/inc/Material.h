namespace ITP485
{

	class Material
	{
	public:

		Material( std::wstring shader, std::wstring texture );
		void ActivateMaterial();

	private:

		PixelShaderPtr mPixelShader = nullptr;
		SamplerStatePtr mSamplerState = nullptr;
		TexturePtr mTexture = nullptr;

	};

	typedef shared_ptr< Material > MaterialPtr;
}