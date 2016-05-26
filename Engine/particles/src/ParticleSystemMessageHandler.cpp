#include "ParticleSystemMessageHandler.h"

namespace ITP485
{

	void ParticleSystemMessageHandler::HandleMessage( const json& msg, ParticleSystemPtr system )
	{
		if ( msg.is_object() )
		{
			// Loop through all the key/value pairs of the object
			for ( json::const_iterator it = msg.begin(); it != msg.end(); ++it )
			{
				std::string key = it.key();
				json value = it.value();

				std::smatch result;

				if ( MessageHandler::ParseForInteger( key, "burst", value ) )
				{
					system->BurstParticles( value );
				}

				// Modify emitter
				std::regex modifyEmitterRegex( "emitter(\\d+)" );
				if ( std::regex_match( key, result, modifyEmitterRegex ) )
				{
					std::string emitterIndexString = result[1];
					size_t emitterIndex = std::stoi( emitterIndexString );
					Dbg_Assert( emitterIndex < system->mEmitters.size(), "Emitter index is out of range." );

					mParticleEmitterMessageHandler.HandleMessage( value, system->mEmitters[emitterIndex] );
				}

				// New emitter
				std::regex newEmitterRegex( "new_emitter" );
				if ( std::regex_match( key, result, newEmitterRegex ) )
				{
					ParticleEmitterPtr newEmitter = ParticleEmitterPtr( new ParticleEmitter() );
					system->AddEmitter( newEmitter );
					mParticleEmitterMessageHandler.HandleMessage( value, newEmitter );
				}
			}
		}
	}

}
