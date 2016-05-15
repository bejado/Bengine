#include <PrecompiledHeader.h>
#include "ParticleEmitter.h"
#include <cstdlib>	// random number generator

namespace ITP485
{

	#define RANDOM_FLOAT ((float) std::rand() / (float) RAND_MAX - .5f)

	Vector3 RandomPointOnUnitSphere()
	{
		Vector3 v = Vector3( RANDOM_FLOAT, RANDOM_FLOAT, RANDOM_FLOAT );
		// v.Normalize();
		return v;
	}

	ParticleEmitter::ParticleEmitter()
	{
		mParticleQuad = QuadPtr( new Quad( sizeof(Particle), MAX_PARTICLES ) );
		mMaterial = MaterialPtr( new Material( L"Shaders\\particle.hlsl", L"Textures\\cage.dds" ) ); 

		// Initialize and link the particles
		for ( int p = 0; p < MAX_PARTICLES; p++ )
		{
			if ( p < MAX_PARTICLES - 1 )	// the last particle doesn't need to be linked
			{
				mParticles[p].next = &mParticles[p + 1];
			}
			mParticles[p].age = -1.f;	// all particles start off dead
			mParticleVelocity[p] = RandomPointOnUnitSphere() * 2.f;	//TODO: this shouldn't be here
		}
		mNextFreeParticle = mParticles;
	}

	Particle* ParticleEmitter::SpawnParticle()
	{
		mAliveParticles++;
		Dbg_Assert( mAliveParticles <= MAX_PARTICLES, "Ran out of particles!" );
		Particle* p = mNextFreeParticle;
		mNextFreeParticle = mNextFreeParticle->next;
		return p;
	}

	void ParticleEmitter::KillParticle( Particle* particle )
	{
		particle->age = -1.f;
		mAliveParticles--;
		particle->next = mNextFreeParticle;
		mNextFreeParticle = particle;
	}

	void ParticleEmitter::Update()
	{
		float deltaTime = Timing::Get().GetDeltaTime();
		mSpawnTimer -= deltaTime;
		if ( mSpawnTimer < 0.f )
		{
			mSpawnTimer = .05f;
			Particle* particle = SpawnParticle();
			particle->age = 0.0f;
			particle->position = Vector3( 0.f, 0.f, 0.f );
		}

		// Update all particles
		for ( int p = 0; p < MAX_PARTICLES; p++ )
		{
			if ( mParticles[p].age >= 0.f )
			{
				mParticles[p].position.Add( mParticleVelocity[p] * deltaTime );
				mParticles[p].age += deltaTime;
			}
			// Reset the particle
			if ( mParticles[p].age > 2.f )
			{
				KillParticle( &mParticles[p] );
			}
		}

		// Update the particle instance buffer
		Particle *perInstanceData = static_cast<Particle*>( mParticleQuad->MapInstanceBuffer() );
		memcpy( perInstanceData, mParticles, sizeof( Particle ) * MAX_PARTICLES );
		mParticleQuad->UnmapInstanceBuffer();
	}

	void ParticleEmitter::Render( const Vector3& viewPosition )
	{
		mMaterial->ActivateMaterial();
		mParticleQuad->BindContext();
		mParticleQuad->DrawInstanced( MAX_PARTICLES );
	}
}
