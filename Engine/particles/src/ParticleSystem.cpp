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

	void ParticleSystem::Draw( const PrimitiveDrawer& drawer, const ViewPtr view ) const
	{
		for ( auto emitter : mEmitters )
		{
			emitter->Render( drawer, view );
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