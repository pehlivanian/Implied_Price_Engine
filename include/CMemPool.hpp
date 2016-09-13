#ifndef __CMEMPOOL_HPP__
#define __CMEMPOOl_HPP__

#include <stdlib.h>
#include <iostream>

class CMemPool
{
 private:
   struct _Unit
     {
	struct _Unit *pPrev, *pNext;
     };
   
   void* m_pMemBlock;
   
   struct _Unit* m_pAllocatedMemBlock;
   struct _Unit* m_pFreeMemBlock;
   
   unsigned long m_ulBlockSize;
   unsigned long m_ulUnitSize;
   
 public:
   CMemPool(unsigned long lUnitNum = 50, unsigned long lUnitSize = 1024);
  ~CMemPool();
   
   void* allocate(size_t ulSize);
   void deallocate(void*);
		    
};

#endif
