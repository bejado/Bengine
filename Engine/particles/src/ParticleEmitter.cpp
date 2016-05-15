#include <PrecompiledHeader.h>
#include "ParticleEmitter.h"
#include <cstdlib>	// random number generator

namespace ITP485
{

	#define RANDOM_FLOAT ((float) std::rand() / (float) RAND_MAX - .5f)

	Vector3 RandomPointOnUnitSphere()
	{
		Vector3 v = Vector3( RANDOM_FLOAT, RANDOM_FLOAT, RANDOM_FLOAT );
		v.Normalize();
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
		}
		mNextFreeParticle = mParticles;

		// Create the particle constant buffer
		EmitterConstants constants;
		constants.startColor = Vector3( 0.f, 1.f, 0.f );
		constants.endColor = Vector3( 1.f, 0.f, 1.f );
		constants.life = mLife;
		mParticleConstantBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( &constants, sizeof( EmitterConstants ), EBindflags::EBF_ConstantBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic );
		GraphicsDriver::Get()->SetPSConstantBuffer( mParticleConstantBuffer, 1 );
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

	void ParticleEmitter::InitParticle( Particle* particle )
	{
		particle->age = 0.0f;
		particle->position = Vector3( 0.f, 0.f, 0.f );
		
		// A bit of some l33t pointer arithmatic to figure out which particle in our array we're dealing with
		uint32_t particleIndex = ( (size_t) ( particle ) - (size_t) ( mParticles ) ) / ( sizeof( Particle ) );
		mParticleVelocity[particleIndex] = RandomPointOnUnitSphere() * 5.f;
	}


	void ParticleEmitter::BurstParticles( uint32_t amount )
	{
		while ( amount-- )
		{
			Particle *particle = SpawnParticle();
			InitParticle( particle );
		}
	}

	void ParticleEmitter::Update()
	{
		float deltaTime = Timing::Get().GetDeltaTime();
		mSpawnTimer -= deltaTime;

		if ( mSpawnTimer < 0.f )
		{
			mSpawnTimer = 2.f;
			BurstParticles( 100 );
		}

		// Update all particles
		for ( int p = 0; p < MAX_PARTICLES; p++ )
		{
			if ( mParticles[p].age >= 0.f )
			{
				mParticles[p].position.Add( mParticleVelocity[p] * deltaTime );
				mParticles[p].age += deltaTime;
				mParticleVelocity[p].Add( Vector3( 0.f, -10.0 * deltaTime, 0.f ) );
			}

			// Reset the particle
			if ( mParticles[p].age > mLife )
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
