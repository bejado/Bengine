namespace ITP485
{

	class Mesh
	{
	public:

		virtual void BindBuffers() = 0;
		virtual void Draw() = 0;

	private:

	};

	typedef shared_ptr< Mesh > MeshPtr;

	// We use these PackedVectors because we don't want byte-alignment

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

	struct VERTEX_P_N_T
	{
		VERTEX_P_N_T( const PackedVector3& position, const PackedVector3& normal, const PackedVector2& texCoord ) : mPosition( position ), mNormal( normal ), mTexCoord( texCoord ) {}
		PackedVector3 mPosition;
		PackedVector3 mNormal;
		PackedVector2 mTexCoord;
	};
}