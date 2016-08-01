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
		virtual void Render() = 0;
		virtual void HandleMessage( const json& msg ) = 0;
	};

	typedef shared_ptr< Game > GamePtr;

}
