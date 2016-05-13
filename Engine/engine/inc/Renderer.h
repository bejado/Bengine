namespace ITP485
{
	struct RenderEntity
	{
		MeshPtr mMesh;
		MaterialPtr mMaterial;
		Matrix4 mModelTransform;
	};

	class Renderer : public Singleton < Renderer >
	{
	public:
		DECLARE_SINGLETON( Renderer );

		void Initialize();
		void BeginRender();
		void Submit(MeshPtr mesh, MaterialPtr material, const Matrix4& translation);
		void Render();
		void FinishRender();

	private:
		RasterizerStatePtr mRasterizerState;
		BlendStatePtr mBlendState;
		DepthStencilPtr mDepthStencilView;
		DepthStencilStatePtr mDepthStencilState;
		GraphicsBufferPtr mObjectToWorldBuffer;

		vector<RenderEntity> mEntities;

	};
}