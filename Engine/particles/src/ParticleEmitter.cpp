#include <PrecompiledHeader.h>
#include "ParticleEmitter.h"

namespace ITP485
{

	ParticleEmitter::ParticleEmitter()
	{
		mParticleQuad = MeshPtr( new Quad() );
		mMaterial = MaterialPtr( new Material( L"Shaders\\texture.hlsl", L"Textures\\crate.dds" ) );

		for ( int x = -50; x < 50; x++ )
		{
			for ( int y = -50; y < 50; y++ ) {
				Particle particle;
				particle.position = Vector3( x, y, 0.f );
				mParticles.push_back( particle );
			}
		}
	}

	void ParticleEmitter::Update()
	{

	}

	void ParticleEmitter::Render( const Vector3& viewPosition )
	{
		mMaterial->ActivateMaterial();
		mParticleQuad->BindBuffers();
		for ( const auto& particle : mParticles )
		{
			Renderer::Get().SetObjectToWorldMatrix( ParticleEmitter::CreateBillboardMatrix( particle.position, viewPosition ) );
			mParticleQuad->Draw();
		}
	}

	Matrix4 ParticleEmitter::CreateBillboardMatrix( const Vector3& anchor, const Vector3& cameraPosition )
	{
		Vector3 normalVector = anchor - cameraPosition;
		normalVector.Normalize();
		Vector3 rightVector = Cross( Vector3::Up, normalVector );
		Vector3 upVector = Cross( normalVector, rightVector );

		Matrix4 rotation;
		rotation.CreateCoordinateFrameTransform( rightVector, upVector, normalVector );
		Matrix4 translation;
		translation.CreateTranslation( anchor );

		translation.Multiply( rotation );

		return translation;
	}

}
