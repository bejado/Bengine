#include <PrecompiledHeader.h>

namespace BNG
{

	class WireframeMaterial : public Material
	{
	public:

		WireframeMaterial();
		virtual void ActivateMaterial() const override;
		virtual void DeactivateMaterial() const override;

	private:

		RasterizerStatePtr mWireframeRasterizerState;
		RasterizerStatePtr mDefaultRasterizerState;

	};

}