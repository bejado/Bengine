namespace ITP485
{

	class Cube : public Mesh
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		Cube();
		virtual void Draw();

	private:

		GraphicsBufferPtr mVertexBuffer;
		GraphicsBufferPtr mIndexBuffer;

	};

	typedef shared_ptr< Cube > CubePtr;

}