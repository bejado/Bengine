#include "SpaceShooter.h"
#include <PrecompiledHeader.h>

#include "ObjMesh.h"
#include "GameUtils.h"
#include "ParticleSystemLoader.h"
#include "Primitives.h"
#include "WireframeMaterial.h"

namespace ITP485
{
	const float PLAYER_ACCELERATION_FACTOR = 50.f;
	const float PLAYER_ROTATION_FACTOR = 2.f;
	const float PLAYER_FRICTION_FACTOR = 0.5f;

	SpaceShooter::SpaceShooter() : mPlayerTranslation( 0.f, 0.f, 0.f ),
								   mPlayerVelocity( 0.f, 0.f, 0.f ),
								   mPlayerAcceleration( 0.f, 0.f, 0.f ),
								   mPlayerRotation( Quaternion::Identity ),
								   mAsteroidRotation( Quaternion::Identity )
	{
		Renderer::Get().Initialize();

		// Create the player's view.
		Quaternion viewRotation( Vector3::Right, Pi / 2.f );
		mCamera = ViewPtr( new View( Vector3( 0.f, 60.f, 0.f ), viewRotation, 1.04719755f, 1920.0f / 1080.0f, 0.1f, 70.f, false ) );
		Renderer::Get().SetCamera( mCamera );

		// Load the player's ship.
		MaterialPtr playerShipMaterial = MaterialPtr( new Material( L"Resources\\Shaders\\tangent.hlsl", L"Resources\\Textures\\frigate.dds" ) );
		player = MeshPrimitivePtr( new ObjMeshPrimitive( "Resources\\Meshes\\frigate_normal.obj", playerShipMaterial ) );
		player->SetScale( 0.3f );
		mPlayerObject = GameObjectPtr( new GameObject( player ) );
		mPlayerObject->Attach();

		// Load an asteroid
		MaterialPtr asteroidMaterial = MaterialPtr( new Material( L"Resources\\Shaders\\tangent.hlsl", L"Resources\\Textures\\asteroid.dds" ) );
		asteroid = MeshPrimitivePtr( new ObjMeshPrimitive( "Resources\\Meshes\\asteroid1.obj", asteroidMaterial ) );
		asteroid->SetScale( 0.1f );
		Renderer::Get().AddPrimitive( asteroid );

		// Load the player's jet particles.
		playerJetParticles = ParticleSystemPtr( new ParticleSystem() );
		ParticleSystemLoader loader;
		loader.LoadFromFile( "Resources\\ParticleSystems\\jet_fuel.part", playerJetParticles );
		playerJetParticles->SetEmitterState( false );	// turn the particles off to start with
		Renderer::Get().AddTranslucentPrimitive( playerJetParticles );

		// Create a debug sphere
		MaterialPtr sphereMaterial = MaterialPtr( new WireframeMaterial() );
		sphere = MeshPrimitivePtr( new SpherePrimitive( sphereMaterial ) );
		sphere->SetScale( 6.5f );
		Renderer::Get().AddPrimitive( sphere );
	}

	void SpaceShooter::UpdatePlayerShip()
	{
		float deltaTime = Timing::Get().GetDeltaTime();

		if ( InputManager::Get().GetKeyState( Key::W ) )
		{
			Vector3 accelerationDirection = Vector3::Forward;
			accelerationDirection.Rotate( mPlayerRotation );
			mPlayerAcceleration = accelerationDirection * PLAYER_ACCELERATION_FACTOR;
			playerJetParticles->SetEmitterState( true );
		}
		else
		{
			mPlayerAcceleration = Vector3( 0.f, 0.f, 0.f );
			playerJetParticles->SetEmitterState( false );
		}

		if ( InputManager::Get().GetKeyState( Key::A ) )
		{
			Quaternion rotateLeft( Vector3::Up, -PLAYER_ROTATION_FACTOR * deltaTime );
			mPlayerRotation.Multiply( rotateLeft );
		}
		if ( InputManager::Get().GetKeyState( Key::D ) )
		{
			Quaternion rotateLeft( Vector3::Up, PLAYER_ROTATION_FACTOR * deltaTime );
			mPlayerRotation.Multiply( rotateLeft );
		}

		// Player ship physics.
		mPlayerVelocity = mPlayerVelocity + ( mPlayerAcceleration - mPlayerVelocity * PLAYER_FRICTION_FACTOR ) * deltaTime;
		mPlayerTranslation = mPlayerTranslation + mPlayerVelocity * deltaTime;

		// Update player's ship.
		sphere->SetTranslation( mPlayerTranslation );
		player->SetTranslation( mPlayerTranslation );
		Quaternion finalRotation = mPlayerRotation;
		finalRotation.Multiply( Quaternion( Vector3::Up, Pi / 2.f ) );
		player->SetRotation( finalRotation );

		Matrix4 jetFuelTranspose = Matrix4::Identity;

		Matrix4 jetFuelOffset;
		jetFuelOffset.CreateTranslation( Vector3(0.f, 0.f, -5.5f) );

		Matrix4 jetFuelRotation;
		jetFuelRotation.CreateFromQuaternion( mPlayerRotation );

		Matrix4 jetFuelTranslation;
		jetFuelTranslation.CreateTranslation( mPlayerTranslation );

		jetFuelTranspose.Multiply( jetFuelTranslation );
		jetFuelTranspose.Multiply( jetFuelRotation );
		jetFuelTranspose.Multiply( jetFuelOffset );

		playerJetParticles->SetTranslationMatrix( jetFuelTranspose );
	}

	void SpaceShooter::UpdateAsteroids()
	{
		float deltaTime = Timing::Get().GetDeltaTime();
		Quaternion asteroidRotation = Quaternion::FromEulerAngles( deltaTime, deltaTime, 0.f );
		mAsteroidRotation.Multiply( asteroidRotation );
		asteroid->SetRotation( mAsteroidRotation );
	}

	void SpaceShooter::Update()
	{
		ExitWithEscapeKey();

		UpdatePlayerShip();
		UpdateAsteroids();

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
