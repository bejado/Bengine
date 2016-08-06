#include "Game.h"
#include "MeshPrimitive.h"
#include "ParticleSystem.h"
#include "GameObject.h"
#include "SpaceShooterObjects.h"

namespace BNG
{

	class SpaceShooter : public Game
	{
	public:
		
		DECLARE_ALIGNED_NEW_DELETE

		SpaceShooter();
		virtual void Update() override;

	private:

		void UpdatePlayerShip();

		ViewPtr mCamera;

		PlayerShipPtr mPlayerObject;
		std::vector<GameObjectPtr> mAsteroids;
		ParticleSystemPtr playerJetParticles;

	};

}