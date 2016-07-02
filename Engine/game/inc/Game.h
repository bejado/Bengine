#include <PrecompiledHeader.h>

namespace ITP485
{

	/**
	Base class for the high level game code.
	*/
	class Game
	{
	public:

		virtual void Update() = 0;
		virtual void Render() = 0;
		virtual void HandleMessage( const json& msg ) = 0;
	};

	typedef shared_ptr< Game > GamePtr;

}
