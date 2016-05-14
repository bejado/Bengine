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
		mParticleQuad = QuadPtr( new Quad() );
		mMaterial = MaterialPtr( new Material( L"Shaders\\particle.hlsl", L"Textures\\cage.dds" ) ); 

		// Give each particle an initial position and velocity
		int i = 0;
		for ( int x = -5; x < 5; x++ )
		{
			for ( int y = -5; y < 5; y++ )
			{
				for ( int z = -5; z < 5; z++ )
				{
					mParticles[i].position = Vector3( 0.f, 0.f, 0.f );
					mParticles[i].age = 0.f;
					mParticleVelocity[i] = RandomPointOnUnitSphere();
					i++;
				}
			}
		}
	}

	void ParticleEmitter::Update()
	{
		float deltaTime = Timing::Get().GetDeltaTime();
		mTime += deltaTime / 10.f;

		for ( int p = 0; p < 1000; p++ )
		{
			mParticles[p].position.Add( mParticleVelocity[p] * deltaTime );
			mParticles[p].age += deltaTime;
			// Reset the particle
			if ( mParticles[p].age > 10.f )
			{
				mParticles[p].position = Vector3( 0.f, 0.f, 0.f );
				mParticles[p].age = 0.f;
			}
		}

		Vector3 *perInstanceData = mParticleQuad->MapInstanceBuffer();
		memcpy( perInstanceData, mParticles, sizeof( Particle ) * 1000 );
		mParticleQuad->UnmapInstanceBuffer();
	}

	void ParticleEmitter::Render( const Vector3& viewPosition )
	{
		mMaterial->ActivateMaterial();
		mParticleQuad->BindContext();
		mParticleQuad->DrawInstanced( 1000 );
	}
}
