#include "Quad.h"

#pragma once

namespace ITP485
{

	const uint32_t MAX_PARTICLES = 1024;

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

		Particle mParticles[MAX_PARTICLES];
		Vector3 mParticleVelocity[MAX_PARTICLES];

		QuadPtr mParticleQuad;
		MaterialPtr mMaterial;
		CameraPtr mCamera;
		float mTime = 0.f;

	};

	typedef shared_ptr< ParticleEmitter > ParticleEmitterPtr;

}
