#pragma once
#ifndef _CPP_backstack_BACKSTACK_H__
#define _CPP_backstack_BACKSTACK_H__

#define PrintBackStack(MSG) backstack::ShowTraceStack(MSG)

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

#elif defined(linux)
#include <cxxabi.h>
#include <execinfo.h>
#define UNW_LOCAL_ONLY
#include <libunwind.h>
#include <stdio.h>

#include <cstring>
#endif

namespace backstack {

inline const char* ShowTraceStack(char* szBriefInfo) {
#ifdef DISABLE_BACKSTACK_APPLICATION
    return "";
#else
#ifdef _WIN32
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
#elif defined(linux)
    static char szStackInfo[1024 * 12];
    static char szFrameInfo[1024];
    strcpy(szStackInfo, szBriefInfo == NULL ? "stack traceback:\n" : szBriefInfo);
    unw_cursor_t cursor;
    unw_context_t context;

    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    while (unw_step(&cursor) > 0) {
        unw_word_t offset, pc;
        char fname[64];

        unw_get_reg(&cursor, UNW_REG_IP, &pc);

        fname[0] = '\0';
        (void)unw_get_proc_name(&cursor, fname, sizeof(fname), &offset);
        int status;
        char* demangled = abi::__cxa_demangle(fname, nullptr, nullptr, &status);
        if (status == 0) {
            char tmp_str[1024];
            sprintf(tmp_str, "\t%s\n", demangled);
            strcat(szFrameInfo, tmp_str);
        }
    }
    strcat(szStackInfo, szFrameInfo);
    return szStackInfo;
#endif
#endif
}

}  // namespace backstack

#endif  //  _CPP_backstack_BACKSTACK_H__
