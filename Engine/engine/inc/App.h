#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include "ParticleEmitterMessageHandler.h"
#include "ParticleSystemMessageHandler.h"

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

		CameraPtr mCamera;

		ParticleSystemPtr mParticleSystem;
		ParticleSystemMessageHandler mParticleSystemMessageHandler;

		float mCameraMoveSpeed = 5.0f;
	};

	typedef shared_ptr< App > AppPtr;

}