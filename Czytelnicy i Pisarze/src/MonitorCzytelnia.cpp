#include "MonitorCzytelnia.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <time.h>
#include <cstdlib>


static int *stanowiska;
static int stanowiskaLength=0;
static int currentWriteindex=0;

static bool pisanie=false;

std::mutex pisarz_mutex;
std::mutex konsola_mutex;

MonitorCzytelnia::MonitorCzytelnia(int n)
{
    stanowiska = new int [n];
    for(int i=0;i<n;i++){
        stanowiska[i]=0;
    }
    stanowiskaLength=n;
}

MonitorCzytelnia::~MonitorCzytelnia()
{
}

void MonitorCzytelnia::EnterCzytelnik()
{
    if(!pisanie){
        int readIndex = rand()%stanowiskaLength;
        int data = stanowiska[readIndex];
        konsola_mutex.lock();
        std::cout<<"C"<<std::this_thread::get_id()<<"\tczyta\t"<<readIndex<<"\n";
        konsola_mutex.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(500000));
    }
}

void MonitorCzytelnia::ExitCzytelnik()
{

}

void MonitorCzytelnia::WritePisarz()
{
    pisarz_mutex.lock();
    pisanie=true;
    stanowiska[currentWriteindex]+=1;
    konsola_mutex.lock();
    std::cout<<"P"<<std::this_thread::get_id()<<"\tpisze\t"<<currentWriteindex<<"\n";
    for(int i=0;i<stanowiskaLength;i++){
        std::cout<<stanowiska[i]<<" ";
    }
    std::cout<<'\n';
    std::this_thread::sleep_for(std::chrono::microseconds(500000));

    konsola_mutex.unlock();

    currentWriteindex++;
    if(currentWriteindex>=stanowiskaLength) currentWriteindex=0;
    pisanie=false;
    pisarz_mutex.unlock();
}
