#include <PrecompiledHeader.h>

namespace ITP485
{
	static struct mg_mgr mgr;
	static struct mg_connection *nc;
	static queue<MessageManager::Message> mMessageQueue;
	static bool isConnected = false;

	int is_websocket( const struct mg_connection *nc )
	{
	  return nc->flags & MG_F_IS_WEBSOCKET;
	}

	void ev_handler( struct mg_connection *nc, int ev, void *ev_data )
	{
		switch (ev) {
			case MG_EV_WEBSOCKET_HANDSHAKE_DONE: {
				/* New websocket connection. Tell everybody. */
				isConnected = true;

				break;
			}

			case MG_EV_WEBSOCKET_FRAME: {
				/* New websocket message.  */
				struct websocket_message *wm = (struct websocket_message *) ev_data;
				struct mg_str d = {(char *) wm->data, wm->size};
				MessageManager::Message msg = { 0 };
				memcpy( msg.data, wm->data, wm->size );
				mMessageQueue.push( msg );

				break;
			}

			case MG_EV_CLOSE: {
				/* Disconnect. Tell everybody. */
				if (is_websocket(nc)) {
				  isConnected = false;
				}

				break;
			}
		}
	}

	void MessageManager::Initialize()
	{
		mg_mgr_init(&mgr, NULL);

		nc = mg_bind(&mgr, s_http_port, ev_handler);
		mg_set_protocol_http_websocket(nc);
		printf("Started on port %s\n", s_http_port);

	}

	void MessageManager::Shutdown()
	{
		mg_mgr_free( &mgr );
	}

	void MessageManager::Poll()
	{
		mg_mgr_poll(&mgr, 0);
	}

	bool MessageManager::PeekMessage( Message& msg )
	{
		if ( !mMessageQueue.empty() )
		{
			msg = mMessageQueue.front();
			mMessageQueue.pop();
			return true;
		}
		return false;
	}

}
