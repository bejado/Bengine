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



	};
}
