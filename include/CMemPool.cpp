#include "CMemPool.hpp"

CMemPool::CMemPool(unsigned long ulUnitNum, unsigned long ulUnitSize) : 
  m_pMemBlock(NULL), m_pAllocatedMemBlock(NULL), m_pFreeMemBlock(NULL), 
  m_ulBlockSize(ulUnitNum * (ulUnitSize + sizeof(struct _Unit))),
  m_ulUnitSize(ulUnitSize)
{
   m_pMemBlock = malloc(m_ulBlockSize); 
   if (nullptr != m_pMemBlock)
     {
	for(unsigned long i=0; i<ulUnitNum; i++)
	  {
	     struct _Unit *pCurUnit = (struct _Unit *)( (char *)m_pMemBlock + i *
	        (ulUnitSize * sizeof(struct _Unit)));
	     pCurUnit->pPrev = nullptr;
	     pCurUnit->pNext = m_pFreeMemBlock;
	     if (nullptr != m_pFreeMemBlock)
	       {
		  m_pFreeMemBlock->pPrev = pCurUnit;
	       }
	     m_pFreeMemBlock = pCurUnit;
	  }	
     }
}

CMemPool::~CMemPool()
{
   free(m_pMemBlock);
}

void *CMemPool::allocate(size_t ulSize)
{
  bool bUseMemPool = true;
   if (ulSize > m_ulUnitSize || false == bUseMemPool ||
       NULL == m_pMemBlock || NULL == m_pFreeMemBlock)
     {
	return malloc(ulSize);
     }
   
   struct _Unit *pCurUnit = m_pFreeMemBlock;
   m_pFreeMemBlock = pCurUnit->pNext;
   if(NULL != m_pFreeMemBlock)
     {
	m_pFreeMemBlock->pPrev = NULL;
     }
   
   pCurUnit->pNext = m_pAllocatedMemBlock;
   
   if (NULL != m_pAllocatedMemBlock)
     {
	m_pAllocatedMemBlock->pPrev = pCurUnit;
     }
   m_pAllocatedMemBlock = pCurUnit;
   
   
   return (void *)((char *)pCurUnit + sizeof(struct _Unit));
   
}

void CMemPool::deallocate(void* p)
{
   if (m_pMemBlock < p && p < (void *)((char *)m_pMemBlock + m_ulBlockSize ))
     {
	struct _Unit *pCurUnit = (struct _Unit *)((char *)p - sizeof(struct _Unit));
	m_pAllocatedMemBlock = pCurUnit->pNext;
	if(NULL != m_pAllocatedMemBlock)
	  {
	     m_pAllocatedMemBlock->pPrev = NULL;
	  }
        pCurUnit->pNext = m_pFreeMemBlock;
	if(NULL != m_pFreeMemBlock)
	  {
	     m_pFreeMemBlock->pPrev = pCurUnit;
	  }
	m_pFreeMemBlock = pCurUnit;
     }
   else 
     {
	free(p);
     }
}
