#include "Game.h"
#include "MeshPrimitive.h"
#include "ParticleSystem.h"

namespace ITP485
{

	class SpaceShooter : public Game
	{
	public:
		
		DECLARE_ALIGNED_NEW_DELETE

		SpaceShooter();
		virtual void Update();
		virtual void Render();
		virtual void HandleMessage( const json& msg );

	private:

		void UpdatePlayerShip();
		void UpdateAsteroids();

		ViewPtr mCamera;

		MeshPrimitivePtr player;
		MeshPrimitivePtr asteroid;

		ParticleSystemPtr playerJetParticles;

		Vector3 mPlayerTranslation;
		Quaternion mPlayerRotation;
		Vector3 mPlayerVelocity;
		Vector3 mPlayerAcceleration;

		Quaternion mAsteroidRotation;
	};

}