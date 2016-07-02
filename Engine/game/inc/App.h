#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include "ParticleEmitterMessageHandler.h"
#include "ParticleSystemMessageHandler.h"

#include "MeshPrimitive.h"
#include "Cube.h"
#include "ObjMesh.h"

namespace ITP485
{

	class App
	{
	public:

		App();
		void Update();
		void Render();
		void HandleMessage( const json& msg );

	private:

		DepthStencilPtr		mDepthStencilView;
		DepthStencilStatePtr mDepthStencilState;

		ViewPtr mCamera;

		MeshPrimitivePtr player;

		float mCameraMoveSpeed = 5.0f;
		float mX;
	};

	typedef shared_ptr< App > AppPtr;

}