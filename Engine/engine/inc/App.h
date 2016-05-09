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

		DepthStencilPtr		mDepthStencilView;
		DepthStencilStatePtr mDepthStencilState;


		CameraPtr mCamera;

		MeshPtr mQuad;
		MaterialPtr mMaterial;

		float mCameraMoveSpeed = 5.0f;
	};

	typedef shared_ptr< App > AppPtr;

}