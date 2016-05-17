#include "ParticleEmitter.h"

namespace ITP485
{

	class App
	{
	public:

		App();
		void Update();
		void Render();
		void HandleMessage( const MessageManager::Message& msg );

	private:

		DepthStencilPtr		mDepthStencilView;
		DepthStencilStatePtr mDepthStencilState;

		CameraPtr mCamera;

		ParticleEmitterPtr mParticleEmitter;

		float mCameraMoveSpeed = 5.0f;
	};

	typedef shared_ptr< App > AppPtr;

}