#include <windows.h>
#include <time.h>
static HINSTANCE        hInstApp;
static HWND             hwndApp;
char   szAppName[]="Problem producenta - konsumenta.";
static HANDLE           theElementy;
static HANDLE           theMiejsca;
static HANDLE           theProducent;
static HANDLE           theKonsument;
static int              we_indeks       = 0;
static int              wy_indeks       = 0;
static int              N               = 15;
static char*            bufor           = NULL;
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
        TextOut(theDC,135,20,"Bufor:",6);
        TextOut(theDC,45,90,"Problem producenta-konsumenta.",30);
        for(int i = 0;i < N;i++)
        {
            int moveHor =0, moveVer =0;

                if(i>0){
                    moveHor -= i*(300/N)/2;
                    moveVer += 20;
                }
                if(i>2){
                    moveHor -= i*(300/N)/2;
                    moveVer += 20;

                }
                if(i>5){
                    moveHor -= i*(300/N);
                    moveVer += 20;

                }
                if(i>9){
                    moveHor -= i*(300/N);
                    moveVer += 20;

                }
                RECT theRect;
                theRect.left    = 9 + i*(300 / N) + moveHor;
                theRect.right   = 7 + (i + 1)*(300 / N) + moveHor;
                theRect.top     = 50 + moveVer;
                theRect.bottom  = 65 + moveVer;




                if (bufor[i])
                        FillRect(theDC,&theRect,theBlueBrush);
                else
                        FillRect(theDC,&theRect,theWhiteBrush);
        };
        DeleteObject((HBRUSH)theWhiteBrush);
        DeleteObject((HBRUSH)theBlueBrush);
        ReleaseDC(hwndApp,theDC);
};
LONG CALLBACK AppWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
  switch (msg)
  {
case WM_KILLFOCUS:
break;
case WM_SETFOCUS:
break;
        case WM_CLOSE:
PostQuitMessage(0);
              break;
case WM_PAINT:
                DrawWindow();
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
