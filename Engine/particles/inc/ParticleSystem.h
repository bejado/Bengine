#include <PrecompiledHeader.h>
#include "ParticleEmitter.h"

#pragma once

namespace ITP485
{

	class ParticleSystem
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		void AddEmitter( ParticleEmitterPtr emitter );

		void Update();
		void Render( const Vector3& viewPosition );
		void BurstParticles( uint32_t amount );

		// We'll delegate all message handling to a message handler class
		friend class ParticleSystemMessageHandler;

	private:

		std::vector<ParticleEmitterPtr> mEmitters;

	};

	typedef shared_ptr< ParticleSystem > ParticleSystemPtr;

}
