#include "ParticleEmitter.h"
#include "ParticleEmitterMessageHandler.h"

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

		ParticleEmitterPtr mParticleEmitter;
		ParticleEmitterMessageHandler mParticleMessageHandler;

		float mCameraMoveSpeed = 5.0f;
	};

	typedef shared_ptr< App > AppPtr;

}