#pragma once

namespace ITP485
{

	// Per-particle data copied to instance buffer each frame
	struct Particle
	{
		Vector3 position;
		float age;
	};

	class ParticleEmitter
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		ParticleEmitter();
		void ParticleEmitter::Update();
		void Render( const Vector3& viewPosition );

	private:

		Particle mParticles[1000];
		Vector3 mParticleVelocity[1000];

		QuadPtr mParticleQuad;
		MaterialPtr mMaterial;
		CameraPtr mCamera;
		float mTime = 0.f;

	};

	typedef shared_ptr< ParticleEmitter > ParticleEmitterPtr;

}
