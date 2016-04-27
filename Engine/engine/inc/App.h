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

		short mMouseDeltaX = 0, mMouseDeltaY = 0;
		short mMouseOldX = 0, mMouseOldY = 0;
		bool mFirstMouseReading = true;
	};

	typedef shared_ptr< App > AppPtr;

}