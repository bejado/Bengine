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
		GraphicsBufferPtr mIndexBuffer;

		GraphicsBufferPtr mObjectToWorldBuffer;

		CameraPtr mCamera;
	};

	typedef shared_ptr< App > AppPtr;

}