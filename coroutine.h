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

namespace mythread
{
    class Mythread
    {
        public:
            Mythread(int stackSize = 10240);
            ~Mythread();
            void start(void);
            template <typename threadCallback,typename ...Args>
            void newThread(threadCallback cb,Args ...args);  //创建新线程
            void destroyThread(int id);  //销毁某个线程

        private:
            void threadSchedule(int n); //线程调度函数
            int makeThreadId(void);     //生成一个线程id
            std::map<int,std::unique_ptr<ucontext>> threadMap_;  //保存线程id及其对应的线程上下文内容
            int current_;                       //保存当前线程id
            int stackSize_;             //每个线程的栈空间的大小
            int maxThreadNumber_;               //最大可创建线程数
    };
}
