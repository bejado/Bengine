#include "MessageHandler.h"
#include <PrecompiledHeader.h>	// for json
#include "ParticleEmitter.h"

#pragma once

namespace BNG
{

	class ParticleEmitterMessageHandler : public MessageHandler
	{
	public:

		void HandleMessage( const json& msg, ParticleEmitterPtr emitter ) const;

	};
}
