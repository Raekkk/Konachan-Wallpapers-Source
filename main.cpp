

#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include <tchar.h>
#include <process.h>

#include "pugixml.hpp"
#include "curl/curl.h"
#include "tray.h"

using namespace std;
using namespace pugi;

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

/*
int main(){
char strona[] = "http://konachan.net/post.xml?limit=1&tags=rating%3Asafe";

char * pobrane = pobierzStrone(strona);
//sprintf(a,"%s>",pobrane);
strcat(pobrane,">");
cout << pobrane << endl << endl;

const char *first = "dupsko";

for (;;){

    xml_document doc;

    if (!doc.load(pobrane)) return -1;
    {

    xml_node tools = doc.child("posts").child("post");
        for (pugi::xml_attribute attr = tools.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(),"file_url")==0){
                    cout << attr.value() << endl << endl;
                    if (!strcmp(attr.value(),first)==0){
                        zapis(attr.value());
                        first = attr.value();

                        int ustaw = SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0 , (PVOID)"wall.jpg", SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
                        if (ustaw)
                        {
                        cout << "dziala tapeta ustawiona" << endl;
                        Sleep(30000);
                        }
                        else
                        {
                        cout << "nie dziala, przykra sprawa" << endl << GetLastError();
                        }
                    }
                    else{ cout <<"DUPA" << endl;}
            }
        }

    }
}
}*/
/*=====================================================================================================*/

unsigned int g_Counter = 0;







/*=====================================================================================================*/
string first;
string strona = "http://konachan.net/post.xml?limit=1&tags=rating%3Asafe";


void ustawWalla(char *a, string b){
xml_document doc;

    if (doc.load(a))
    {
    xml_node tools = doc.child("posts").child("post");
        for (pugi::xml_attribute attr = tools.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(),"jpeg_url")==0){
                    string odwolanie = attr.value();
                    if (odwolanie != b){
                        cout <<attr.value()<<"\n"<<first <<"\n";
                        first = attr.value();
                        zapis(attr.value());

                        int ustaw = SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0 , (PVOID)"wall.jpg", SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
                        if (ustaw)
                        {
                        cout << "dziala tapeta ustawiona\n";
                        Sleep(1000);
                        }
                        else
                        {
                        cout << "nie dziala, przykra sprawa\n";
                        Sleep(1000);
                        }
                    }
                    else
                        {
                            cout <<"DUPA\n";
                            Sleep(1000);
                    }
            }
        }

    }
}


void  __cdecl drugiWatek(void * Args){
    char *temp = new char[strona.size() +1];
    strcpy(temp,strona.c_str());

    char * pobrane = pobierzStrone(temp);
    strcat(pobrane,">");

        for (;;){
        char *temp = new char[strona.size() +1];
        strcpy(temp,strona.c_str());

        ustawWalla(pobrane,first);

        pobrane = pobierzStrone(temp);
        strcat(pobrane,">");
        cout << temp <<endl<<strona;
        }

/*
for (;;){


 xml_document doc;

    if (doc.load(pobrane)) //return -1;MO¯E ZMIANA NA WHILE>>>???CHHUJ WIE
    {
    //cout << first << "\n";
    xml_node tools = doc.child("posts").child("post");
        for (pugi::xml_attribute attr = tools.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(),"file_url")==0){


                    if (!strcmp(attr.value(),first)==0){
                            cout << first << "\n";
                        //zmianaF(attr.value());
                            cout << first << "\n";
                        //zapis(attr.value());



                        int ustaw = SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0 , (PVOID)"wall.jpg", SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
                        if (ustaw)
                        {
                        cg_Counterout <<"dziala tapeta ustawiona\n" << first;
                        Sleep(5000);
                        //ustawWalla();
                        //return 1;
                        //Sleep(30000);
                        }
                        else
                        {
                        cout << "nie dziala, przykra sprawa\n";
                        Sleep(5000);
                        //ustawWalla();
                        //return 0;
                        }
                    }
                    else
                        {
                            cout <<"DUPA\n";
                            Sleep(5000);
                            //ustawWalla();
                            //return -1;
                    }
            }
        }

    }
}
   // return 1;*/
}










/*========================================================================================================*/



LPSTR NazwaKlasy = "Klasa Okienka";
MSG Komunikat;
HWND g_hPrzycisk;
HWND g_hCheckbox;



LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );



int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{

    // WYPE£NIANIE STRUKTURY
    WNDCLASSEX wc;

    wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground =( HBRUSH )( COLOR_WINDOW + 1 );
    wc.lpszMenuName = NULL;
    wc.lpszClassName = NazwaKlasy;
    wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );




    // REJESTROWANIE KLASY OKNA
    if( !RegisterClassEx( & wc ) )
    {
        MessageBox( NULL, "Wysoka Komisja odmawia rejestracji tego okna!", "Niestety...",
        MB_ICONEXCLAMATION | MB_OK );
        return 1;
    }

    // TWORZENIE OKNA
    HWND hwnd;

    hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, NazwaKlasy, "Oto okienko", WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, 190, 240, NULL, NULL, hInstance, NULL );

    if( hwnd == NULL )
    {
        MessageBox( NULL, "Okno odmówi³o przyjœcia na œwiat!", "Ale kicha...", MB_ICONEXCLAMATION );
        return 1;
    }

    //GUZIKI ITP

    g_hPrzycisk = CreateWindowEx( 0, "BUTTON", "GUZIK", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,10, 64, 150, 30, hwnd, (HMENU)1, hInstance, NULL );
    g_hCheckbox = CreateWindowEx( 0, "BUTTON", "SAFE?", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,10, 10, 150, 30, hwnd, (HMENU)2, hInstance, NULL );
    CheckDlgButton(hwnd,2, BST_CHECKED);


    // TRAY
    LPSTR sTip = "Konahcan Wallpapers";
    NOTIFYICONDATA nid;

    nid.cbSize = sizeof( NOTIFYICONDATA );
    nid.hWnd = hwnd;
    nid.uID = ID_TRAY1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = ID_TRAY1;
    nid.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    lstrcpy( nid.szTip, sTip );

    ShowWindow( hwnd, nCmdShow ); // Poka¿ okienko...
    UpdateWindow( hwnd );


    BOOL r;
    r = Shell_NotifyIcon( NIM_ADD, & nid );
    if( !r ) MessageBox( hwnd, "No niestety, z ikonki nici...", "Łeeee...", MB_ICONEXCLAMATION );



    // Pêtla komunikatów
    while( GetMessage( & Komunikat, NULL, 0, 0 ) )
    {
        TranslateMessage( & Komunikat );
        DispatchMessage( & Komunikat );
    }


    return Komunikat.wParam;
}

// OBS£UGA ZDARZEÑ
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {


    case WM_CREATE:
        {HANDLE hUstaw = ( HANDLE ) _beginthread(drugiWatek,0, NULL);
          int ID2 = GetDlgCtrlID(g_hCheckbox);
        //for (;;){HANDLE hUstaw = ( HANDLE ) _beginthread(ustawWalla,0, NULL); Sleep(400);}
        CheckDlgButton(hwnd,ID2, BST_CHECKED);
        }
        break;

    case WM_COMMAND:
        {
            if(( HWND ) lParam == g_hCheckbox )
            {
                if (IsDlgButtonChecked(hwnd, 2)==BST_UNCHECKED)
                {
                    CheckDlgButton(hwnd,2, BST_CHECKED);
                    //SendMessage(g_hCheckbox, BM_SETCHECK, BST_UNCHECKED,0);
                    strona= "http://konachan.net/post.xml?limit=1&tags=rating%3Asafe";
                }
                else
                {
                    CheckDlgButton(hwnd,2, BST_UNCHECKED);cout <<"aa";
                    //SendMessage(g_hCheckbox, BM_SETCHECK, BST_CHECKED,0);
                    strona = "http://konachan.net/post.xml?limit=1&tags=-rating%3Asafe";
                    cout <<strona;
                }
            }
        //if(( HWND ) lParam == g_hPrzycisk )
        //ustawWalla();
        //HANDLE hUstaw = ( HANDLE ) _beginthread(ustawWalla,0, NULL);
        }
        break;

    case WM_CLOSE:
        DestroyWindow( hwnd );
        break;

    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;

        default:
        return DefWindowProc( hwnd, msg, wParam, lParam );
    }

    return 0;
}
