/**
 * Copyright (c) Vidscale , 2014
 * @SigSimCommStructs.hpp
 * @author vkumar 
 * @date   2014-05-31
 */

/* History:
 * Date       Author  Description
 * ------------------------------------------------------------------------
 * 2014-05-31     vkumar  
 */


#ifndef _AUTO_LOCK_H_
#define _AUTO_LOCK_H_

#include <pthread.h>

class AutoLock
{
    public:

    // constructor aquires the lock
    AutoLock(pthread_mutex_t& rMutex):m_mutex(rMutex)
    {
      pthread_mutex_lock(&m_mutex);
    }

    // destructor releases the lock
    ~AutoLock()
    {
      pthread_mutex_unlock(&m_mutex);
    }

    private:
    // just a temporary place holder, not intended
    // for permanent storage
    pthread_mutex_t& m_mutex;
    // No copy constructor
    AutoLock(const AutoLock&);
};

#endif
