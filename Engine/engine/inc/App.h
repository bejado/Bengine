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

		GraphicsBufferPtr mObjectToWorldBuffer;

		CameraPtr mCamera;

		CubePtr mCube[13];

		float mCameraPathAmount = 0.f;
	};

	typedef shared_ptr< App > AppPtr;

}