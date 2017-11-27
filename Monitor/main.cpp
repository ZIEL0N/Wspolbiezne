#include <iostream>
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include "Monitor.h"

#define N 3
#define b1 16
#define b2 50


using namespace std;

static HANDLE procesy[N];       //Procesy P1 ... Pn
static int wartosci[N];         //Wartosci produkowane przez funkcje, stale dla danego procesu, zeby mozna bylo rozpoznac kolejnosc ich wykonania
static HANDLE procesK;          //Proces konsumuj¹cy z pe³nego bufora P2
static HANDLE procesS;          //Proces przetwarzaj¹cy z bufora P1 do P2

static Monitor monitor;

DWORD WINAPI Proces(LPVOID o) { //Funkcja procesów P1 ... Pn
    int *oValue = (int *)o;                         //pobierz wartoœæ któr¹ bêdziesz dodawa³ do bufora
    while(true) {
    //Sleep();
    monitor.EnterProces(*oValue);
    }
    return 0;
}

DWORD WINAPI ProcesS(LPVOID o) {    //Funkcja procesu S, zakladamy ze z bufora b1 pobieramy 2 wartosci i dodajemy je zanim uzyjemy ich w buforze b2
    int m = 0;                                              //zmienna mowiaca z ktorej pozycji w buforze b1 powinnismy pobrac wartosc
    int n = 0;                                              //zmienna mowiaca na ktora pozycje w buforze b2 wpisujemy wartosci
    while(true) {
        monitor.EnterProcesS(m,n);
        }
    }

DWORD WINAPI Bufor(LPVOID o) {
    //entry Wstaw();
    //entry Pobierz();

    int bufor1[b1];
    int wskP, wskK = 1;
    int licznik = 1;

}

DWORD WINAPI ProcesK(LPVOID o) {
    while(true) {
        monitor.EnterProcesK();
    }
}

int main()
{
    srand( time( NULL ) );

    procesS = CreateThread(NULL,0,ProcesS,NULL,0,NULL);
    procesK = CreateThread(NULL,0,ProcesK,NULL,0,NULL);

    for(int i=0;i<N;i++) {
        wartosci[i] = i+1;
    }

    for(int i=0;i<N;i++) {
        procesy[i] = CreateThread(NULL,0,Proces,&wartosci[i],0,NULL);
    }

    getchar();

    TerminateThread(procesS,0);
    CloseHandle(procesS);
    TerminateThread(procesK,0);
    CloseHandle(procesK);


    for(int i=0;i<N;i++) {
        TerminateThread(procesy[i],0);
        CloseHandle(procesy[i]);
    }

    return 0;
}
