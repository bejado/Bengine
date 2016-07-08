#include <PrecompiledHeader.h>
#include "Quad.h"

#pragma once

namespace ITP485
{

	const uint32_t MAX_PARTICLES = 1024;

	// Per-particle data copied to instance buffer each frame
	union Particle
	{
		Particle() { memset( this, 0, sizeof( Particle ) ); }
		Particle( const Particle &obj ) { memcpy( this, &obj, sizeof( Particle ) ); }
		struct {
			Vector3 position;
			float age;
		};
		Particle* next;		// when the particle is not in use, this holds a link to the next free particle
	};

	struct EmitterConstants
	{
		Vector3 startColor;
		Vector3 endColor;
		float life;
	};

	class ParticleEmitter
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		ParticleEmitter( const Vector3& emitterPosition = Vector3( 0.f, 0.f, 0.f ),
						 const Vector3& startColor = Vector3( 1.f, 1.f, 1.f ),
						 const Vector3& endColor = Vector3( 1.f, 0.f, 0.f ) );
		void Update();
		void Render( const PrimitiveDrawer& drawer, const ViewPtr view );
		void BurstParticles( uint32_t amount );
		void SetEmitterState( bool state ) { mEmitterState = state; }

		/**
		* Sets the birth origin of new particles. Does not affect particles already living.
		*/
		void SetEmitterOrigin( const Vector3& position ) { mEmitterPosition = position; }

		/**
		* Sets the matrix used to orient all the particles in this emitter. Affects all past and future particles.
		*/
		void SetTranslationMatrix( const Matrix4& matrix ) { mTranslationMatrix = matrix; }

		// We'll delegate all message handling to a message handler class
		friend class ParticleEmitterMessageHandler;

	private:

		Particle* SpawnParticle();
		void KillParticle( Particle* particle );
		void InitParticle( Particle* particle );

		uint32_t ParticleIndex( Particle* particle );
		Particle* FindFirstParticle();
		Particle* FindNextParticle( Particle* startParticle );
		void SwapParticles( Particle* first, Particle* second );
		void DepthSort();
		void UpdateParticleConstantBuffer();

		Particle mParticles[MAX_PARTICLES];
		Vector3 mParticleVelocity[MAX_PARTICLES];

		QuadPtr mParticleQuad;
		MaterialPtr mMaterial;
		float mSpawnTimer = 0.f;

		uint32_t mAliveParticles = 0;
		Particle *mNextFreeParticle = nullptr;

		static GraphicsBufferPtr ParticleConstantBuffer;	// static, because we only need one
		EmitterConstants mEmitterConstants;
		Vector3 mEmitterPosition = Vector3( 0.f, 0.f, 0.f );
		Vector3 mGravity = Vector3( 0.f, 0.f, 0.f );
		float mInitialVelocity = 5.f;
		float mEmitterRate = 1.f;	// in particles per second
		Matrix4 mTranslationMatrix;
		bool mEmitterState;

	};

	typedef shared_ptr< ParticleEmitter > ParticleEmitterPtr;

}
