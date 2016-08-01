#include "ParticleSystem.h"

namespace BNG
{

	ParticleSystem::ParticleSystem()
	{}

	void ParticleSystem::Update()
	{
		for ( auto& emitter : mEmitters )
		{
			emitter->Update();
		}
	}

	void ParticleSystem::SetEmitterOrigin( const Vector3& origin )
	{
		for ( auto& emitter : mEmitters )
		{
			emitter->SetEmitterOrigin( origin );
		}
	}

	void ParticleSystem::SetEmitterState( bool state )
	{
		for ( auto& emitter : mEmitters )
		{
			emitter->SetEmitterState( state );
		}
	}

	void ParticleSystem::SetTranslationMatrix( const Matrix4& translation )
	{
		for ( auto& emitter : mEmitters )
		{
			emitter->SetTranslationMatrix( translation );
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