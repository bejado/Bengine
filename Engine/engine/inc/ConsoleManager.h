#include <PrecompiledHeader.h>

namespace ITP485
{
	class ConsoleManager : public Singleton < ConsoleManager >
	{
	public:

		DECLARE_SINGLETON( ConsoleManager );

		void HandleConsoleInput();
		void RenderConsole() const;


	private:

		std::wstring console;

	};

}