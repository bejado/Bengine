namespace ITP485
{
	const uint32_t MAX_INSTANCES = 10000;

	class Quad 
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		Quad();
		virtual void BindContext();
		void DrawInstanced( uint32_t instanceCount );
		Vector3* MapInstanceBuffer();
		void UnmapInstanceBuffer();

	private:

		GraphicsBufferPtr mVertexBuffer;
		GraphicsBufferPtr mIndexBuffer;
		GraphicsBufferPtr mInstanceBuffer;

		VertexShaderPtr mVertexShader;
		InputLayoutPtr mInputLayout;

		Vector3 mInstanceData[MAX_INSTANCES];

	};

	typedef shared_ptr< Quad > QuadPtr;

}