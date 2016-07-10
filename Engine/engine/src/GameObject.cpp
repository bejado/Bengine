#include "GameObject.h"
#include <PrecompiledHeader.h>

namespace ITP485
{
	void GameObject::Attach() const
	{
		Renderer::Get().AddPrimitive( mRenderPrimitive );
	}

}
