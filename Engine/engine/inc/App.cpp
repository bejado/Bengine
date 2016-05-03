#include <PrecompiledHeader.h>

namespace ITP485
{
	const float CAMERA_MIN_SPEED = 1.f;
	const float MOUSE_WHEEL_SCALE = 1.f / 100.f;
	const float MOUSE_ROTATION_SCALE = 1.f / 300.f;

	App::App()
	{
		Renderer::Get().Initialize();

		// let's make some shaders! Here's the code from lecture to load up the vertex shader in App3_1.hlsl
		vector< char > compiledVertexShader;
		ITP485::GraphicsDriver::Get()->CompileShaderFromFile( L"Shaders\\red.hlsl", "VS", "vs_4_0", compiledVertexShader );
		mVertexShader = GraphicsDriver::Get()->CreateVertexShader( compiledVertexShader );

		// now create an input layout to describe vertices that contain 3 floats for position data and nothing else
		InputLayoutElement elements[3]{{ "POSITION", 0, EGFormat::EGF_R32G32B32_Float, 0 }, { "NORMAL", 0, EGFormat::EGF_R32G32B32_Float, sizeof( float ) * 3 }, { "TEXCOORD", 0, EGFormat::EGF_R32G32_Float, sizeof( float ) * 6 }};
		InputLayoutPtr inputLayout = GraphicsDriver::Get()->CreateInputLayout( elements, 3, compiledVertexShader );
		GraphicsDriver::Get()->SetInputLayout( inputLayout );

		// Create the camera constant buffer
		GraphicsBufferPtr perCameraConstantBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( NULL, sizeof( Camera::PerCameraConstants ), EBindflags::EBF_ConstantBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic );
		GraphicsDriver::Get()->SetPerCameraConstantBuffer( perCameraConstantBuffer );
		GraphicsDriver::Get()->SetVSConstantBuffer( perCameraConstantBuffer, 0 );
		GraphicsDriver::Get()->SetPSConstantBuffer( perCameraConstantBuffer, 0 );

		// create our camera
		mCamera = CameraPtr( new Camera( Vector3( 0, 0, -5.f ), Quaternion::Identity, 1.04719755f, 1.333f, 1.f, 100.f ) );

		// Create the mesh and material
		mCubeOne = CubePtr( new Cube() );
		mCubeTwo = CubePtr( new Cube() );
		mCubeOneMaterial = MaterialPtr( new Material( L"Shaders\\red.hlsl", L"" ) );
		mCubeTwoMaterial = MaterialPtr( new Material( L"Shaders\\green.hlsl", L"" ) );

		// Set vertex shader
		GraphicsDriver::Get()->SetVertexShader( mVertexShader );
	}

	void App::Update()
	{
		// Handle mouse movement
		long mouseX = InputManager::Get().GetMouseDeltaX();
		long mouseY = InputManager::Get().GetMouseDeltaY();
		mCamera->RotateCameraFixedAxis( Vector3::Up, mouseX * MOUSE_ROTATION_SCALE );
		mCamera->RotateCameraRelativeAxis( Vector3::Right, mouseY * MOUSE_ROTATION_SCALE );

		// Handle keyboard movement
		float moveAmount = Timing::Get().GetDeltaTime() * mCameraMoveSpeed;
		if ( InputManager::Get().GetKeyState( Key::A ) )
		{
			mCamera->MoveCamera( Vector3::Left, moveAmount );
		}
		if ( InputManager::Get().GetKeyState( Key::D ) )
		{
			mCamera->MoveCamera( Vector3::Right, moveAmount );
		}
		if ( InputManager::Get().GetKeyState( Key::W ) )
		{
			mCamera->MoveCamera( Vector3::Forward, moveAmount );
		}
		if ( InputManager::Get().GetKeyState( Key::S ) )
		{
			mCamera->MoveCamera( Vector3::Backward, moveAmount );
		}

		// Adjust the camera move speed by the mouse wheel
		short wheel = InputManager::Get().GetMouseWheelDelta();
		if ( wheel != 0 ) {
			mCameraMoveSpeed += wheel * MOUSE_WHEEL_SCALE;
			if ( mCameraMoveSpeed < CAMERA_MIN_SPEED ) {
				mCameraMoveSpeed = CAMERA_MIN_SPEED;
			}
		}

		// Allow ESC to exit
		if ( InputManager::Get().GetKeyState( Key::ESC ) ) {
			PostQuitMessage( 0 );
		}

		mCamera->UpdateConstants();
	}

	void App::Render()
	{
		Renderer::Get().BeginRender();
		Renderer::Get().Submit( mCubeOne, mCubeOneMaterial, Matrix4::Identity );
		Renderer::Get().Render();
		Renderer::Get().FinishRender();
	}

}