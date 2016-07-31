#include "SpaceShooter.h"
#include <PrecompiledHeader.h>

#include "ObjMesh.h"
#include "ParticleSystemLoader.h"
#include "Primitives.h"
#include "WireframeMaterial.h"

namespace ITP485
{

	SpaceShooter::SpaceShooter()
	{
		Renderer::Get().Initialize();

		// Create the player's view.
		Quaternion viewRotation( Vector3::Right, Pi / 2.f );
		mCamera = ViewPtr( new View( Vector3( 0.f, 60.f, 0.f ), viewRotation, 1.04719755f, 1920.0f / 1080.0f, 0.1f, 70.f, false ) );
		Renderer::Get().SetCamera( mCamera );

		// Load the player's ship.
		MaterialPtr playerShipMaterial = MaterialPtr( new Material( L"Resources\\Shaders\\tangent.hlsl", L"Resources\\Textures\\frigate.dds" ) );
		MeshPrimitivePtr player = MeshPrimitivePtr( new ObjMeshPrimitive( "Resources\\Meshes\\frigate_normal.obj", playerShipMaterial ) );
		mPlayerObject = PlayerShipPtr( new PlayerShip( player ) );
		mPlayerObject->SetScale( 0.3f );
		mPlayerObject->SetBounds( 6.5f );
		Matrix4 playerOffset;
		playerOffset.CreateFromQuaternion( Quaternion( Vector3::Up, Pi / 2.f ) );
		mPlayerObject->SetOffset( playerOffset );
		mPlayerObject->SetFrictionFactor( 0.5f );
		mPlayerObject->Attach();

		// Load an asteroid
		MaterialPtr asteroidMaterial = MaterialPtr( new Material( L"Resources\\Shaders\\tangent.hlsl", L"Resources\\Textures\\asteroid.dds" ) );
		MeshPrimitivePtr asteroid = MeshPrimitivePtr( new ObjMeshPrimitive( "Resources\\Meshes\\asteroid1.obj", asteroidMaterial ) );
		mAsteroidObject = GameObjectPtr( new Asteroid( asteroid ) );
		mAsteroidObject->SetScale( 1.f );
		mAsteroidObject->SetBounds( 4.f );
		mAsteroidObject->Attach();

		// Load the player's jet particles.
		playerJetParticles = ParticleSystemPtr( new ParticleSystem() );
		ParticleSystemLoader loader;
		loader.LoadFromFile( "Resources\\ParticleSystems\\jet_fuel.part", playerJetParticles );
		playerJetParticles->SetEmitterState( false );	// turn the particles off to start with
		Renderer::Get().AddTranslucentPrimitive( playerJetParticles );
	}

	void SpaceShooter::UpdatePlayerShip()
	{
		if ( InputManager::Get().GetKeyState( Key::W ) )
		{
			mPlayerObject->MoveForward();
			playerJetParticles->SetEmitterState( true );
		}
		else
		{
			mPlayerObject->StopAcceleration();
			playerJetParticles->SetEmitterState( false );
		}

		if ( InputManager::Get().GetKeyState( Key::A ) )
		{
			mPlayerObject->RotateLeft();
		}
		if ( InputManager::Get().GetKeyState( Key::D ) )
		{
			mPlayerObject->RotateRight();
		}

		// Update player's particles
		Matrix4 jetFuelTranspose = Matrix4::Identity;

		Matrix4 jetFuelOffset;
		jetFuelOffset.CreateTranslation( Vector3(0.f, 0.f, -5.5f) );

		Matrix4 jetFuelRotation;
		jetFuelRotation.CreateFromQuaternion( mPlayerObject->GetRotation() );

		Matrix4 jetFuelTranslation;
		jetFuelTranslation.CreateTranslation( mPlayerObject->GetTranslation() );

		jetFuelTranspose.Multiply( jetFuelTranslation );
		jetFuelTranspose.Multiply( jetFuelRotation );
		jetFuelTranspose.Multiply( jetFuelOffset );

		playerJetParticles->SetTranslationMatrix( jetFuelTranspose );
	}

	void SpaceShooter::Update()
	{
		Game::Update();

		UpdatePlayerShip();

		mPlayerObject->Update();
		mAsteroidObject->Update();

		// Update player's particles.
		playerJetParticles->Update();
	}

	void SpaceShooter::Render()
	{
		Renderer::Get().Render();
	}

	void SpaceShooter::HandleMessage( const json& msg )
	{
	}

}
