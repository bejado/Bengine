#include <PrecompiledHeader.h>
#include "ParticleSystem.h"
#include "ParticleSystemMessageHandler.h"

#pragma once

namespace BNG
{

	class ParticleSystemLoader
	{
	public:

		void LoadFromFile( const std::string& path, const ParticleSystemPtr system ) const;

	private:

		ParticleSystemMessageHandler mParticleSystemMessageHandler;

	};

}
