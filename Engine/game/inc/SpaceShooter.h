#include "Game.h"
#include "MeshPrimitive.h"

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

		ViewPtr mCamera;
		MeshPrimitivePtr player;

		Vector3 mPlayerTranslation;
		Quaternion mPlayerRotation;
		Vector3 mPlayerVelocity;
		Vector3 mPlayerAcceleration;
	};

}