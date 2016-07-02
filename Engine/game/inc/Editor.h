#include "Game.h"

#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include "ParticleEmitterMessageHandler.h"
#include "ParticleSystemMessageHandler.h"

#include "MeshPrimitive.h"
#include "Cube.h"
#include "ObjMesh.h"

namespace ITP485
{

	class Editor : public Game
	{
	public:

		Editor();
		virtual void Update();
		virtual void Render();
		virtual void HandleMessage( const json& msg );

	private:

		ViewPtr mCamera;

		MeshPrimitivePtr player;

		float mCameraMoveSpeed = 5.0f;
		float mX;
	};

}