//
//  Created by 杜国超 on 19-1-22.
//  Copyright © 2019年 杜国超. All rights reserved.
//  共享内存数据读写锁
//

#ifndef SHMQUEUE_SHM_RWLOCK_H
#define SHMQUEUE_SHM_RWLOCK_H

#include <sys/sem.h>
#include <sys/types.h>

namespace shmmqueue
{
class CShmRWlock
{
public:
    // 构造函数
    CShmRWlock();
    //构造函数.
    CShmRWlock(key_t iKey);
    //读锁
    int Rlock() const;
    //释放读锁
    int UnRlock() const;
    /**
     * TryRlock
     * @return  true lock ok,false lock failed
     */
    bool TryRlock() const;
    //写锁
    int Wlock() const;
    //释放写锁
    int UnWlock() const;
    /**
    * TryRlock
    * @return  true lock ok,false lock failed
    */
    bool TryWlock() const;
    //lock block until lock sucess
    int Lock() const;
    //Unlock
    int Unlock() const;
    /**
     * trylock
     * @return true lock ok,false lock failed
     */
    bool trylock() const;
    //get sem key
    key_t Getkey() const;
    //get sem id
    int getid() const;

private:
    //初始化
    void init(key_t iKey);
protected:
    int m_iSemID;
    key_t m_iSemKey;
};



class PthreadRWlock {
public:

    PthreadRWlock() {
            pthread_rwlockattr_t rwattr;
            pthread_rwlockattr_init(&rwattr);
            pthread_rwlockattr_setpshared(&rwattr, PTHREAD_PROCESS_SHARED);
            pthread_rwlock_init(&m_lock, &rwattr);
    }
    ~PthreadRWlock() {
        pthread_rwlock_destroy(&m_lock);
    }

    //读锁
    int Rlock() {
        return pthread_rwlock_rdlock(&m_lock);
    }
    //释放读锁
    int UnRlock() {
        return pthread_rwlock_unlock(&m_lock);
    }
    /**
     * TryRlock
     * @return  true lock ok,false lock failed
     */
    bool TryRlock() {
        return pthread_rwlock_tryrdlock(&m_lock) == 0;
    }
    //写锁
    int Wlock()  {
        return pthread_rwlock_wrlock(&m_lock);
    }
    //释放写锁
    int UnWlock() {
        return pthread_rwlock_unlock(&m_lock);
    }
    /**
    * TryRlock
    * @return  true lock ok,false lock failed
    */
    bool TryWlock() {
        return pthread_rwlock_trywrlock(&m_lock) == 0;
    }

private:
    pthread_rwlock_t m_lock;
};

class CSafeShmRlock
{
public:
    CSafeShmRlock() : m_pLock(NULL)
    {
    }
    CSafeShmRlock(PthreadRWlock *pLock)
        : m_pLock(pLock)
    {
        if (m_pLock != NULL)
        {
            m_pLock->Rlock();
        }
    }

    void InitLock(PthreadRWlock *pLock)
    {
        m_pLock = pLock;
        m_pLock->Rlock();
    }

    ~CSafeShmRlock()
    {
        m_pLock->UnRlock();
    }
private:
//    CShmRWlock *m_pLock;
    PthreadRWlock *m_pLock;
};

class CSafeShmWlock
{
public:
    CSafeShmWlock()
            : m_pLock(NULL)
    {

    }
    CSafeShmWlock(PthreadRWlock *pLock)
        : m_pLock(pLock)
    {
        m_pLock->Wlock();
    }

    void InitLock(PthreadRWlock *pLock)
    {
        m_pLock = pLock;
        m_pLock->Wlock();
    }


    ~CSafeShmWlock()
    {
        if (m_pLock != NULL)
        {
            m_pLock->UnWlock();
        }
    }
private:
//    CShmRWlock *m_pLock;
    PthreadRWlock *m_pLock;
};
}
#endif //SHMQUEUE_SHM_RWLOCK_H
