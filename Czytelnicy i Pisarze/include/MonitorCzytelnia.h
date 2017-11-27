#ifndef MONITORCZYTELNIA_H
#define MONITORCZYTELNIA_H


class MonitorCzytelnia
{
    public:
        MonitorCzytelnia(int n);
        virtual ~MonitorCzytelnia();
        void EnterCzytelnik();
        void ExitCzytelnik();
        void WritePisarz();

    protected:

    private:
};

#endif // MONITORCZYTELNIA_H
