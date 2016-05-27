#include "ParticleEmitterMessageHandler.h"

namespace ITP485
{
	void ParticleEmitterMessageHandler::HandleMessage( const json& msg, ParticleEmitterPtr emitter ) const
	{
		if ( msg.is_object() )
		{
			// Loop through all the key/value pairs of the object
			for ( json::const_iterator it = msg.begin(); it != msg.end(); ++it ) {
				std::string key = it.key();
				json value = it.value();


				if ( MessageHandler::ParseForFloat( key, "life", value ) )
				{
					emitter->mEmitterConstants.life = value;
				}

				if ( MessageHandler::ParseForFloat( key, "gravity", value ) )
				{
					emitter->mGravity = value;
				}

				if ( MessageHandler::ParseForFloat( key, "initialVelocity", value ) )
				{
					emitter->mInitialVelocity = value;
				}

				if ( MessageHandler::ParseForFloat( key, "rate", value ) )
				{
					emitter->mEmitterRate = value;
				}

				if ( MessageHandler::ParseForVector3( key, "startColor", value ) )
				{
					emitter->mEmitterConstants.startColor = Vector3( value[0], value[1], value[2] );
				}

				if ( MessageHandler::ParseForVector3( key, "endColor", value ) )
				{
					emitter->mEmitterConstants.endColor = Vector3( value[0], value[1], value[2] );
				}
			}
		}
	}
}