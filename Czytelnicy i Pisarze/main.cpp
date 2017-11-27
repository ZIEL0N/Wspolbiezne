#include <iostream>
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include "MonitorCzytelnia.h"

#define N 30


using namespace std;

static HANDLE procesyPisarz[N];
static HANDLE procesyCzytelnik[N];

static MonitorCzytelnia *czytelnia = new MonitorCzytelnia(N);


DWORD WINAPI ProcesPisarz(LPVOID o) {
    while(true)
    {
        czytelnia->WritePisarz();
    }
}


DWORD WINAPI ProcesCzytelnik(LPVOID o) {
    while(true)
    {
        czytelnia->EnterCzytelnik();
        czytelnia->ExitCzytelnik();
    }
}

int main()
{
    srand( time( NULL ) );


    for(int i=0;i<N;i++){
        procesyCzytelnik[i]=CreateThread(NULL,0,ProcesCzytelnik,NULL,0,NULL);
        procesyPisarz[i]=CreateThread(NULL,0,ProcesPisarz,NULL,0,NULL);
    }

    getchar();



    for(int i=0;i<N;i++) {
        TerminateThread(procesyCzytelnik[i],0);
        CloseHandle(procesyCzytelnik[i]);
        TerminateThread(procesyPisarz[i],0);
        CloseHandle(procesyPisarz[i]);
    }

    return 0;
}
