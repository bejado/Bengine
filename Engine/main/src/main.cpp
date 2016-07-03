#ifdef EDITOR
#include "Editor.h"
#else
#include "SpaceShooter.h"
#endif // EDITOR
#include <PrecompiledHeader.h>

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE               g_hInst = nullptr;
HWND                    g_hWnd = nullptr;

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"WindowClass";
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 1920, 1080 };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( L"WindowClass", L"Ben's Game Engine",
                           WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
                           nullptr );

	ITP485::InputManager::Get().SetWindow( g_hWnd );

	if( !g_hWnd )
	{
		return E_FAIL;
	}

    ShowWindow( g_hWnd, nCmdShow );

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )

{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

	if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
	{
		return 0;
	}

	// Initialize subsystems
	ITP485::GraphicsDriver::StaticInit( g_hWnd );
#ifdef EDITOR
	ITP485::GamePtr game = ITP485::GamePtr( new ITP485::Editor() );
#else
	ITP485::GamePtr game = ITP485::GamePtr( new ITP485::SpaceShooter() );
#endif // EDITOR
	ITP485::MessageManager::Get().Initialize();

	// Main message loop here
	MSG msg = { 0 };
	ITP485::MessageManager::Message socketMsg = { 0 };
	while ( msg.message != WM_QUIT ) {
		if ( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			// Poll for new websocket messages
			ITP485::MessageManager::Get().Poll();
			if ( ITP485::MessageManager::Get().PeekMessage( socketMsg ) )
			{
				try
				{
					auto jsonMsg = json::parse( socketMsg.data );
					game->HandleMessage( jsonMsg );
				}
				catch ( const std::exception& )
				{
					Dbg_Assert( false, "Invalid JSON message received." );
				}
			}

			Timing::Get().Update();

			game->Update();
			game->Render();

			ITP485::InputManager::Get().ResetEvents();
		}
	}

	// Shut down subsystems
	ITP485::MessageManager::Get().Shutdown();

	DestroyWindow( g_hWnd );

	return 0;
}

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM inWParam, LPARAM inLParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

	// Allow the InputManager a chance to handle the event
	if ( ITP485::InputManager::Get().HandleEvent( message, inWParam, inLParam ) )
	{
		return 0;
	}

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

		case WM_ACTIVATEAPP:
			// Hide the cursor while our app has focus
			ShowCursor( false );
			break;

        default:
			return DefWindowProc( hWnd, message, inWParam, inLParam );
    }

    return 0;
}