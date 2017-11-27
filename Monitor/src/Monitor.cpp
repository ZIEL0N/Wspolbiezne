#include "Monitor.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

#define b1 2
#define b2 20

using namespace std;

int bufor1[b1];          //bufor b1 o rozmiarze zdefiniowanym z zmiennej b1
int bufor2[b2];          //bufor b2 o rozmiarze zdefiniowanym z zmiennej b2
int b1counter = 0;

static int index = 0;           //wskazuje na któr¹ pozycjê w buforze b1 bêd¹ wstawiane kolejne elementy

bool pelny=false;

std::mutex pelny_mutex;

std::mutex b1counter_mutex;

std::mutex bufor1_mutex;
std::mutex bufor2_mutex;
std::mutex index_mutex;

Monitor::Monitor()
{
    //ctor
}

Monitor::~Monitor()
{
    //dtor
}

void Monitor::EnterProces(int value)
{
    if(b1counter<b1)
        {
        index_mutex.lock();
        bufor1_mutex.lock();
        bufor1[index] = value;                        //umieœæ wartoœæ w buforze
        b1counter_mutex.lock();
        b1counter++;
        b1counter_mutex.unlock();
        bufor1_mutex.unlock();
        index = (index + 1) % b1;                       //zwieksz index
        index_mutex.unlock();
        }
}

void Monitor::EnterProcesS(int &n, int &m)
{
    int p1, p2, p3;                                         //zmienne lokalne w ktorych zapisujemy pobrane wartosci z b1 i ich sume przekazywana do b2
        if(b1counter >= 2 && !pelny) {
            b1counter_mutex.lock();
            b1counter = b1counter - 2;
            b1counter_mutex.unlock();
        p1 = bufor1[m];
        m = (m + 1) % b1;
        p2 = bufor1[m];                                     //pobierz je i zwieksz index
        m = (m + 1) % b1;
        p3 = p1 + p2;                                       //oblicz wartosc p3
        bufor2_mutex.lock();
        bufor2[n] = p3;                                     //wstaw wartosc w bufor2 i zwieksz index
        bufor2_mutex.unlock();
        n++;
        if(n==b2) {
            pelny_mutex.lock();
            pelny = true;
            pelny_mutex.unlock();
            n=0;
        }
        }

}

void Monitor::EnterProcesK()
{

    if(pelny) {
        bufor2_mutex.lock();

        for(int i=0;i<b2;i++) {                             //zkonsumuj wartosci z bufora i wyzeruj
            cout << bufor2[i] << " ";                       //przy okazji wyzeruj je
            bufor2[i] = 0;
        }
        cout << endl;

        pelny_mutex.lock();
        pelny=false;
        pelny_mutex.unlock();
        bufor2_mutex.unlock();
    }
}
