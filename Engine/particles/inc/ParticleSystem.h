#include <PrecompiledHeader.h>
#include "ParticleEmitter.h"

#pragma once

namespace ITP485
{

	class ParticleSystem : public RenderPrimitive
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		ParticleSystem();

		void AddEmitter( ParticleEmitterPtr emitter );

		void Update();
		virtual void Draw( const PrimitiveDrawer& drawer, const ViewPtr view ) const;
		void BurstParticles( uint32_t amount );
		void SetEmitterState( bool state );
		void SetEmitterOrigin( const Vector3& origin );
		void SetTranslationMatrix( const Matrix4& translation );

		// We'll delegate all message handling to a message handler class
		friend class ParticleSystemMessageHandler;

	private:

		std::vector<ParticleEmitterPtr> mEmitters;

	};

	typedef shared_ptr< ParticleSystem > ParticleSystemPtr;

}
