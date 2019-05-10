//---------------------------------------------------------------------------
#ifndef MC_LOCKERH
#define MC_LOCKERH
//---------------------------------------------------------------------------
#ifdef _WIN32
    #include <Windows.h>
class mc_locker
{
public:
    mc_locker()
        {
        InitializeCriticalSection(&m_lk);
        }

    ~mc_locker()
        {
        DeleteCriticalSection(&m_lk);
        }

    void lock()
        {
        EnterCriticalSection(&m_lk);
        }

    void unlock()
        {
        LeaveCriticalSection(&m_lk);
        }

    bool try_lock()
        {
        return (FALSE != TryEnterCriticalSection(&m_lk));
        }

private:
    CRITICAL_SECTION m_lk;
};
#endif //_WIN32
#ifdef __linux__
#include <pthread.h>
class mc_locker
{
public:
    mc_locker():m_lk(PTHREAD_MUTEX_INITIALIZER)
    {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&m_lk, &attr);
    }

    ~mc_locker()
    {
        //DeleteCriticalSection(&m_lk);
    }

    void lock()
    {
        pthread_mutex_lock(&m_lk);
    }

    void unlock()
    {
        pthread_mutex_unlock(&m_lk);
    }

    bool try_lock()
    {
        return false;
        //return (FALSE != TryEnterCriticalSection(&m_lk));
    }

private:
    pthread_mutex_t m_lk;
};
#endif
//---------------------------------------------------------------------------
class mc_auto_locker
{
public:
    mc_auto_locker(mc_locker& lk) : m_lk(lk)
    {
        m_lk.lock();
    }

    ~mc_auto_locker()
    {
        m_lk.unlock();
    }

protected:
    mc_locker& m_lk;
};

//---------------------------------------------------------------------------
#define kinkoo_auto_locker(_x_)  mc_auto_locker __tmp_##_x_(_x_);
//---------------------------------------------------------------------------
#endif
