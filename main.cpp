#include <windows.h>
#include <time.h>
#include <iostream>
#include <string>

using namespace std;

static bool done1;
static bool done2;

static HANDLE Semafor1;
static HANDLE Semafor2;
static HANDLE Watek1;
static HANDLE Watek2;
static int N;
static int** trojkat;
static int opcja0 = 0;
static int opcja1 = 1;
clock_t start;

int index = 1;
static HANDLE semaforIndex;

void LiczenieIteracyjne() {
  for (int i=0;i<N;i++)
  {
    trojkat[i][0]=1;
    trojkat[i][i]=1;

    for (int j=0; j<i-1; j++)
      trojkat[i][j+1]=trojkat[i-1][j]+trojkat[i-1][j+1];
  }
}

DWORD WINAPI WatekNieparzysty(LPVOID o) {
    int temp=0;
    for (int i=0;i<N;i++)
    {
    trojkat[i][0]=1;
    trojkat[i][i]=1;
    }
    while(temp+1<N)
    {
        WaitForSingleObject(Semafor1,INFINITE);
        temp = index;
        index++;
        ReleaseSemaphore(Semafor2,1,NULL);

        for (int i=temp+1;i<N;i++)
        {
            trojkat[i][temp] = trojkat[i-1][temp-1] + trojkat[i-1][temp];
        }
    }
    ReleaseSemaphore(Semafor2,1,NULL);
    cout << "Czas liczenia na 2 watkach:" << ((double)(clock() - start)/CLOCKS_PER_SEC) << endl;
}

DWORD WINAPI WatekParzysty(LPVOID o) {
    int temp=0;
    while(temp+1<N)
    {
        WaitForSingleObject(Semafor2,INFINITE);
        temp = index;
        index++;
        ReleaseSemaphore(Semafor1,1,NULL);

        for (int i=temp+1;i<N;i++)
        {
            trojkat[i][temp] = trojkat[i-1][temp-1] + trojkat[i-1][temp];
        }
    }
    ReleaseSemaphore(Semafor1,1,NULL);
}


int main()
{
    cin >> N;
    trojkat = new int*[N];
    for(int i=0;i<N;i++) {
        trojkat[i] = new int[i+1];
        for(int j=0;j<=i;j++) {
            trojkat[i][j]=0;
        }
    }
    semaforIndex = CreateSemaphore(NULL,1,1,NULL);
    Semafor1 = CreateSemaphore(NULL,1,1,NULL);
    Semafor2 = CreateSemaphore(NULL,0,1,NULL);
    Watek1 = CreateThread(NULL,0,WatekNieparzysty,NULL,0,NULL);
    Watek2 = CreateThread(NULL,0,WatekParzysty,NULL,0,NULL);
    start = clock();

    Sleep(1000);



    cout << "Wynik :\n";
    for(int j=0;j<N;j++) {
        cout << trojkat[N-1][j] << " ";
    }
    cout << endl;
    TerminateThread(Watek1,0);
    TerminateThread(Watek2,0);
    CloseHandle(Watek1);
    CloseHandle(Watek2);
    CloseHandle(Semafor1);
    CloseHandle(Semafor2);

    for(int i=0;i<N;i++) {
        for(int j=0;j<=i;j++) {
            trojkat[i][j]=0;
        }
    }

    start = clock();
    LiczenieIteracyjne();
    cout << "Czas liczenia na 1 watku:" << ((double)(clock() - start)/CLOCKS_PER_SEC) << endl;
    cout << "Wynik :\n";
    for(int j=0;j<N;j++) {
        cout << trojkat[N-1][j] << " ";
    }
    return 0;
}

