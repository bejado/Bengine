#include <PrecompiledHeader.h>
#include "ParticleEmitter.h"

namespace ITP485
{

	ParticleEmitter::ParticleEmitter()
	{
		mParticleQuad = QuadPtr( new Quad() );
		mMaterial = MaterialPtr( new Material( L"Shaders\\texture.hlsl", L"Textures\\cage.dds" ) );

		int i = 0;
		for ( int x = -5; x < 5; x++ )
		{
			for ( int y = -5; y < 5; y++ )
			{
				for ( int z = -5; z < 5; z++ )
				{
					mParticles[i++].position = Vector3( (float) x * 2.f, (float) y * 2.f, (float) z * 2.f);
				}
			}
		}
	}

	void ParticleEmitter::Update()
	{
		mTime += Timing::Get().GetDeltaTime() / 10.f;
		Vector3 *perInstanceData = mParticleQuad->MapInstanceBuffer();
		memcpy( perInstanceData, mParticles, sizeof( Particle ) * 1000 );
		mParticleQuad->UnmapInstanceBuffer();
	}

	void ParticleEmitter::Render( const Vector3& viewPosition )
	{
		mMaterial->ActivateMaterial();
		mParticleQuad->BindContext();
		mParticleQuad->DrawInstanced( 1000 );
	}
}
