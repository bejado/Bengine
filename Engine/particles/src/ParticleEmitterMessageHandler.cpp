#include "ParticleEmitterMessageHandler.h"

namespace ITP485
{
	void ParticleEmitterMessageHandler::HandleMessage( const json& msg, ParticleEmitterPtr emitter )
	{
		if ( msg.is_object() )
		{
			// Loop through all the key/value pairs of the object
			for ( json::const_iterator it = msg.begin(); it != msg.end(); ++it ) {
				std::string key = it.key();
				json value = it.value();

				if ( ParseForInteger( key, "burst", value ) )
				{
					emitter->BurstParticles( value );
				}

				if ( ParseForFloat( key, "life", value ) )
				{
					emitter->mEmitterConstants.life = value;
				}

				if ( ParseForFloat( key, "gravity", value ) )
				{
					emitter->mGravity = value;
				}

				if ( ParseForFloat( key, "initialVelocity", value ) )
				{
					emitter->mInitialVelocity = value;
				}

				if ( ParseForFloat( key, "rate", value ) )
				{
				}

				if ( ParseForVector3( key, "startColor", value ) )
				{
					emitter->mEmitterConstants.startColor = Vector3( value[0], value[1], value[2] );
				}

				if ( ParseForVector3( key, "endColor", value ) )
				{
					emitter->mEmitterConstants.endColor = Vector3( value[0], value[1], value[2] );
				}
			}
		}
	}
}