/*======================================================
    > File Name: main.cpp
    > Author: MiaoShuai
    > E-mail:  
    > Other :  
    > Created Time: 2016年01月06日 星期三 18时11分35秒
 =======================================================*/

#include <iostream>
#include <vector>
#include <string>
#include "coroutine.h"
#include <unistd.h>

void func(void)
{
    printf("hello\n");
}

int main(int argc,char **argv)
{
  mythread::Mythread t;
  t.start();
  t.newThread(func);
  sleep(1000);
  return 0;
}
