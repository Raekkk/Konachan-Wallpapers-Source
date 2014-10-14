#ifndef WINAPI_H_INCLUDED
#define WINAPI_H_INCLUDED
LPSTR NazwaKlasy = "Klasa Okienka";
MSG Komunikat;
HWND g_hPrzycisk;
HWND g_hCheckbox;

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );


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

    ShowWindow( hwnd, nCmdShow ); // Poka¿ okienko...
    UpdateWindow( hwnd );





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
        if(( HWND ) lParam == g_hPrzycisk )
        //ustawWalla();
        //HANDLE hUstaw = ( HANDLE ) _beginthread(ustawWalla,0, NULL);
        int a = ddd();
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



#endif // WINAPI_H_INCLUDED
