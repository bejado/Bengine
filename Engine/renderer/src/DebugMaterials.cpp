#include "WireframeMaterial.h"

namespace BNG
{
	WireframeMaterial::WireframeMaterial() : Material( L"Resources\\Shaders\\wireframe.hlsl", L"" ),
											 mWireframeRasterizerState( GraphicsDriver::Get()->CreateRasterizerState( EFM_Wireframe, ECM_Back ) ),
											 mDefaultRasterizerState( GraphicsDriver::Get()->CreateRasterizerState( EFM_Solid, ECM_Back ) )		// TODO: have an implicit set of defaults
	{
	}

	void WireframeMaterial::ActivateMaterial() const 
	{
		Material::ActivateMaterial();
		GraphicsDriver::Get()->SetRasterizerState( mWireframeRasterizerState );
	}

	void WireframeMaterial::DeactivateMaterial() const
	{
		GraphicsDriver::Get()->SetRasterizerState( mDefaultRasterizerState );
	}

}
