#include "Quad.h"

#pragma once

namespace ITP485
{

	const uint32_t MAX_PARTICLES = 1024;

	// Per-particle data copied to instance buffer each frame
	union Particle
	{
		Particle() { memset( this, 0, sizeof( Particle ) ); }
		struct {
			Vector3 position;
			float age;
		};
		Particle* next;		// when the particle is not in use, this holds a link to the next free particle
	};

	class ParticleEmitter
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		ParticleEmitter();
		void ParticleEmitter::Update();
		void Render( const Vector3& viewPosition );

	private:

		Particle* SpawnParticle();
		void KillParticle( Particle* particle );

		Particle mParticles[MAX_PARTICLES];
		Vector3 mParticleVelocity[MAX_PARTICLES];

		QuadPtr mParticleQuad;
		MaterialPtr mMaterial;
		CameraPtr mCamera;
		float mSpawnTimer = 0.f;

		uint32_t mAliveParticles = 0;
		Particle *mNextFreeParticle = nullptr;

	};

	typedef shared_ptr< ParticleEmitter > ParticleEmitterPtr;

}
