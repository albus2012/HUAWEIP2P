#include"Thread.h"
#include"CurrentThread.h"

#include<boost/static_assert.hpp>
#include<boost/type_traits/is_same.hpp>

#include<errno.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<sys/types.h>
#include<linux/unistd.h>

namespace huaweip2p
{

namespace CurrentThread
{
  __thread int t_cachedTid = 0;
  __thread char t_tidString[32];
  __thread const char* t_threadName = "unknown";
  const bool sameType = boost::is_same<int, pid_t>::value;
  BOOST_STATIC_ASSERT(sameType);
}
 
pid_t gettid()
{
  return static_cast<pid_t>(::syscall(SYS_gettid));
}

void afterFork()
{
  huaweip2p::CurrentThread::t_cachedTid = 0;
  huaweip2p::CurrentThread::t_threadName = "main";
  CurrentThread::tid();
}

class ThreadNameInitializer
{
 public:
  ThreadNameInitializer()
  {
    huaweip2p::CurrentThread::t_threadName = "main";
    CurrentThread::tid();
    pthread_atfork(NULL, NULL, &afterFork);
  }
};

ThreadNameInitializer init;
}

using namespace huaweip2p;

void CurrentThread::cacheTid()
{
  if(t_cachedTid == 0)
  {
    t_cachedTid = huaweip2p::gettid();
    int n = snprintf(t_tidString, sizeof(t_tidString), "%5d ", t_cachedTid);
    assert(n == 6);
    (void) n;
  }
}

bool CurrentThread::isMainThread()
{ return tid() == ::getpid(); }
  
AtomicInt32 Thread::numCreated_;

Thread::Thread(const ThreadFunc& func, const string& n)
  : started_(false),
    pthreadId_(0),
    tid_(0),
    func_(func),
    name_(n)
{ numCreated_.increment(); }

Thread::~Thread() { }

void Thread::start()
{
  assert(!started_);
  started_ = true;
  errno = pthread_create(&pthreadId_, NULL, &startThread, this);
  if(errno != 0)
  {
    printf("Failed in pthread_create");
  }
}

int Thread::join()
{
  assert(started_);
  return pthread_join(pthreadId_, NULL);
}

void* Thread::startThread(void* obj)
{
  Thread* thread = static_cast<Thread*>(obj);
  thread->runInThread();
  return NULL;
}

void Thread::runInThread()
{
  tid_ = CurrentThread::tid();
  huaweip2p::CurrentThread::t_threadName = name_.c_str();
  try
  {
    func_();
    huaweip2p::CurrentThread::t_threadName = "finished";
  }
  catch (...)
  {
    huaweip2p::CurrentThread::t_threadName = "crashed";
    fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
    throw;
  }
}



