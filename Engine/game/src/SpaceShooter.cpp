#include "SpaceShooter.h"
#include <PrecompiledHeader.h>

#include "ObjMesh.h"
#include "GameUtils.h"
#include "ParticleSystemLoader.h"

namespace ITP485
{
	const float PLAYER_ACCELERATION_FACTOR = 50.f;
	const float PLAYER_ROTATION_FACTOR = 2.f;
	const float PLAYER_FRICTION_FACTOR = 0.5f;

	SpaceShooter::SpaceShooter() : mPlayerTranslation( 0.f, 0.f, 0.f ),
								   mPlayerVelocity( 0.f, 0.f, 0.f ),
								   mPlayerAcceleration( 0.f, 0.f, 0.f ),
								   mPlayerRotation(Quaternion::Identity)
	{
		Renderer::Get().Initialize();

		// Create the player's view.
		Quaternion viewRotation( Vector3::Right, Pi / 2.f );
		// Quaternion viewRotation(Quaternion::Identity);
		mCamera = ViewPtr( new View( Vector3( 0.f, 60.f, 0.f ), viewRotation, 1.04719755f, 1920.0f / 1080.0f, 0.1f, 70.f, false ) );
		Renderer::Get().SetCamera( mCamera );

		// Load the player's ship.
		player = MeshPrimitivePtr( new ObjMeshPrimitive( "Resources\\Meshes\\frigate_normal.obj" ) );
		player->SetScale( 0.3f );
		Renderer::Get().AddPrimitive( player );

		// Load the player's jet particles.
		playerJetParticles = ParticleSystemPtr( new ParticleSystem() );
		ParticleSystemLoader loader;
		loader.LoadFromFile( "Resources\\ParticleSystems\\jet_fuel.part", playerJetParticles );
		playerJetParticles->SetEmitterState( false );	// turn the particles off to start with
		Renderer::Get().AddTranslucentPrimitive( playerJetParticles );
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

	void SpaceShooter::Update()
	{
		ExitWithEscapeKey();

		UpdatePlayerShip();

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
