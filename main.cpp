#include <windows.h>
#include <time.h>
#include <iostream>
#include <string>

#define sto  (100)
#define dwo (200)
#define tro (300)

void ObliczTrojkatNaWatkach();
DWORD WINAPI LiczenieNaWatkach(LPVOID);

static bool nieidzdalej=true;
static HINSTANCE        hInstApp;
static HWND             hwndApp;
static HWND             hwndEditField;
static HWND             hwndButton;
static HWND             hwndButton2;
char   szAppName[]="Trojkat Pascala";
static HANDLE           theElementy;
static HANDLE           theMiejsca;
static HANDLE           Semafor;
static HANDLE           theProducent;
static HANDLE           theKonsument;
static HANDLE*         tablicaWatkow;
static int              we_indeks       = 0;
static int              wy_indeks       = 0;
static int              N               = 10;
static char*            bufor           = NULL;
static int**            trojkat;
static int*             Argument1;

struct Index{
    public:
        int i;
        int j;
        long long value=-1;

        Index(){
            this->i=0;
            this->j=0;
        }
        Index(int i, int j){
            this->i=i;
            this->j=j;
        }
};

static Index**           trojkatNaWatkach;
static HANDLE**           trojkatSemaforow;
static HANDLE**           trojkatWatkow;

static HANDLE           watekPrintu;


void Clear(){
    if(trojkatWatkow!=NULL && trojkatSemaforow!=NULL){
        for(int i=0;i<N;i++){
            for(int j=0;j<i;j++){
                CloseHandle(trojkatSemaforow[i][j]);
                TerminateThread(trojkatWatkow[i][j],0);
                CloseHandle(trojkatWatkow[i][j]);
            }
        }
    }
    TerminateThread(watekPrintu,0);
    CloseHandle(watekPrintu);
}
clock_t start;
clock_t start2;

DWORD WINAPI PrintTrojkat(LPVOID o){
    int suma=0;
    for(int i=0;i<N;i++){
        for(int j=0;j<=i;j++){
                suma++;
            }
    }
    WaitForMultipleObjects(N*N,*trojkatSemaforow,true,INFINITE);
    std:: cout << "Watki: " << ((double)(clock() - start)/CLOCKS_PER_SEC) << '\n';
    /*for(int i=0;i<N;i++){
        for(int j=0;j<=i;j++){
            std::cout<<trojkatNaWatkach[i][j].value<<' ';
        }
        std::cout<<'\n';
    }*/
}


void UzupelnijTrojkatPascalaNaWatkach(){
  watekPrintu = CreateThread(NULL,0,PrintTrojkat,NULL,0, NULL);

  start = clock();

  for (int i=0;i<N;i++){
        for(int j=0;j<=i;j++){
            trojkatSemaforow[i][j] = CreateSemaphore(NULL,0,2,NULL);
            if(i==0||j==i||j==0) ReleaseSemaphore(trojkatSemaforow[i][j],2,NULL);
            else trojkatWatkow[i][j]=CreateThread(NULL,2,LiczenieNaWatkach,&trojkatNaWatkach[i][j],0, NULL);
        }
  }
}


DWORD WINAPI LiczenieNaWatkach(LPVOID o) {
    Index *ind = (struct Index *)o;
    WaitForSingleObject(trojkatSemaforow[ind->i-1][ind->j-1], INFINITE);
    WaitForSingleObject(trojkatSemaforow[ind->i-1][ind->j], INFINITE);
    ind->value= trojkatNaWatkach[ind->i-1][ind->j-1].value + trojkatNaWatkach[ind->i-1][ind->j].value;

    ReleaseSemaphore(trojkatSemaforow[ind->i][ind->j],2,NULL);
}

void ObliczTrojkatNaWatkach() {
  nieidzdalej = true;
  trojkatNaWatkach = new Index* [N];
  trojkatSemaforow = new HANDLE* [N];
  trojkatWatkow = new HANDLE* [N];
  for (int i=0;i<N;i++){
        trojkatNaWatkach[i]=new Index[i+1];
        trojkatSemaforow[i]=new HANDLE[i+1];
        trojkatWatkow[i]=new HANDLE[i+1];
        for(int j=0;j<=i;j++){
            trojkatNaWatkach[i][j].i=i;
            trojkatNaWatkach[i][j].j=j;
            if(i==0||j==i||j==0) trojkatNaWatkach[i][j].value=1;
        }
  }
  UzupelnijTrojkatPascalaNaWatkach();

}

void ObliczTrojkatIteracyjnie() {
    Index **trojkatPascala = new Index *[N];
    start2 = clock();
    for (int i=0;i<N;i++)
    {
        trojkatPascala[i]=new Index [i+1];

        for (int j=0; j<=i; j++) {
            trojkatPascala[i][j].i=i;
            trojkatPascala[i][j].j=i;
            if(i<2 || j==0 || j==i){
                trojkatPascala[i][j].value=1;
            }
            else{
                trojkatPascala[i][j].value=trojkatPascala[i-1][j-1].value+trojkatPascala[i-1][j].value;
            }
        }
    }
    std::cout << "Iteracyjnie: " << ((double)(clock() - start2)/CLOCKS_PER_SEC) << '\n';

    /*for (int j=0;j<N;j++)
    {
        for(int i=0;i<=j;i++) {
            std::cout << trojkatPascala[j][i].value << " ";
        }
        std::cout << "\n";
    }*/
}

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
      LPSTR jakisText;
      if(LOWORD(wParam)==dwo) {
            Clear();
            GetWindowText(hwndEditField,jakisText,10);
            char * tekst = jakisText;
            N = atoi(tekst);
            if(N<=0) N=0;
            ObliczTrojkatNaWatkach();
      }
      if(LOWORD(wParam)==tro) {
            GetWindowText(hwndEditField,jakisText,10);
            char * tekst = jakisText;
            N = atoi(tekst);
            if(N<=0) N=0;
            ObliczTrojkatIteracyjnie();
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
    Clear();
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
  300,100,0,0,hInst,0);
  ShowWindow(hwndApp,SW_SHOW);

    hwndEditField = CreateWindow(
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

    hwndButton = CreateWindow(
            "BUTTON",  // Predefined class; Unicode assumed
            "Thread",      // Button text
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
            110,         // x position
            10,         // y position
            100,        // Button width
            20,        // Button height
            hwndApp,     // Parent window
            (HMENU)dwo,       // No menu.
            (HINSTANCE)GetWindowLong(hwndApp, GWL_HINSTANCE),
            NULL);      // Pointer not needed.
    hwndButton2 = CreateWindow(
            "BUTTON",  // Predefined class; Unicode assumed
            "Iteration",      // Button text
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
            110,         // x position
            30,         // y position
            100,        // Button width
            20,        // Button height
            hwndApp,     // Parent window
            (HMENU)tro,       // No menu.
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


