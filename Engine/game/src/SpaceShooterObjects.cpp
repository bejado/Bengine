#include "SpaceShooterObjects.h"

namespace BNG
{

	Asteroid::Asteroid( MeshPrimitivePtr renderPrimitive ) : GameObject( renderPrimitive )
	{
		Quaternion randomRotation = Quaternion::FromEulerAngles( RandomFloat( 0.f, 100.f ), RandomFloat( 0.f, 100.f ), RandomFloat( 0.f, 100.f ) );
		Rotate( randomRotation );
	}

	void Asteroid::Update()
	{
		GameObject::Update();

		// Constantly rotate the asteroid
		float deltaTime = Timing::Get().GetDeltaTime();
		Quaternion asteroidRotation = Quaternion::FromEulerAngles( deltaTime, deltaTime, 0.f );
		Rotate( asteroidRotation );
	}

	void PlayerShip::MoveForward()
	{
		Vector3 accelerationDirection = Vector3::Forward;
		accelerationDirection.Rotate( GetRotation() );
		SetAcceleration( accelerationDirection * PLAYER_ACCELERATION_FACTOR );
	}

	void PlayerShip::StopAcceleration()
	{
		SetAcceleration( Vector3( 0.f, 0.f, 0.f ) );
	}

	void PlayerShip::RotateLeft()
	{
		float deltaTime = Timing::Get().GetDeltaTime();
		Quaternion rotateLeft( Vector3::Up, -PLAYER_ROTATION_FACTOR * deltaTime );
		Rotate( rotateLeft );
	}

	void PlayerShip::RotateRight()
	{
		float deltaTime = Timing::Get().GetDeltaTime();
		Quaternion rotateRight( Vector3::Up, PLAYER_ROTATION_FACTOR * deltaTime );
		Rotate( rotateRight );
	}

	void PlayerShip::Update()
	{
		GameObject::Update();
	}

}
