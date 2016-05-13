#pragma once

namespace ITP485
{

	struct Particle
	{
		Vector3 position;
	};

	class ParticleEmitter
	{
	public:

		ParticleEmitter();
		void ParticleEmitter::Update();
		void Render( const Vector3& viewPosition );

		static Matrix4 CreateBillboardMatrix( const Vector3& anchor, const Vector3& cameraPosition );

	private:

		vector<Particle> mParticles;
		MeshPtr mParticleQuad;
		MaterialPtr mMaterial;
		CameraPtr mCamera;

	};

	typedef shared_ptr< ParticleEmitter > ParticleEmitterPtr;

}
