#pragma once

#include "Renderer.h"

namespace ITP485
{

	// We use these PackedVectors because we don't want byte-alignment
	// TODO: candidate for moving these structs into a "VertexFactory" type class

	struct PackedVector2
	{
		PackedVector2( float x, float y ) : mX( x ), mY( y ) {}
		PackedVector2() : PackedVector2( 0.f, 0.f ) {}
		float mX, mY;
	};

	struct PackedVector3
	{
		PackedVector3( float x, float y, float z ) : mX( x ), mY( y ), mZ( z ) {}
		PackedVector3() : PackedVector3( 0.f, 0.f, 0.f ) {}
		float mX, mY, mZ;
	};

	struct VERTEX_P_T
	{
		VERTEX_P_T( const PackedVector3& position, const PackedVector2& texCoord ) : mPosition( position ), mTexCoord( texCoord ) {}
		PackedVector3 mPosition;
		PackedVector2 mTexCoord;
	};

	struct VERTEX_P_N_T
	{
		VERTEX_P_N_T( const PackedVector3& position, const PackedVector3& normal, const PackedVector2& texCoord ) : mPosition( position ), mNormal( normal ), mTexCoord( texCoord ) {}
		PackedVector3 mPosition;
		PackedVector3 mNormal;
		PackedVector2 mTexCoord;
	};

	class MeshPrimitive : public RenderPrimitive
	{
	public:
		DECLARE_ALIGNED_NEW_DELETE

		MeshPrimitive();
		virtual void Draw( const PrimitiveDrawer& drawer, const ViewPtr view ) const;
		void SetTranslation( const Vector3& translation );
		void SetScale( float scale );
		void SetRotation( const Quaternion& rotation );

	protected:
		GraphicsBufferPtr mVertexBuffer;
		GraphicsBufferPtr mIndexBuffer;
		GraphicsBufferPtr mUniformBuffer;
		InputLayoutPtr mInputLayout;
		VertexShaderPtr mVertexShader;
		MaterialPtr mMaterial;
		size_t mNumIndices;
		size_t mVertexStride;

	private:
		void UpdateVertexUniformBuffer() const;
		Vector3 mTranslation;
		float mScale;	// we only support uniform scale, for now
		Quaternion mRotation;
		mutable bool mUniformBufferDirty = false;

	};

	typedef shared_ptr< MeshPrimitive > MeshPrimitivePtr;

}
