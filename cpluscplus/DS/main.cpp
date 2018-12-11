#include <stdio.h>
#include <stdint.h>
#include "DoubleListT.hpp"

class id : public DoubleListNode <id>
{
    uint32_t x;
public:
    void DeleteSelf() { } //do nothing here
};

class masterIndex : public DoubleListNode <masterIndex>
{
public:
    class id *m_Empty;
    class id *m_Allocated;
    masterIndex()
    {
      m_Empty = NULL;
      m_Allocated = NULL;
    }
    void DeleteSelf() { } //do nothing here
};

void initialize()
{
    for(uin32_t t = 1; t < 10; t++)
    {
      id *emptyId = new id();
      masterIndex *indexNode =  new masterIndex();
      indexNode->m_Empty

    }
}
int main()
{

    DoubleListT <id>  m_MasterIndexPool;
    DoubleListT <id>  m_EmptyIdPool;
    DoubleListT <id>  m_AllocatedIdPool;

    printf("size of m_IdPool is (%u)\n", (uint32_t)m_IdPool.Size());
    return 0;
}
