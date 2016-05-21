#include "ParticleSystem.h"

namespace ITP485
{

	void ParticleSystem::Update()
	{
		for ( auto emitter : mEmitters )
		{
			emitter->Update();
		}
	}

	void ParticleSystem::Render( const Vector3& viewPosition )
	{
		for ( auto emitter : mEmitters )
		{
			emitter->Render( viewPosition );
		}
	}

	void ParticleSystem::BurstParticles( uint32_t amount )
	{
		for ( auto emitter : mEmitters )
		{
			emitter->BurstParticles( amount );
		}
	}

	void ParticleSystem::AddEmitter( ParticleEmitterPtr emitter )
	{
		mEmitters.push_back( emitter );
	}

}