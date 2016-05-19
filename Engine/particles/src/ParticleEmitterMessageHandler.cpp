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

				if ( key.compare( "burst" ) == 0 && value.is_number_integer() )
				{
					uint32_t amount = it.value();
					emitter->BurstParticles( amount );
				}

				if ( key.compare( "life" ) == 0 && value.is_number_float() )
				{
					float life = it.value();
					emitter->mEmitterConstants.life = life;
				}

				if ( key.compare( "gravity" ) == 0 && value.is_number_float() )
				{
					float gravity = it.value();
					emitter->mGravity = gravity;
				}

				if ( key.compare( "startColor" ) == 0 && value.is_array() && value.size() == 3 )
				{
					emitter->mEmitterConstants.startColor = Vector3( value[0], value[1], value[2] );
				}

				if ( key.compare( "endColor" ) == 0 && value.is_array() && value.size() == 3 )
				{
					emitter->mEmitterConstants.endColor = Vector3( value[0], value[1], value[2] );
				}
			}
		}
	}
}