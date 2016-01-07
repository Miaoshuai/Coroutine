/*======================================================
    > File Name: coroutine.h
    > Author: MiaoShuai
    > E-mail:  
    > Other :  
    > Created Time: 2016年01月05日 星期二 14时43分17秒
 =======================================================*/
#pragma once

#include <functional>
#include <ucontext.h>
#include <map>
#include <memory>
#include <assert.h>
#include <stdio.h>

namespace mythread
{
    class Mythread
    {
        public:
            Mythread(int stackSize = 10240);
            ~Mythread();
            void start(void);

            template <typename threadCallback,typename ...Args>
            void newThread(threadCallback cb,Args ...args)  //创建新线程
            {
                printf("222\n");
                std::unique_ptr<ucontext_t> threadContext(new ucontext_t);
                threadContext->uc_stack.ss_sp = new char[stackSize_];
                threadContext->uc_stack.ss_size = stackSize_;
                threadContext->uc_link = 0;
                threadContext->uc_flags = 0;
                assert(threadContext->uc_stack.ss_sp != NULL);
                makecontext(threadContext.get(),cb,sizeof...(args),args...);  //可能C++11多参与传统C不同
                printf("222\n");
                threadMap_.insert(std::pair<int,std::unique_ptr<ucontext_t>>(makeThreadId(),std::move(threadContext)));
                printf("333\n"); 
            }

            void destroyThread(int id);  //销毁某个线程

        private:
            static void threadSchedule(int n); //线程调度函数
            int makeThreadId(void);     //生成一个线程id
            static std::map<int,std::unique_ptr<ucontext_t>> threadMap_;  //保存线程id及其对应的线程上下文内容
            static int current_;                       //保存当前线程id
            int stackSize_;             //每个线程的栈空间的大小
            int maxThreadNumber_;               //最大可创建线程数
    };
}
