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

	class PlayerShip : public GameObject
	{
	public:

		DECLARE_ALIGNED_NEW_DELETE

		PlayerShip( MeshPrimitivePtr renderPrimitive ) : GameObject( renderPrimitive ) {}

	private:

	};

}