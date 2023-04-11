#pragma once

#include <pthread.h>

class CAutoLock
{
public:
    CAutoLock(): m_pLock(NULL) {}
    CAutoLock(pthread_mutex_t *pLock)
            : m_pLock(pLock)
    {
        pthread_mutex_lock(m_pLock);
    }

    void InitLock(pthread_mutex_t *pLock)
    {
        m_pLock = pLock;
        pthread_mutex_lock(m_pLock);
    }

    ~CAutoLock()
    {
        if (m_pLock != NULL)
        {
            pthread_mutex_unlock(m_pLock);
        }
    }

private:
    CAutoLock(const CAutoLock&) = delete;
    CAutoLock& operator=(const CAutoLock&) = delete;

private:
    pthread_mutex_t *m_pLock;
};