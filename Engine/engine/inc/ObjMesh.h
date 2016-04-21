namespace ITP485
{

	class __declspec( align( 16 ) ) ObjMesh : public Mesh
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		ObjMesh( std::string file );
		virtual void Draw();

	private:

		GraphicsBufferPtr mVertexBuffer;
		GraphicsBufferPtr mIndexBuffer;
		unsigned int mNumIndexes;

	};

	typedef shared_ptr< ObjMesh > ObjMeshPtr;

}