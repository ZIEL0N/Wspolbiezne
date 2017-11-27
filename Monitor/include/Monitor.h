#ifndef MONITOR_H
#define MONITOR_H


class Monitor
{
    public:
        Monitor();
        virtual ~Monitor();
        void EnterProces(int value);
        void EnterProcesK();
        void EnterProcesS(int &n, int &m);

    protected:

    private:
};

#endif // MONITOR_H
