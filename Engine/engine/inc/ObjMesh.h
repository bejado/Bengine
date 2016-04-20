namespace ITP485
{

	class __declspec( align( 16 ) ) ObjMesh
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		ObjMesh() : ObjMesh( Vector3( 0.f, 0.f, 0.f ) ) {}
		ObjMesh( float x, float y, float z ) : ObjMesh( Vector3( x, y, z ) ) {}
		ObjMesh( const Vector3& position );

		void UpdateObjectWorldBuffer( GraphicsBufferPtr buffer );
		void Draw();

	private:

		Vector3 mPosition;
		GraphicsBufferPtr mVertexBuffer;
		GraphicsBufferPtr mIndexBuffer;
		unsigned int mNumIndexes;

	};

	typedef shared_ptr< ObjMesh > ObjMeshPtr;

}