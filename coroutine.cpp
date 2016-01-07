/*======================================================
    > File Name: coroutine.cpp
    > Author: MiaoShuai
    > E-mail:  
    > Other :  
    > Created Time: 2016年01月06日 星期三 15时12分35秒
 =======================================================*/
#include <ucontext.h>
#include <signal.h>
#include "coroutine.h"
#include <assert.h>
#include <sys/time.h>
#include <signal.h>
#include <functional>

using namespace mythread;

Mythread::Mythread(int stackSize)
    :stackSize_(stackSize),
    current_(0)
{
    
}

Mythread::~Mythread()
{
    for(auto &t : threadMap_)
    {
        free(t.second->uc_stack.ss_sp);   
    }
    threadMap_.clear(); //将map中的内容清空   
}

void Mythread::start(void)
{
    std::unique_ptr<ucontext_t> mainContextPtr(new ucontext_t);
    threadMap_.insert(std::pair<int,std::unique_ptr<ucontext_t>>(1,std::move(mainContextPtr)));//将主线程加入到map中

    signal(SIGALRM,std::bind(&Mythread::threadSchedule,this,std::placeholders::_1));
    
    struct itimerval tick;
    tick.it_value.tv_sec = 0;   //无定时器延时
    tick.it_value.tv_usec = 0;
    
    tick.it_interval.tv_sec = 0;
    tick.it_interval.tv_usec = 10000;   //每10毫秒切一次线程    
}

void Mythread::newThread(threadCallback cb,Args ...args)
{
    std::unique_ptr<ucontext_t> threadContext(new ucontext_t);
    threadContext->uc_stack.ss_sp = new char[stackSize_];
    threadContext->uc_stack.ss_size = stackSize_;
    threadContext->uc_link = 0;
    threadContext->uc_flags = 0;
    assert(threadContext->uc_stack.ss_sp != NULL);
    
    makecontext(threadContext,cb,args...);  //可能C++11多参与传统C不同
    
    threadMap_.insert(std::pair<int,std::unique_ptr<ucontext_t>>(makeThreadId(),std::move(threadContext)));
}

void Mythread::destroyThread(int id)
{
    free(threadMap_[id]->uc_stack.ss_sp);    //释放线程栈空间
    threadMap_.erase(id);   //将map中的线程上下文删除
}
 
int Mythread::makeThreadId(void)
{
    if((--threadMap_.end())->first != threadMap_.size())    //说明map中有未利用的id
    {
        for(int i = 1,auto it = threadMap_.begin(); it != threadMap_.end(); i++,it++)
        {
            if(i != it->first)
            {
                return i;   
            }
        }
    }
    else
    {
        return threadMap_.size() + 1;
    }

    return 0;
}

void Mythread::threadSchedule(int n)
{
    int lastId = current_;  //保存当前id
    if(current_ != (--threadMap_.end())->first)//当前线程不是map中的最后一个
    {
        auto it = threadMap_.find(current_);
        current_ = (++it)->first;
    }
    else
    {
        current_ = threadMap_.begin()->first;   //从map的起点开始
    }

    swapcontext(threadMap_[lastId].get(),threadMap_[current_].get());   //切换线程
}
