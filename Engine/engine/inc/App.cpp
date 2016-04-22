#include <PrecompiledHeader.h>

namespace ITP485
{

	App::App()
	{
		// let's make some shaders! Here's the code from lecture to load up the vertex shader in App3_1.hlsl
		vector< char > compiledVertexShader;
		ITP485::GraphicsDriver::Get()->CompileShaderFromFile( L"Shaders\\shader.hlsl", "VS", "vs_4_0", compiledVertexShader );
		mVertexShader = GraphicsDriver::Get()->CreateVertexShader( compiledVertexShader );

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
		mObjectToWorldBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( NULL, sizeof( Matrix4 ), EBindflags::EBF_ConstantBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic );
		GraphicsDriver::Get()->SetVSConstantBuffer( mObjectToWorldBuffer, 1 );

		// Set up our depth buffer and depth test
		mDepthStencilView = GraphicsDriver::Get()->CreateDepthStencil(GraphicsDriver::Get()->GetWindowWidth(), GraphicsDriver::Get()->GetWindowHeight());
		mDepthStencilState = GraphicsDriver::Get()->CreateDepthStencilState(true, EComparisonFunc::ECF_Less);
		GraphicsDriver::Get()->SetDepthStencil(mDepthStencilView);
		GraphicsDriver::Get()->SetDepthStencilState(mDepthStencilState);

		// create our camera
		mCamera = CameraPtr( new Camera( Vector3( 0, 0, 0 ), Quaternion::Identity, 1.04719755f, 1.333f, 1.f, 100.f ) );

		// Create the mesh and material
		mMesh = ObjMeshPtr( new ObjMesh( "Meshes\\fighter.obj" ) );
		mMaterial = MaterialPtr( new Material( L"Shaders\\shader.hlsl", L"Textures\\fighter.dds" ) );

		// Set object to world matrix
		Matrix4 *objectToWorld = static_cast<Matrix4*>(GraphicsDriver::Get()->MapBuffer( mObjectToWorldBuffer ));
		memcpy( objectToWorld, &Matrix4::Identity.GetTranspose(), sizeof( Matrix4 ) );
		GraphicsDriver::Get()->UnmapBuffer( mObjectToWorldBuffer );
	}

	void App::Update()
	{
		mCameraPathAmount += Timing::Get().GetDeltaTime();
		mCamera->SetPosition( cos( mCameraPathAmount ) * 30.f, cos( mCameraPathAmount * 1.25f ) * 40.f, sin( mCameraPathAmount ) * 60.f );
		mCamera->LookAt( 0, 0, 0 );
		mCamera->UpdateConstants();
	}

	void App::Render()
	{
		// Clear back buffer and depth stencil
		GraphicsDriver::Get()->ClearBackBuffer();
		GraphicsDriver::Get()->ClearDepthStencil( mDepthStencilView, 1.0f );

		// Set vertex shader
		GraphicsDriver::Get()->SetVertexShader( mVertexShader );

		mMaterial->ActivateMaterial();
		mMesh->Draw();

		// Present!
		ITP485::GraphicsDriver::Get()->Present();
	}

}