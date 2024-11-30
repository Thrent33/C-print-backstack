#pragma once
#ifndef _CPP_backstack_BACKSTACK_H__
#define _CPP_backstack_BACKSTACK_H__

#define PrintBackStack(MSG) backstack::ShowTraceStack(MSG)

#include <string>

#ifdef _WIN32
// clang-format off
#include <stdio.h>
#include <windows.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
// clang-format on

#if _MSC_VER
#define snprintf _snprintf
#endif

#define STACK_INFO_LEN 1024
#endif

namespace backstack {

#ifdef _WIN32
const char* ShowTraceStack(char* szBriefInfo) {
    static const int MAX_STACK_FRAMES = 12;
    void* pStack[MAX_STACK_FRAMES];
    static char szStackInfo[STACK_INFO_LEN * MAX_STACK_FRAMES];
    static char szFrameInfo[STACK_INFO_LEN];

    HANDLE process = GetCurrentProcess();
    SymInitialize(process, NULL, TRUE);
    WORD frames = CaptureStackBackTrace(0, MAX_STACK_FRAMES, pStack, NULL);
    strcpy_s(szStackInfo, szBriefInfo == NULL ? "stack traceback:\n" : szBriefInfo);

    for (WORD i = 0; i < frames; ++i) {
        DWORD64 address = (DWORD64)(pStack[i]);

        DWORD64 displacementSym = 0;
        char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
        PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;
        pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        pSymbol->MaxNameLen = MAX_SYM_NAME;

        DWORD displacementLine = 0;
        IMAGEHLP_LINE64 line;
        line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

        if (SymFromAddr(process, address, &displacementSym, pSymbol) &&
            SymGetLineFromAddr64(process, address, &displacementLine, &line)) {
            snprintf(szFrameInfo, sizeof(szFrameInfo), "\t%s() at %s:%d(0x%llx)\n", pSymbol->Name, line.FileName,
                     line.LineNumber, pSymbol->Address);
        } else {
            snprintf(szFrameInfo, sizeof(szFrameInfo), "\terror: %d\n", GetLastError());
        }
        strcat_s(szStackInfo, szFrameInfo);
    }

    return szStackInfo;
}
#endif

}  // namespace backstack

#endif  //  _CPP_backstack_BACKSTACK_H__
