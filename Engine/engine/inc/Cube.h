namespace ITP485
{

	class __declspec( align( 16 ) ) Cube : public Mesh
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