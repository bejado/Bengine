namespace ITP485
{

	class ObjMesh : public Mesh
	{
	public:

		ObjMesh( std::string file );
		virtual void BindContext();
		virtual void Draw();

	private:

		GraphicsBufferPtr mVertexBuffer;
		GraphicsBufferPtr mIndexBuffer;
		unsigned int mNumIndexes;

	};

	typedef shared_ptr< ObjMesh > ObjMeshPtr;

}