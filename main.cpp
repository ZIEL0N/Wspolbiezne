#include <windows.h>
#include <time.h>
#include <iostream>
#include <string>

#define sto  (100)
#define dwo (200)

void OurFunction();
DWORD WINAPI LiczenieNaWatkach(LPVOID);

static bool nieidzdalej=true;
static HINSTANCE        hInstApp;
static HWND             hwndApp;
static HWND             hwndButton;
static HWND             hwndButton2;
char   szAppName[]="Problem producenta - konsumenta.";
static HANDLE           theElementy;
static HANDLE           theMiejsca;
static HANDLE           Semafor;
static HANDLE           theProducent;
static HANDLE           theKonsument;
static HANDLE*         tablicaWatkow;
static int              we_indeks       = 0;
static int              wy_indeks       = 0;
static int              N               = 15;
static char*            bufor           = NULL;
static int**            trojkat;
static int*             Argumentl

void UzupelnijTrojkatPascala(){
  for (int i=0;i<N;i++){
        for(int j=0;j<i;j++){
            if(!(j==0||j==i-1)&&i>1){
                trojkat[i][j]=trojkat[i-1][j-1]+trojkat[i-1][j];
            }
            std::cout<<trojkat[i][j]<<' ';
        }
        std::cout<<'\n';
  }
}

void StworzSemafor() {
    Semafor = CreateSemaphore(NULL,0,1,NULL);
    int iloscWatkow = 0;
    for(int i=1;i<=N-2;i++) {
        iloscWatkow += i;
    }
    tablicaWatkow = new HANDLE[iloscWatkow];
    for(int i=0;i<iloscWatkow;i++) {
        tablicaWatkow[i] = CreateThread(NULL,0,LiczenieNaWatkach,0,0, NULL);
    }
}

DWORD WINAPI LiczenieNaWatkach(LPVOID) {

}


//----------------------------------------------------------------------
DWORD WINAPI Producent(LPVOID)
{
        while(1)
        {
                Sleep((rand()%10)*100 + 100);
                WaitForSingleObject(theMiejsca,INFINITE);
                WaitForSingleObject(theMiejsca,INFINITE);
                bufor[we_indeks] = 1;
                we_indeks = (we_indeks + 1) % N;
                ReleaseSemaphore(theElementy,1,NULL);
                RedrawWindow(hwndApp,NULL,NULL,RDW_INTERNALPAINT);
        };
 return 0;
};
DWORD WINAPI Konsument(LPVOID)
{
        while(1)
        {
                Sleep((rand()%10)*150 + 100);
                WaitForSingleObject(theElementy,INFINITE);
                bufor[wy_indeks] = 0;
                wy_indeks = (wy_indeks + 1) % N;
                ReleaseSemaphore(theMiejsca,1,NULL);
                RedrawWindow(hwndApp,NULL,NULL,RDW_INTERNALPAINT);
        };
 return 0;
};
//----------------------------------------------------------------------
void DrawWindow()
{
        HDC theDC = GetDC(hwndApp);
        HBRUSH theWhiteBrush    = CreateSolidBrush(RGB(255,255,255));
        HBRUSH theBlueBrush     = CreateSolidBrush(RGB(50,50,150));
        SetTextColor(theDC,RGB(0,0,0));
        SetBkMode(theDC,TRANSPARENT);

        int moveHor = 320, moveVer =0;
        for(int i = 0;i < N;i++)
        {
            moveVer += 20;
            moveHor = 320 - 20*i/2;
            for(int j=0;j<i;j++){

                RECT theRect;
                theRect.left    = j*20 +  moveHor;
                theRect.right   = 10+ j*20 + moveHor;
                theRect.top     = 50 + moveVer;
                theRect.bottom  = 65 + moveVer;

                if (bufor[i])
                        FillRect(theDC,&theRect,theBlueBrush);
                else
                        FillRect(theDC,&theRect,theWhiteBrush);
            }
        }
        DeleteObject((HBRUSH)theWhiteBrush);
        DeleteObject((HBRUSH)theBlueBrush);
        ReleaseDC(hwndApp,theDC);
};
LONG CALLBACK AppWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
  switch (msg)
  {
  case WM_COMMAND:
      if(LOWORD(wParam)==dwo) {
            std::cout<<"no";
            LPSTR jakisText;
            GetWindowText(hwndButton,jakisText,3);
            N = 5;
            OurFunction();
      }
    break;
case WM_KILLFOCUS:
break;
case WM_SETFOCUS:
break;
        case WM_CLOSE:
PostQuitMessage(0);
              break;
case WM_PAINT:
    if(!nieidzdalej) {
                DrawWindow();
    }
break;
case WM_DESTROY:
                CloseHandle(theElementy);
                CloseHandle(theMiejsca);
                TerminateThread(theProducent,0);
                TerminateThread(theKonsument,0);
                CloseHandle(theProducent);
                CloseHandle(theKonsument);
                if (bufor) delete[] bufor;
break;
  };
  return DefWindowProc(hwnd,msg,wParam,lParam);
}
BOOL AppInit(HINSTANCE hInst,HINSTANCE hPrev,int sw,LPSTR szCmdLine)
{
  // Rejestracja klasy okna.
  WNDCLASS cls;
  hInstApp = hInst;
  if (!hPrev)
  {
    cls.hCursor        = LoadCursor(0,IDC_ARROW);
    cls.hIcon          = NULL;
    cls.lpszMenuName   = NULL;
    cls.lpszClassName  = szAppName;
    cls.hbrBackground  = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    cls.hInstance      = hInst;
    cls.style          = CS_VREDRAW | CS_HREDRAW;
    cls.lpfnWndProc    = (WNDPROC)AppWndProc;
    cls.cbClsExtra     = 0;
    cls.cbWndExtra     = 0;
    if (!RegisterClass(&cls)) return FALSE;
  }
  // Tworzenie okna aplikacji.
  RECT rek;
  GetWindowRect(GetDesktopWindow(),&rek);
  hwndApp = CreateWindowEx(
                  WS_EX_APPWINDOW,
                  szAppName,szAppName,
  WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
  ((rek.right - rek.left) - 320)/2,((rek.bottom - rek.top) - 160)/2,
  640,320,0,0,hInst,0);
  ShowWindow(hwndApp,SW_SHOW);

    hwndButton = CreateWindow(
            "EDIT",  // Predefined class; Unicode assumed
            "",      // Button text
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
            10,         // x position
            10,         // y position
            100,        // Button width
            20,        // Button height
            hwndApp,     // Parent window
            NULL,       // No menu.
            (HINSTANCE)GetWindowLong(hwndApp, GWL_HINSTANCE),
            NULL);      // Pointer not needed.

    hwndButton2 = CreateWindow(
            "BUTTON",  // Predefined class; Unicode assumed
            "",      // Button text
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
            110,         // x position
            10,         // y position
            100,        // Button width
            20,        // Button height
            hwndApp,     // Parent window
            (HMENU)dwo,       // No menu.
            (HINSTANCE)GetWindowLong(hwndApp, GWL_HINSTANCE),
            NULL);      // Pointer not needed.

  return TRUE;
}
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int sw)
{
  MSG     msg;
  if (!AppInit(hInst,hPrev,sw,szCmdLine)) return FALSE;
  for (;;)
  {
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
  {
                break;
  }
      else
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      };
    }
    else
    {
       WaitMessage();
    }
  }
  return msg.wParam;
}

void OurFunction() {
  nieidzdalej = true;
  // Tworzymy bufor.
  bufor = new char[N];
  for(int i = 0;i < N;i++) bufor[i] = 0;
  // Tworzymy semafory.
  theElementy   = CreateSemaphore(NULL,0,N,NULL);
  theMiejsca    = CreateSemaphore(NULL,N,N,NULL);
  // Tworzymy watki.
  DWORD ID;
  theProducent  = CreateThread(NULL,0,Producent,0,0,&ID);
  theKonsument  = CreateThread(NULL,0,Konsument,0,0,&ID);
  // Inicjacja generatora losowego.
  time_t t;
  srand((unsigned) time(&t));

  trojkat = new int* [N];
  for (int i=0;i<N;i++){
        trojkat[i]=new int[i+1];
        for(int j=0;j<i;j++){
            if(j==0||j==i-1) trojkat[i][j]=1;
        }
  }

  UzupelnijTrojkatPascala();

  if(N>1) {
    StworzSemafor();
  }

}

