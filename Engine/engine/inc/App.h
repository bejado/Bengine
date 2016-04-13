namespace ITP485
{

	class App
	{
	public:
		App();
		void Update();
		void Render();

	private:
		VertexShaderPtr mVertexShader;
		PixelShaderPtr mPixelShader;
		GraphicsBufferPtr mVertexBuffer;
	};

	typedef shared_ptr< App > AppPtr;

}