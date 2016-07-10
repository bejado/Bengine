#pragma once

#include "Renderer.h"
#include <PrecompiledHeader.h>

namespace ITP485
{

	class GameObject
	{
	public:

		GameObject( RenderPrimitivePtr renderPrimitive ) : mRenderPrimitive( renderPrimitive ) {}

		void Attach() const;


	private:

		RenderPrimitivePtr mRenderPrimitive;
		float bounds;

	};

	typedef shared_ptr< GameObject > GameObjectPtr;

}