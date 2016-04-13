#include <PrecompiledHeader.h>

namespace ITP485
{

	App::App()
	{
		// let's make some shaders! Here's the code from lecture to load up the vertex shader in App3_1.hlsl
		vector< char > compiledVertexShader;
		ITP485::GraphicsDriver::Get()->CompileShaderFromFile( L"Shaders\\App3_1.hlsl", "VS", "vs_4_0", compiledVertexShader );
		mVertexShader = GraphicsDriver::Get()->CreateVertexShader( compiledVertexShader );

		// now load up the pixel shader named PS
		vector< char > compiledPixelShader;
		GraphicsDriver::Get()->CompileShaderFromFile( L"Shaders\\App3_1.hlsl", "PS", "ps_4_0", compiledPixelShader );
		mPixelShader = GraphicsDriver::Get()->CreatePixelShader( compiledPixelShader );

		// now create an input layout to describe vertices that contain 3 floats for position data and nothing else
		InputLayoutElement element( "POSITION", 0, EGFormat::EGF_R32G32B32_Float, 0 );
		InputLayoutPtr inputLayout = GraphicsDriver::Get()->CreateInputLayout( &element, 1, compiledVertexShader );

		// now set it in the graphics driver
		GraphicsDriver::Get()->SetInputLayout( inputLayout );

		// Create a solid fill rasterizer state
		RasterizerStatePtr solidRasterizerState = GraphicsDriver::Get()->CreateRasterizerState(EFM_Solid);
		GraphicsDriver::Get()->SetRasterizerState(solidRasterizerState);

		// create our vertex buffer
		Vector3 vertices[3];
		vertices[0].Set( 0.f, 0.5f, 0.5f );
		vertices[1].Set( 0.5f, -0.5f, 0.5f );
		vertices[2].Set( -0.5f, -0.5f, 0.5f );

		mVertexBuffer = GraphicsDriver::Get()->CreateGraphicsBuffer( vertices, sizeof( Vector3 ) * 3, EBindflags::EBF_VertexBuffer, 0, EGraphicsBufferUsage::EGBU_Immutable );
	}

	void App::Update()
	{

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
		GraphicsDriver::Get()->Draw( 3, 0 );
	}

}