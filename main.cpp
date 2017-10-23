#include <windows.h>
#include <time.h>
#include <iostream>
#include <string>

using namespace std;

static HANDLE Semafor1;
static HANDLE Semafor2;
static HANDLE Watek1;
static HANDLE Watek2;
static int N;
static int** trojkat;
static int opcja0 = 0;
static int opcja1 = 1;

void LiczenieIteracyjne() {
  for (int i=0;i<N;i++)
  {
    trojkat[i][0]=1;
    trojkat[i][i]=1;

    for (int j=0; j<i-1; j++)
      trojkat[i][j+1]=trojkat[i-1][j]+trojkat[i-1][j+1];
  }
}

DWORD WINAPI LiczenieNaWatkach(LPVOID o) {

    int *oValue = (int *)o;
    if(*oValue==0) {
            WaitForSingleObject(Semafor1,INFINITE);
        for(int i=0;i<N;i++) {
            for(int j=0;j<=i/2;j++) {
                    if(j==0) trojkat[i][j]=1;
                    else
                    {
                        if(i>=2)
                        {
                            if(j*2==i)
                            {
                                if((trojkat[i-1][j-1]==0 || trojkat[i-1][j]==0))
                                {
                                    WaitForSingleObject(Semafor1,INFINITE);
                                    j--;
                                }
                                else {
                                    trojkat[i][j]=trojkat[i-1][j-1] + trojkat[i-1][j];
                                    ReleaseSemaphore(Semafor2,1,NULL);
                                }
                            }
                            else
                            {
                                trojkat[i][j]=trojkat[i-1][j-1] + trojkat[i-1][j];
                            }
                        }
                    }
            }
        }
    }
    else {
        WaitForSingleObject(Semafor2,INFINITE);
        for(int i=0;i<N;i++)
        {
            for(int j=(i+2)/2;j<=i;j++)
                {
                if(j==i)
                {
                    trojkat[i][j]=1;
                    if(i==1) ReleaseSemaphore(Semafor1,1,NULL);
                }
                    else
                    {
                        if(i>=2)
                        {
                            if(j*2-1==i)
                            {
                                if((trojkat[i-1][j-1]==0 || trojkat[i-1][j]==0)) {
                                    WaitForSingleObject(Semafor2,INFINITE);
                                    j--;
                                }
                                else {
                                    trojkat[i][j]=trojkat[i-1][j-1] + trojkat[i-1][j];
                                    ReleaseSemaphore(Semafor1,1,NULL);
                                }
                            }
                            else
                            {
                                trojkat[i][j]=trojkat[i-1][j-1] + trojkat[i-1][j];
                            }
                        }
                    }
                }
            }
        }
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
    Semafor1 = CreateSemaphore(NULL,0,1,NULL);
    Semafor2 = CreateSemaphore(NULL,1,1,NULL);
    Watek1 = CreateThread(NULL,0,LiczenieNaWatkach,&opcja0,2,NULL);
    Watek2 = CreateThread(NULL,0,LiczenieNaWatkach,&opcja1,0,NULL);
    clock_t start = clock();

    bool done = false;
    while(!done) {
            done=true;
        for(int j=0;j<N;j++) {
                if(trojkat[N-1][j]!=0) {}
                else done=false;
        }
    }
    system("cls");
    cout << "Czas liczenia na 2 watkach:" << ((double)(clock() - start)/CLOCKS_PER_SEC) << endl;
    cout << "Wynik :\n";
    for(int j=0;j<N;j++) {
        cout << trojkat[N-1][j] << " ";
    }
    cout << endl;
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
    done = false;
    while(!done) {
            done=true;
        for(int j=0;j<N;j++) {
                if(trojkat[N-1][j]!=0) {}
                else done=false;
        }
    }
    cout << "Czas liczenia na 1 watku:" << ((double)(clock() - start)/CLOCKS_PER_SEC) << endl;
    cout << "Wynik :\n";
    for(int j=0;j<N;j++) {
        cout << trojkat[N-1][j] << " ";
    }
    return 0;
}

