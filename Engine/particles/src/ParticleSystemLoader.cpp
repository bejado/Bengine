#include "ParticleSystemLoader.h"
#include <fstream>

namespace BNG
{

	void ParticleSystemLoader::LoadFromFile( const std::string& path, const ParticleSystemPtr system ) const
	{
		std::ifstream filestream( path );
		std::stringstream buffer;
		buffer << filestream.rdbuf();
		json file = json::parse( buffer.str() );

		// Loop through all the key/value pairs of the object
		for ( json::const_iterator it = file.begin(); it != file.end(); ++it )
		{
			std::string key = it.key();
			json value = it.value();

			// Array of emitters
			std::regex emittersRegex( "emitters" );
			std::smatch result;
			if ( std::regex_match( key, result, emittersRegex ) )
			{

				// Loop through each emitter and translate it into a "new_emitter" message
				for ( const json& thisEmitter : value )
				{
					json newEmitterMsg;
					newEmitterMsg["new_emitter"] = thisEmitter;

					mParticleSystemMessageHandler.HandleMessage( newEmitterMsg, system );
				}

			}

		}
	}

}