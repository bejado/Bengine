#include <PrecompiledHeader.h>
#include "ParticleSystem.h"
#include "ParticleSystemMessageHandler.h"

#pragma once

namespace ITP485
{

	class ParticleSystemLoader
	{
	public:

		void LoadFromFile( const std::string& path, const ParticleSystemPtr system ) const;

	private:

		ParticleSystemMessageHandler mParticleSystemMessageHandler;

	};

}
