#include <PrecompiledHeader.h>

namespace ITP485
{

	// Forward delcaration
	class PrimitiveDrawer;

	/**
	* Base class for all render primitives. Primitives draw themselves using the PrimitiveDrawer interface.
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
			size_t vertexStride;
			GraphicsBufferPtr indexbuffer;
			GraphicsBufferPtr vertexUniformBuffer;
			size_t indices;
			MaterialPtr material;
			InputLayoutPtr inputLayout;
			VertexShaderPtr vertexShader;
		};

		virtual void DrawMesh( const PrimitiveDrawer::Mesh& mesh ) const;
	};

	class DepthOnlyDrawer : public PrimitiveDrawer
	{
	public:

		DepthOnlyDrawer();
		virtual void DrawMesh( const PrimitiveDrawer::Mesh& mesh ) const;

	private:

		PixelShaderPtr mDepthOnlyShader;
	};

	struct PerCameraConstants
	{
		Matrix4 mProjectionViewMatrix;
		Vector3 mCameraPosition;
		Matrix4 mLightMatrix;
	};

	/**
	 * The main Rendering algorithm.
	 */
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

		void SetCamera( const ViewPtr view );
		void SetLight( const ViewPtr light );

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

		PrimitiveDrawer mBasePassDrawer;
		DepthOnlyDrawer mShadowPassDrawer;

		ViewPtr mCamera;
		ViewPtr mLight;
		vector<RenderPrimitivePtr> mPrimitives;

	};
}