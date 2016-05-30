namespace ITP485
{

	// Forward delcaration
	class PrimitiveDrawer;

	/**
	* Base class for all render primitives. Primitives draw themselves given a PrimitiveDrawer.
	*/
	class RenderPrimitive
	{
	public:
		virtual void Draw( const PrimitiveDrawer& drawer ) const = 0;

	};

	typedef shared_ptr< RenderPrimitive > RenderPrimitivePtr;

	class PrimitiveDrawer
	{
	public:

		struct Mesh
		{
			GraphicsBufferPtr vertexBuffer;
			GraphicsBufferPtr indexbuffer;
			GraphicsBufferPtr vertexUniformBuffer;
			size_t indices;
			MaterialPtr material;
			InputLayoutPtr inputLayout;
			VertexShaderPtr vertexShader;
		};

		virtual void DrawMesh( const PrimitiveDrawer::Mesh& mesh ) const
		{
			GraphicsDriver::Get()->SetVSConstantBuffer( mesh.vertexUniformBuffer, 1 );
			GraphicsDriver::Get()->SetVertexShader( mesh.vertexShader );
			GraphicsDriver::Get()->SetInputLayout( mesh.inputLayout );
			GraphicsDriver::Get()->SetVertexBuffer( mesh.vertexBuffer, sizeof( VERTEX_P_N_T ) );	// TODO: here's where the vertex factory comes in
			GraphicsDriver::Get()->SetIndexBuffer( mesh.indexbuffer );
			mesh.material->ActivateMaterial();

			// Draw!
			GraphicsDriver::Get()->DrawIndexed( mesh.indices, 0, 0 );
		}

	};

	class DepthOnlyDrawer : public PrimitiveDrawer
	{
	public:

		DepthOnlyDrawer()
		{
			// Load the pixel shader
			vector< char > compiledPixelShader;
			GraphicsDriver::Get()->CompileShaderFromFile( L"Resources\\Shaders\\shadow.hlsl", "DepthOnly", "ps_4_0", compiledPixelShader );
			mDepthOnlyShader = GraphicsDriver::Get()->CreatePixelShader( compiledPixelShader );
		}

		virtual void DrawMesh( const PrimitiveDrawer::Mesh& mesh ) const
		{
			GraphicsDriver::Get()->SetVSConstantBuffer( mesh.vertexUniformBuffer, 1 );
			GraphicsDriver::Get()->SetVertexShader( mesh.vertexShader );
			GraphicsDriver::Get()->SetInputLayout( mesh.inputLayout );
			GraphicsDriver::Get()->SetVertexBuffer( mesh.vertexBuffer, sizeof( VERTEX_P_N_T ) );	// TODO: here's where the vertex factory comes in
			GraphicsDriver::Get()->SetIndexBuffer( mesh.indexbuffer );
			GraphicsDriver::Get()->SetPixelShader( mDepthOnlyShader );

			// Draw!
			GraphicsDriver::Get()->DrawIndexed( mesh.indices, 0, 0 );
		}

	private:

		PixelShaderPtr mDepthOnlyShader;
	};

	struct PerCameraConstants
	{
		Matrix4 mProjectionViewMatrix;
		Vector3 mCameraPosition;
		Matrix4 mLightMatrix;
	};

	class Renderer : public Singleton < Renderer >
	{
	public:
		DECLARE_SINGLETON( Renderer );

		void Initialize();
		void Render();

		/**
		* Add a primitive to this Renderer to be rendered each frame.
		*/
		void AddPrimitive( const RenderPrimitivePtr primitive );

		void SetCamera( const CameraPtr& camera );

	private:
		void UpdateViewConstants( const Matrix4& projectionView, const Vector3& position ) const;

		RasterizerStatePtr mRasterizerState;
		BlendStatePtr mBlendState;
		DepthStencilPtr mDepthStencilView;
		DepthStencilStatePtr mDepthStencilState;
		GraphicsBufferPtr mCameraConstantBuffer;

		DepthStencilPtr mShadowMapDepthStencil;
		TexturePtr mShadowMapTexture;
		SamplerStatePtr mShadowMapSamplerState;

		CameraPtr mCamera;
		CameraPtr mLight;	// TODO: make this a real light!
		vector<RenderPrimitivePtr> mPrimitives;

	};
}