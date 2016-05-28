namespace ITP485
{

	class MeshPrimitive : public RenderPrimitive
	{
	public:
		DECLARE_ALIGNED_NEW_DELETE

		MeshPrimitive();
		virtual void Draw( const PrimitiveDrawer& drawer ) const;
		void SetTranslation( const Vector3& translation );
		void SetScale( float scale );

	protected:
		GraphicsBufferPtr mVertexBuffer;
		GraphicsBufferPtr mIndexBuffer;
		GraphicsBufferPtr mUniformBuffer;
		InputLayoutPtr mInputLayout;
		VertexShaderPtr mVertexShader;
		MaterialPtr mMaterial;
		size_t mNumIndices;

	private:
		void UpdateVertexUniformBuffer() const;
		Vector3 mTranslation;
		float mScale;	// we only support uniform scale, for now
		mutable bool mUniformBufferDirty = false;

	};

	typedef shared_ptr< MeshPrimitive > MeshPrimitivePtr;

}