namespace ITP485
{

	class __declspec( align( 16 ) ) Cube
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		Cube() : Cube( Vector3( 0.f, 0.f, 0.f ) ) {}
		Cube( float x, float y, float z ) : Cube( Vector3( x, y, z ) ) {}
		Cube( const Vector3& position );

		void UpdateObjectWorldBuffer( GraphicsBufferPtr buffer );
		void Draw();

	private:

		Vector3 mPosition;
		GraphicsBufferPtr mVertexBuffer;
		GraphicsBufferPtr mIndexBuffer;

	};

	typedef shared_ptr< Cube > CubePtr;

}