#include "ParticleEmitter.h"
#include <cstdlib>	// random number generator

namespace ITP485
{

	#define RANDOM_FLOAT ((float) std::rand() / (float) RAND_MAX - .5f)

	GraphicsBufferPtr ParticleEmitter::ParticleConstantBuffer = nullptr;

	Vector3 RandomPointOnUnitSphere()
	{
		Vector3 v = Vector3( RANDOM_FLOAT, RANDOM_FLOAT, RANDOM_FLOAT );
		v.Normalize();
		return v;
	}

	ParticleEmitter::ParticleEmitter( const Vector3& emitterPosition,
									  const Vector3& startColor,
									  const Vector3& endColor ) : mEmitterPosition( emitterPosition )
	{
		mParticleQuad = QuadPtr( new Quad( sizeof(Particle), MAX_PARTICLES ) );
		mMaterial = MaterialPtr( new Material( L"Shaders\\particle.hlsl", L"" ) ); 

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

		mEmitterConstants.startColor = startColor;
		mEmitterConstants.endColor = endColor;
		mEmitterConstants.life = 1.f;

		// Create the particle constant buffer, if one doesn't already exist
		if ( ParticleConstantBuffer == nullptr )
		{
			ParticleConstantBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( nullptr, sizeof( EmitterConstants ), EBindflags::EBF_ConstantBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic );
			GraphicsDriver::Get()->SetPSConstantBuffer( ParticleConstantBuffer, 1 );
		}
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
		particle->position = mEmitterPosition;
		
		// A bit of some l33t pointer arithmetic to figure out which particle in our array we're dealing with
		uint32_t particleIndex = ( (size_t) ( particle ) - (size_t) ( mParticles ) ) / ( sizeof( Particle ) );
		mParticleVelocity[particleIndex] = RandomPointOnUnitSphere() * mInitialVelocity;
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
			BurstParticles( 1 );
			mSpawnTimer = 1.f / mEmitterRate;
		}

		// Update all particles
		for ( int p = 0; p < MAX_PARTICLES; p++ )
		{
			if ( mParticles[p].age >= 0.f )
			{
				mParticles[p].position.Add( mParticleVelocity[p] * deltaTime );
				mParticles[p].age += deltaTime;
				if ( mGravity != 0.f )
				{
					mParticleVelocity[p].Add( Vector3( 0.f, mGravity * deltaTime, 0.f ) );
				}
			}

			// Reset the particle
			if ( mParticles[p].age > mEmitterConstants.life )
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
		UpdateParticleConstantBuffer();
		mParticleQuad->BindContext();
		mParticleQuad->DrawInstanced( MAX_PARTICLES );
	}

	void ParticleEmitter::UpdateParticleConstantBuffer()
	{
		EmitterConstants *emitterData = GraphicsDriver::Get()->MapBuffer<EmitterConstants>( ParticleConstantBuffer );
		memcpy( emitterData, &mEmitterConstants, sizeof( EmitterConstants ) );
		GraphicsDriver::Get()->UnmapBuffer( ParticleConstantBuffer );
	}
}
