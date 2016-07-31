#pragma once

#include "GameObject.h"

namespace ITP485
{

	class Asteroid : public GameObject
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		Asteroid( MeshPrimitivePtr renderPrimitive ) : GameObject( renderPrimitive ) {}
		virtual void Update() override;

	private:

	};

	const float PLAYER_ACCELERATION_FACTOR = 50.f;
	const float PLAYER_ROTATION_FACTOR = 2.f;

	class PlayerShip : public GameObject
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		PlayerShip( MeshPrimitivePtr renderPrimitive ) : GameObject( renderPrimitive ) {}
		virtual void Update() override;
		void MoveForward();
		void StopAcceleration();
		void RotateLeft();
		void RotateRight();

	private:

	};

	typedef shared_ptr< PlayerShip > PlayerShipPtr;

}