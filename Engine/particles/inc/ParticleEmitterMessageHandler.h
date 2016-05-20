#include <PrecompiledHeader.h>	// for json
#include "ParticleEmitter.h"

#pragma once

namespace ITP485
{

	class ParticleEmitterMessageHandler
	{
	public:

		void HandleMessage( const json& msg, ParticleEmitterPtr emitter );

	private:

		bool ParseForInteger( const std::string& key, const std::string& target, const json& value )
		{
			return key.compare( target ) == 0 && value.is_number_integer();
		}

		bool ParseForFloat( const std::string& key, const std::string& target, const json& value )
		{
			return key.compare( target ) == 0 && value.is_number_float();
		}

		bool ParseForVector3( const std::string& key, const std::string& target, const json& value )
		{
			return key.compare( target ) == 0 && value.is_array() && value.size() == 3;
		}

	};
}
