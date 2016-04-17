#include <PrecompiledHeader.h>

namespace ITP485
{

	App::App()
	{
		// let's make some shaders! Here's the code from lecture to load up the vertex shader in App3_1.hlsl
		vector< char > compiledVertexShader;
		ITP485::GraphicsDriver::Get()->CompileShaderFromFile( L"Shaders\\shader.hlsl", "VS", "vs_4_0", compiledVertexShader );
		mVertexShader = GraphicsDriver::Get()->CreateVertexShader( compiledVertexShader );

		// now load up the pixel shader named PS
		vector< char > compiledPixelShader;
		GraphicsDriver::Get()->CompileShaderFromFile( L"Shaders\\shader.hlsl", "PS", "ps_4_0", compiledPixelShader );
		mPixelShader = GraphicsDriver::Get()->CreatePixelShader( compiledPixelShader );

		// now create an input layout to describe vertices that contain 3 floats for position data and nothing else
		InputLayoutElement elements[3]{{ "POSITION", 0, EGFormat::EGF_R32G32B32_Float, 0 }, { "NORMAL", 0, EGFormat::EGF_R32G32B32_Float, sizeof( float ) * 3 }, { "TEXCOORD", 0, EGFormat::EGF_R32G32_Float, sizeof( float ) * 6 }};
		InputLayoutPtr inputLayout = GraphicsDriver::Get()->CreateInputLayout( elements, 3, compiledVertexShader );

		// now set it in the graphics driver
		GraphicsDriver::Get()->SetInputLayout( inputLayout );

		// Create a solid fill rasterizer state
		RasterizerStatePtr solidRasterizerState = GraphicsDriver::Get()->CreateRasterizerState(EFM_Solid);
		GraphicsDriver::Get()->SetRasterizerState(solidRasterizerState);

		// create our projection view matrix buffer
		GraphicsBufferPtr perCameraConstantBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( NULL, sizeof( Matrix4 ), EBindflags::EBF_ConstantBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic );
		GraphicsDriver::Get()->SetPerCameraConstantBuffer( perCameraConstantBuffer );
		GraphicsDriver::Get()->SetVSConstantBuffer( perCameraConstantBuffer, 0 );

		// create our object to world constant buffer
		mObjectToWorldBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer(NULL, sizeof(Matrix4), EBindflags::EBF_ConstantBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic);
		GraphicsDriver::Get()->SetVSConstantBuffer(mObjectToWorldBuffer, 1);

		// create our sampler state

		// TexturePtr texture = GraphicsDriver::Get()->CreateTextureFromFile(widestr.c_str());
		// SamplerStatePtr samplerState = GraphicsDriver::Get()->CreateSamplerState();
		// GraphicsDriver::Get()->SetPSSamplerState(samplerState, 0);
		
		// create our camera
		mCamera = CameraPtr(new Camera(Vector3(-1, 0, -3), Quaternion::Identity, 1.04719755f, 1.333f, 1.f, 100.f));

		// create the cubes
		mCube = CubePtr( new Cube( -.5f, -.5f, -.5f ) );
	}

	void App::Update()
	{
		mCameraPathAmount += Timing::Get().GetDeltaTime();
		mCamera->SetPosition( cos( mCameraPathAmount ) * 5.f, cos( mCameraPathAmount * .25f ) * 5.f, sin( mCameraPathAmount ) * 5.f );
		mCamera->LookAt( 0, 0, 0 );
		mCamera->UpdateConstants();
	}

	void App::Render()
	{
		// Set vertex shader
		GraphicsDriver::Get()->SetVertexShader( mVertexShader );

		// Set pixel shader
		GraphicsDriver::Get()->SetPixelShader( mPixelShader );

		mCube->UpdateObjectWorldBuffer( mObjectToWorldBuffer );
		mCube->Draw();
	}

}