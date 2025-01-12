#pragma once
#include <stdint.h>
#include <stddef.h>

namespace Heap
{
    class HeapManager;

    struct _HeapSegHdr
    {
        size_t length;
        _HeapSegHdr* next;
        _HeapSegHdr* last;
        const char* text = "<DEF>";
        int64_t activeMemFlagVal = 0;
        const char* file = "<DEF>";
        const char* func = "<DEF>";
        int line = 0;
        uint64_t time = 0;
        bool free;
        void CombineForward(HeapManager* manager);
        void CombineBackward(HeapManager* manager);
        _HeapSegHdr* Split(HeapManager* manager, size_t splitLength);
        uint64_t magicNum;
    };

    class HeapManager
    {
        public:
        int64_t _heapCount;

        int64_t _usedHeapCount;
        int64_t _usedHeapAmount;
        int64_t _activeMemFlagVal;

        void* _heapStart;
        void* _heapEnd;

        _HeapSegHdr* _lastHdr;

        void SubInitHeap(void* heapAddress, size_t pageCount);
        void InitializeHeap(int pageCount);


        void* _Xmalloc(int64_t size,  const char* text);
        void* _Xmalloc(int64_t size, const char* func, const char* file, int line);
        void* _Xmalloc(int64_t size, const char* text, const char* func, const char* file, int line);

        void _Xfree(void* address);
        void _Xfree(void* address, const char* func, const char* file, int line);
        bool _XtryFree(void* address, const char* func, const char* file, int line);

        bool ExpandHeap(size_t length);

        bool HeapCheck(bool wait);
    };

    extern HeapManager* GlobalHeapManager;
}


#ifndef _KERNEL_SRC

// get number of arguments with __NARG__
#define __NARG__(...)  __NARG_I_(__VA_ARGS__,__RSEQ_N())
#define __NARG_I_(...) __ARG_N(__VA_ARGS__)
#define __ARG_N( \
      _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
     _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
     _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
     _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
     _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
     _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
     _61,_62,_63,N,...) N
#define __RSEQ_N() \
     63,62,61,60,                   \
     59,58,57,56,55,54,53,52,51,50, \
     49,48,47,46,45,44,43,42,41,40, \
     39,38,37,36,35,34,33,32,31,30, \
     29,28,27,26,25,24,23,22,21,20, \
     19,18,17,16,15,14,13,12,11,10, \
     9,8,7,6,5,4,3,2,1,0

// general definition for any function name
#define _VFUNC_(name, n) name##n
#define _VFUNC(name, n) _VFUNC_(name, n)
#define VFUNC(func, ...) _VFUNC(func, __NARG__(__VA_ARGS__)) (__VA_ARGS__)

// definition for FOO
#define _Malloc(...) VFUNC(_Malloc, __VA_ARGS__)



#define _Malloc1(size) Heap::GlobalHeapManager->_Xmalloc(size, __PRETTY_FUNCTION__, __FILE__, __LINE__)
#define _Malloc2(size, text) Heap::GlobalHeapManager->_Xmalloc(size, text, __PRETTY_FUNCTION__, __FILE__, __LINE__)
#define _Free(address) Heap::GlobalHeapManager->_Xfree((void*)address, __PRETTY_FUNCTION__, __FILE__, __LINE__)
#define _TryFree(address) Heap::GlobalHeapManager->_XtryFree((void*)address, __PRETTY_FUNCTION__, __FILE__, __LINE__)


#include "new.hpp"

#endif