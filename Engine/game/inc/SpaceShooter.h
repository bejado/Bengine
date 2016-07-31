#include "Game.h"
#include "MeshPrimitive.h"
#include "ParticleSystem.h"
#include "GameObject.h"
#include "SpaceShooterObjects.h"

namespace ITP485
{

	class SpaceShooter : public Game
	{
	public:
		
		DECLARE_ALIGNED_NEW_DELETE

		SpaceShooter();
		virtual void Update() override;
		virtual void Render();
		virtual void HandleMessage( const json& msg );

	private:

		void UpdatePlayerShip();

		ViewPtr mCamera;

		PlayerShipPtr mPlayerObject;
		GameObjectPtr mAsteroidObject;
		ParticleSystemPtr playerJetParticles;

	};

}