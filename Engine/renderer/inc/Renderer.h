#pragma once

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
		virtual void Draw( const PrimitiveDrawer& drawer, const ViewPtr view ) const = 0;

	};

	typedef shared_ptr< RenderPrimitive > RenderPrimitivePtr;

	class PrimitiveDrawer
	{
	public:

		/**
		* The mesh data to render. Set instanceCount > 1 to render instanced meshes.
		*/
		struct Mesh
		{
			InputLayoutPtr inputLayout = nullptr;
			VertexShaderPtr vertexShader = nullptr;

			GraphicsBufferPtr vertexBuffer = nullptr;
			GraphicsBufferPtr instanceBuffer = nullptr;
			size_t vertexStride = 0;
			size_t instanceStride = 0;
			GraphicsBufferPtr indexBuffer = nullptr;
			GraphicsBufferPtr vertexUniformBuffer = nullptr;
			GraphicsBufferPtr fragmentUniformBuffer = nullptr;

			size_t indices = 0;
			size_t instanceCount = 1;

			MaterialPtr material;
		};

		virtual void ActivateMaterial( const PrimitiveDrawer::Mesh& mesh ) const;

		/**
		* Bind the depth state for the mesh. Return true if depth state should be restored to the normal state after drawing.
		*/
		virtual bool SetDepthState( const PrimitiveDrawer::Mesh& mesh ) const;

		virtual void DrawMesh( const PrimitiveDrawer::Mesh& mesh ) const;

	};

	class DepthOnlyDrawer : public PrimitiveDrawer
	{
	public:

		void ActivateMaterial( const PrimitiveDrawer::Mesh& mesh ) const override;

	};

	class TransclucentDrawer : public PrimitiveDrawer
	{
	public:

		bool SetDepthState( const PrimitiveDrawer::Mesh& mesh ) const override;

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

		Renderer() : mCamera( nullptr), mLight( nullptr ) {}

		void Initialize();
		void Render();

		/**
		* Add a primitive to this Renderer to be rendered each frame.
		*/
		void AddPrimitive( const RenderPrimitivePtr primitive );

		/**
		* Add a primitive to be rendererd in a separate translucency pass after all opaque primitivies./
		*/
		void AddTranslucentPrimitive( const RenderPrimitivePtr primitive );

		void SetCamera( const ViewPtr view );
		void SetLight( const ViewPtr light );

	private:
		void UpdateViewConstants( const Matrix4& projectionView, const Vector3& position ) const;

		RasterizerStatePtr mRasterizerState;
		BlendStatePtr mBlendState;
		DepthStencilPtr mDepthStencilView;
		GraphicsBufferPtr mCameraConstantBuffer;

		DepthStencilPtr mShadowMapDepthStencil;
		TexturePtr mShadowMapTexture;
		SamplerStatePtr mShadowMapSamplerState;

		PrimitiveDrawer mBasePassDrawer;
		DepthOnlyDrawer mShadowPassDrawer;
		TransclucentDrawer mTransclucentDrawer;

		ViewPtr mCamera;
		ViewPtr mLight;
		vector<RenderPrimitivePtr> mPrimitives;
		vector<RenderPrimitivePtr> mTranslucentPrimitives;

	};
}