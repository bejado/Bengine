#include <mongoose.h>

namespace ITP485
{

	static const char *s_http_port = "8000";

	class MessageManager : public Singleton < MessageManager >
	{
	public:

		struct Message
		{
			char data[256];
		};

		DECLARE_SINGLETON( MessageManager );

		void Initialize();
		void Shutdown();
		void Poll();
		bool PeekMessage( Message& msg );

	};

}
