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
		InputLayoutElement element( "POSITION", 0, EGFormat::EGF_R32G32B32_Float, 0 );
		InputLayoutPtr inputLayout = GraphicsDriver::Get()->CreateInputLayout( &element, 1, compiledVertexShader );

		// now set it in the graphics driver
		GraphicsDriver::Get()->SetInputLayout( inputLayout );

		// Create a solid fill rasterizer state
		// RasterizerStatePtr solidRasterizerState = GraphicsDriver::Get()->CreateRasterizerState(EFM_Solid);
		// GraphicsDriver::Get()->SetRasterizerState(solidRasterizerState);

		// create our vertex buffer
		Vector3 vertices[8];
		vertices[0].Set( 0.f, 0.f, 0.f );
		vertices[1].Set( 1.f, 0.f, 0.f );
		vertices[2].Set( 1.f, 1.f, 0.f );
		vertices[3].Set( 0.f, 1.f, 0.f );
		vertices[4].Set( 0.f, 0.f, 1.f );
		vertices[5].Set( 1.f, 0.f, 1.f );
		vertices[6].Set( 1.f, 1.f, 1.f );
		vertices[7].Set( 0.f, 1.f, 1.f );
		mVertexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( vertices, sizeof( Vector3 ) * 8, EBindflags::EBF_VertexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );

		// create our index buffer
		uint16_t indexes[36] = { 0, 3, 1, 1, 3, 2,		// front
								 0, 4, 3, 4, 7, 3,
								 1, 2, 5, 2, 6, 5,
								 5, 6, 7, 7, 4, 5,
								 3, 7, 6, 6, 2, 3,
								 0, 1, 5, 5, 4, 0 };
		mIndexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer(indexes, sizeof(uint16_t) * 36, EBindflags::EBF_IndexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable);
		GraphicsDriver::Get()->SetIndexBuffer(mIndexBuffer);

		// create our projection view matrix buffer
		GraphicsBufferPtr perCameraConstantBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( NULL, sizeof( Matrix4 ), EBindflags::EBF_ConstantBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic );
		GraphicsDriver::Get()->SetPerCameraConstantBuffer( perCameraConstantBuffer );
		GraphicsDriver::Get()->SetVSConstantBuffer( perCameraConstantBuffer, 0 );

		// create our object to world constant buffer
		mObjectToWorldBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer(NULL, sizeof(Matrix4), EBindflags::EBF_ConstantBuffer, ECPUAccessFlags::ECPUAF_CanWrite, EGraphicsBufferUsage::EGBU_Dynamic);
		GraphicsDriver::Get()->SetVSConstantBuffer(mObjectToWorldBuffer, 1);

		// set the object to world constant buffer
		Matrix4 *objectToWorld = static_cast<Matrix4*>(GraphicsDriver::Get()->MapBuffer(mObjectToWorldBuffer));
		Matrix4 translationMatrix;
		translationMatrix.CreateTranslation( Vector3( -.5, -.5, -.5 ) );
		memcpy( objectToWorld, &translationMatrix.GetTranspose(), sizeof( Matrix4 ) );
		GraphicsDriver::Get()->UnmapBuffer(mObjectToWorldBuffer);
		
		// create our camera
		mCamera = CameraPtr(new Camera(Vector3(-1, 0, -3), Quaternion::Identity, 1.04719755f, 1.333f, 1.f, 100.f));
	}

	void App::Update()
	{
		mCameraPathAmount += Timing::Get().GetDeltaTime();
		mCamera->SetPosition( cos( mCameraPathAmount ) * 5, cos( mCameraPathAmount * .5 ) * 5, sin( mCameraPathAmount ) * 5 );
		mCamera->LookAt( 0, 0, 0 );
		mCamera->UpdateConstants();
	}

	void App::Render()
	{
		// Set vertex buffer
		GraphicsDriver::Get()->SetVertexBuffer( mVertexBuffer, sizeof( Vector3 ) );

		// Set vertex shader
		GraphicsDriver::Get()->SetVertexShader( mVertexShader );

		// Set pixel shader
		GraphicsDriver::Get()->SetPixelShader( mPixelShader );

		// Draw!
		GraphicsDriver::Get()->DrawIndexed( 36, 0, 0 );
	}

}