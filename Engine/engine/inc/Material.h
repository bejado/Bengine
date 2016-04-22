namespace ITP485
{

	class Material
	{
	public:

		Material( std::wstring shader, std::wstring texture );
		void ActivateMaterial();

	private:

		PixelShaderPtr mPixelShader;
		SamplerStatePtr mSamplerState;
		TexturePtr mTexture;

	};

	typedef shared_ptr< Material > MaterialPtr;
}