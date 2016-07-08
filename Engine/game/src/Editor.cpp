#include "MapLoader.h"
#include "Editor.h"
#include "ParticleSystemLoader.h"
#include <PrecompiledHeader.h>

#include "GameUtils.h"

namespace ITP485
{

	Editor::Editor()
	{
		Renderer::Get().Initialize();

		// Create the player's view.
		mCamera = ViewPtr( new View( Vector3( 0.f, 1.f, -5.f ), Quaternion::Identity, 1.04719755f, 1920.0f / 1080.0f, 0.1f, 70.f, false ) );
		Renderer::Get().SetCamera( mCamera );

		player = MeshPrimitivePtr( new ObjMeshPrimitive( "Resources\\Meshes\\fighter_normal.obj" ) );
		player->SetScale( 0.1f );
		Renderer::Get().AddPrimitive( player );
	}

	void Editor::Update()
	{
		ExitWithEscapeKey();
	}

	void Editor::Render()
	{
		Renderer::Get().Render();
	}

	void Editor::HandleMessage( const json& msg )
	{
	}

}
