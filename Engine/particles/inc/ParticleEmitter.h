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

		DECLARE_ALIGNED_NEW_DELETE

		ParticleEmitter();
		void ParticleEmitter::Update();
		void Render( const Vector3& viewPosition );

	private:

		Particle mParticles[1000];
		QuadPtr mParticleQuad;
		MaterialPtr mMaterial;
		CameraPtr mCamera;
		float mTime = 0.f;

	};

	typedef shared_ptr< ParticleEmitter > ParticleEmitterPtr;

}
