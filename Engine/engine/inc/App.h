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

		GraphicsBufferPtr mObjectToWorldBuffer;

		CameraPtr mCamera;

		MeshPtr mFighterMesh;
		MaterialPtr mFighterMaterial;

		float mCameraPathAmount = 0.f;
	};

	typedef shared_ptr< App > AppPtr;

}