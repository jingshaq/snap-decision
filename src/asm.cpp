// asm.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#pragma   comment(lib,"User32.lib")

extern "C" void* EntryPointLoad();


typedef  void* (*malloc_t)(int);

int main()
{
   int* pszBuf=NULL;

    pszBuf = (int*)LocalAlloc(
              0x0040,
             4);

    std::cout << "init " << std::endl;
    auto localAllocAddr = EntryPointLoad();
    if (localAllocAddr != NULL) {
        printf("LocalAlloc address: %p==> \n", localAllocAddr);
            typedef void *(*FTT)(const char *);
            // auto f = ((FTT)(localAllocAddr))("User32.dll");
            // auto f = GetProcAddress((HMODULE)localAllocAddr,"LocalAlloc");
            // char *f = (char*)localAllocAddr;
            // f[0]='w';
            // f[1]='\n';
            // f[2]='\0';
            
            std::cout << "Hello World!+" <<std::endl;
    }
    else {
        printf("Failed to get LocalAlloc address.\n");
    }
    auto r = GetModuleHandleA("Kernel32.dll");
    // auto f = GetProcAddress(r,"LocalAlloc");
    // std::cout << "Hello World!"<< f <<std::endl;
    std::cout << "Hello World!" <<std::endl;
    FindWindowA(NULL,NULL);
    return 0;
}

 