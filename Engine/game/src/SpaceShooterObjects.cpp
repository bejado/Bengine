#include "SpaceShooterObjects.h"

namespace ITP485
{

	void Asteroid::Update()
	{
		GameObject::Update();

		// Constantly rotate the asteroid
		float deltaTime = Timing::Get().GetDeltaTime();
		Quaternion asteroidRotation = Quaternion::FromEulerAngles( deltaTime, deltaTime, 0.f );
		Rotate( asteroidRotation );
	}

}
