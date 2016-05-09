namespace ITP485
{

	class Quad : public Mesh
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		Quad();
		virtual void Draw();

	private:

		GraphicsBufferPtr mVertexBuffer;
		GraphicsBufferPtr mIndexBuffer;

	};

	typedef shared_ptr< Quad > QuadPtr;

}