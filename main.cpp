

#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include <tchar.h>
#include <process.h>
#include <gdiplus.h>
#include <stdio.h>

#include "pugixml.hpp"
#include "curl/curl.h"
#include "tray.h"

using namespace std;
using namespace pugi;
using namespace Gdiplus;

static string buffer;


VOID WINAPI Sleep(DWORD dwMilliseconds);

inline static int string_writer( char * data, size_t size, size_t nmemb, string * buffer ) {
    int result = 0;
    if( buffer != NULL ) {
        buffer->append( data, size * nmemb );
        result = size * nmemb;
    }
    return result;
}

inline static char * pobierzStrone( char * strona ) {
    bool byl_problem = false;

    do {
        buffer.reserve( 10000 );
        buffer.clear();
        CURL * curl;
        CURLcode result;

        curl = curl_easy_init();
        if( curl ) {
            curl_easy_setopt( curl, CURLOPT_URL, strona );
            curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, string_writer );
            curl_easy_setopt( curl, CURLOPT_WRITEDATA, & buffer );

            result = curl_easy_perform( curl );
            if( result == CURLE_OK )
                 byl_problem = false;
            else {
                cout << "!Wystapil blad podczas pobierania strony, powtarzam czynnosc!\n";
                byl_problem = true;
            }
            curl_easy_cleanup( curl );
        }
    } while( byl_problem );

    long long N = buffer.length();
    char * out = new char[ N + 1 ];
    ::copy( buffer.c_str(),( buffer.c_str() + N - 1 ), out );
    out[ N - 1 ] = '\0';
    return out;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void zapis(const char * url){
            CURL *curl;
            FILE *fp;
            CURLcode res;
            char outfilename[FILENAME_MAX] = "wall.jpg";
            curl = curl_easy_init();
            if (curl) {
                fp = fopen(outfilename,"wb");
                curl_easy_setopt(curl, CURLOPT_URL, url);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
                res = curl_easy_perform(curl);
                /* always cleanup */
                curl_easy_cleanup(curl);
                fclose(fp);
    }
}



/*=====================================================================================================*/
string first;
string strona = "http://konachan.net/post.xml?limit=1&tags=rating%3Asafe";
HINSTANCE hInst;



int ustawWalla(char *a, string b){
xml_document doc;

    if (doc.load(a))
    {
    xml_node tools = doc.child("posts").child("post");
        for (pugi::xml_attribute attr = tools.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(),"jpeg_url")==0){
                    string odwolanie = attr.value();
                    if (odwolanie != b){
                        first = attr.value();
                        zapis(attr.value());
                        int ustaw = SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0 , (PVOID)"wall.jpg", SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
                        if (ustaw)
                        {
                        cout << "works\n";
                        Sleep(5000);
                        }
                        else
                        {
                        cout << "didnt work\n";
                        Sleep(5000);
                        }
                    }
                    else
                        {
                            cout <<"no new images\n";
                            Sleep(5000);
                    }
            }
        }

    }
    return 1;
}


void __cdecl drugiWatek(void * Args){
    char *temp = new char[strona.size() +1];
    strcpy(temp,strona.c_str());

    char * pobrane = pobierzStrone(temp);
    strcat(pobrane,">");
    int plecy;

        while(true){

                    char *temp = new char[strona.size() +1];
                    strcpy(temp,strona.c_str());
                    plecy = ustawWalla(pobrane,first);
                    pobrane = pobierzStrone(temp);
                    strcat(pobrane,">");
                    cout << "sec thread will end\n";
                    Sleep(10);
        }
}

void getImage();

/*========================================================================================================*/


LPSTR NazwaKlasy = "Class";
MSG Komunikat;
HBITMAP g_Logo;
HWND g_hCheckboxRandom;
HWND g_hCheckboxLast;
HWND g_hCheckbox;
HWND g_hTextT;
HWND g_hOk;


LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK About(HWND, UINT, WPARAM, LPARAM);





int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    //START GDI
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);




    // WYPE£NIANIE STRUKTURY
    WNDCLASSEX wc;

    wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE(101) );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground =( HBRUSH )( COLOR_WINDOW + 1 );
    wc.lpszMenuName = NULL;
    wc.lpszClassName = NazwaKlasy;
    wc.hIconSm = LoadIcon( hInstance, MAKEINTRESOURCE(101) );




    // REJESTROWANIE KLASY OKNA
    if( !RegisterClassEx( & wc ) )
    {
        MessageBox( NULL, "Wysoka Komisja odmawia rejestracji tego okna!", "Niestety...",
        MB_ICONEXCLAMATION | MB_OK );
        return 1;
    }

    // TWORZENIE OKNA
    HWND hwnd;

    hInst = hInstance;

    hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, NazwaKlasy, "Konachan Wallpapers", WS_OVERLAPPEDWINDOW^(WS_MINIMIZEBOX | WS_SIZEBOX),
    CW_USEDEFAULT, CW_USEDEFAULT, 504, 172, NULL, NULL, hInstance, NULL );

    if( hwnd == NULL )
    {
        MessageBox( NULL, "Window will not appear.", "Sorry", MB_ICONEXCLAMATION );
        return 1;
    }



    //GUZIKI ITP

    //g_hGroup = CreateWindowEx( 0, "BUTTON", "Random", WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_GROUPBOX,100, 100, 120, 92, hwnd, (HMENU)1, hInstance, NULL );
    //g_hCheckboxRandom = CreateWindowEx( 0, "BUTTON", "Random", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,246, 100, 100, 30, hwnd, (HMENU)3, hInstance, NULL );
    g_hOk = CreateWindowEx( 0, "BUTTON", "OK", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,380, 100, 100, 30, hwnd, (HMENU)4, hInstance, NULL );



    g_hCheckbox = CreateWindowEx( 0, "BUTTON", "Only Safe", WS_CHILD | WS_VISIBLE | BS_FLAT | BS_CHECKBOX,10, 100, 150, 30, hwnd, (HMENU)2, hInstance, NULL );
  //  g_hCheckboxLast = CreateWindowEx( 0, "BUTTON", "Last", WS_CHILD | WS_VISIBLE | BS_FLAT | BS_CHECKBOX,123, 100, 150, 30, hwnd, (HMENU)2, hInstance, NULL );
    CheckDlgButton(hwnd,2, BST_CHECKED);



    DWORD dlugosc = GetWindowTextLength( g_hTextT );
    LPSTR Bufor =( LPSTR ) GlobalAlloc( GPTR, dlugosc + 1 );
    GetWindowText( g_hTextT, Bufor, dlugosc + 1 );

    //TRAYY
    LPSTR sTip = "Konachan Wallpapers";
    NOTIFYICONDATA nid;

    nid.cbSize = sizeof( NOTIFYICONDATA );
    nid.hWnd = hwnd;
    nid.uID = ID_TRAY1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = CMSG_TRAY1;
    nid.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(101));
    lstrcpy( nid.szTip, sTip );
    Shell_NotifyIcon(NIM_ADD, & nid);


    ShowWindow( hwnd, nCmdShow );
    UpdateWindow( hwnd );



    HANDLE hSecondThread = ( HANDLE ) _beginthread(drugiWatek,0, NULL);

    // Pêtla komunikatów
    while( GetMessage( & Komunikat, NULL, 0, 0 ) )
    {
        TranslateMessage( & Komunikat );
        DispatchMessage( & Komunikat );
    }


    return Komunikat.wParam;
}

void rysujo(HDC hdc){
   Graphics graphics(hdc);
   Image    image(L"dol.db");
   graphics.DrawImage(&image,10,10,484,80);
}


// OBS£UGA ZDARZEÑ
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
HDC hdc;
PAINTSTRUCT ps;

TCHAR szH[100];
LoadString(hInst, msg, szH, 100);

switch( msg )
    {

    case WM_PAINT:
        {
            HBRUSH hbrBkgnd;
            RECT r;

            GetClientRect( hwnd, & r );
            hdc = BeginPaint( hwnd, & ps );
            hbrBkgnd = CreateSolidBrush( RGB( 255, 255, 255 ) );
            FillRect( hdc, & r, hbrBkgnd );
            rysujo(hdc);
            EndPaint( hwnd, & ps );
        }

    case CMSG_TRAY1:
        {
            if( wParam == ID_TRAY1 )
                if( lParam == WM_LBUTTONDOWN )
                    ShowWindow( hwnd, SW_RESTORE );

            if (lParam == WM_RBUTTONDOWN)
                {
                HMENU popupMenu = CreatePopupMenu();
                AppendMenu(popupMenu, MF_STRING, IDM_ABOUT, "About");
                AppendMenuW(popupMenu, MF_SEPARATOR, 0, NULL);
                AppendMenu(popupMenu, MF_STRING, IDM_QUIT, "Exit");

                POINT ptCursor;
                GetCursorPos(&ptCursor);
                TrackPopupMenuEx(popupMenu, TPM_RIGHTBUTTON, ptCursor.x, ptCursor.y, hwnd,0);
                DestroyMenu(popupMenu);
            }
        }
    break;

    case WM_CTLCOLORSTATIC:
        {
            SetBkMode((HDC)wParam, TRANSPARENT);
            return (LRESULT)::GetStockObject(NULL_BRUSH);
        }

    case WM_INITDIALOG:
        {
            SetClassLong( hwnd, GCL_HICON,( LONG ) LoadIcon( GetModuleHandle( NULL ), MAKEINTRESOURCE( 3 ) ) );
        }

    case WM_COMMAND:
        {
            if(( HWND ) lParam == g_hCheckbox )
            {
                if (IsDlgButtonChecked(hwnd, 2)==BST_UNCHECKED)
                {
                    CheckDlgButton(hwnd,2, BST_CHECKED);
                    strona= "http://konachan.net/post.xml?limit=1&tags=rating%3Asafe";
                }
                else
                {
                    CheckDlgButton(hwnd,2, BST_UNCHECKED);
                    strona = "http://konachan.net/post.xml?limit=1";
                    cout <<strona;
                }
            }

            if ((HWND) lParam == g_hOk)
            {
                ShowWindow(hwnd,SW_HIDE);
            }

            if (LOWORD(wParam)==IDM_QUIT)
            {
                DestroyWindow(hwnd);
            }

            if (LOWORD(wParam)==IDM_ABOUT)
            {
                DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(0x8002), hwnd, (DLGPROC)About);
            }
        }
        break;

    case WM_CLOSE:
        ShowWindow(hwnd,SW_HIDE);
        break;

    case WM_DESTROY:
        NOTIFYICONDATA nid;
        nid.cbSize = sizeof( NOTIFYICONDATA );
        nid.hWnd = hwnd;
        nid.uID = ID_TRAY1;
        nid.uFlags = 0;

        Shell_NotifyIcon( NIM_DELETE, & nid );

        PostQuitMessage( 0 );
        break;

        default:
        return DefWindowProc( hwnd, msg, wParam, lParam );
    }

    return 0;
}

LRESULT CALLBACK About(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
