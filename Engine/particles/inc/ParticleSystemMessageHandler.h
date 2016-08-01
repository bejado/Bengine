#include "MessageHandler.h"
#include <PrecompiledHeader.h>	// for json
#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include "ParticleEmitterMessageHandler.h"

#pragma once

namespace BNG
{

	class ParticleSystemMessageHandler : public MessageHandler
	{
	public:

		void HandleMessage( const json& msg, ParticleSystemPtr system ) const;

	private:

		ParticleEmitterMessageHandler mParticleEmitterMessageHandler;

	};

}