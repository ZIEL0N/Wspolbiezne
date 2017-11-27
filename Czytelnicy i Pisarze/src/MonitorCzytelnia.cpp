#include "MonitorCzytelnia.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <time.h>


int *stanowiska;
static int stanowiskaLength=0;
static int currentWriteindex=0;

bool pisanie=false;

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
    int data = stanowiska[currentWriteindex];
    konsola_mutex.lock();
    std::cout<<"C "<<std::this_thread::get_id()<<" czyta "<<currentWriteindex<<" "<<data<<"\n";
    konsola_mutex.unlock();
    std::this_thread::sleep_for(std::chrono::microseconds(500000));


}

void MonitorCzytelnia::ExitCzytelnik()
{

}

void MonitorCzytelnia::WritePisarz()
{
    pisarz_mutex.lock();

    stanowiska[currentWriteindex]+=1;
    konsola_mutex.lock();
    std::cout<<"P "<<std::this_thread::get_id()<<" pisze "<<currentWriteindex<<"\n";

    std::this_thread::sleep_for(std::chrono::microseconds(500000));

    konsola_mutex.unlock();

    currentWriteindex++;
    if(currentWriteindex>=stanowiskaLength) currentWriteindex=0;

    pisarz_mutex.unlock();
}
