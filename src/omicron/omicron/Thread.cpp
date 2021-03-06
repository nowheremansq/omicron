/******************************************************************************
* THE OMICRON SDK
*-----------------------------------------------------------------------------
* Copyright 2010-2015		Electronic Visualization Laboratory,
*							University of Illinois at Chicago
* Authors:
*  Alessandro Febretti		febret@gmail.com
*-----------------------------------------------------------------------------
* Copyright (c) 2010-2015, Electronic Visualization Laboratory,
* University of Illinois at Chicago
* All rights reserved.
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer. Redistributions in binary
* form must reproduce the above copyright notice, this list of conditions and
* the following disclaimer in the documentation and/or other materials provided
* with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*-----------------------------------------------------------------------------
* What's in this file:
* Exposes a few small wrapper classes around tinythreads, that implement basic
* multithreading support.
******************************************************************************/
#include "omicron/Thread.h"

#include "tinythread/tinythread.h"

using namespace omicron;

///////////////////////////////////////////////////////////////////////////////
Lock::Lock()
{
    myLockImpl = new tthread::mutex();
}

///////////////////////////////////////////////////////////////////////////////
Lock::~Lock()
{
    delete myLockImpl;
    myLockImpl = NULL;
}

///////////////////////////////////////////////////////////////////////////////
void Lock::lock()
{
    myLockImpl->lock();
}

///////////////////////////////////////////////////////////////////////////////
void Lock::unlock()
{
    myLockImpl->unlock();
}

///////////////////////////////////////////////////////////////////////////////
void threadProcWrapper(void* threadData)
{
    ((Thread*)threadData)->threadProc();
}

///////////////////////////////////////////////////////////////////////////////
Thread::Thread():
    myThreadImpl(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////
Thread::~Thread()
{
}

///////////////////////////////////////////////////////////////////////////////
void Thread::start()
{
    // Make sure the thread is not running already.
    stop();

    if(myThreadImpl == NULL)
    {
        myThreadImpl = new tthread::thread(threadProcWrapper, this);
    }
}

///////////////////////////////////////////////////////////////////////////////
void Thread::stop()
{
    if(myThreadImpl != NULL)
    {
        myThreadImpl->join();
        delete myThreadImpl;
        myThreadImpl = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
ThreadEvent::ThreadEvent()
{
    myCVImpl = new tthread::condition_variable();
}

///////////////////////////////////////////////////////////////////////////////
ThreadEvent::~ThreadEvent()
{
    delete myCVImpl;
    myCVImpl = NULL;
}

///////////////////////////////////////////////////////////////////////////////
void ThreadEvent::signal()
{
    AutoLock al(myLock);
    myCVImpl->notify_all();
}

///////////////////////////////////////////////////////////////////////////////
void ThreadEvent::signalOne()
{
    AutoLock al(myLock);
    myCVImpl->notify_one();
}

///////////////////////////////////////////////////////////////////////////////
void ThreadEvent::wait()
{
    AutoLock al(myLock);
    myCVImpl->wait(*myLock.impl());
}