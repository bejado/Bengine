#include <PrecompiledHeader.h>

namespace BNG
{

	/**
	Base class for the high level game code.
	*/
	class Game
	{
	public:

		virtual void Update();
		virtual void Render();
		virtual void HandleMessage( const json& msg );

	};

	typedef shared_ptr< Game > GamePtr;

}
