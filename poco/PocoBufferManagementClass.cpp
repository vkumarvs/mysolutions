#include "Poco/RefCountedObject.h"
#include "Poco/MemoryPool.h"
#include "Poco/AutoPtr.h"

using namespace Poco;
using Poco::AutoPtr;

class buffer : public RefCountedObject {
    uint8_t buf[PKT_BUFFER_SIZE];
    static Buffer_Stats stats;
    static MemoryPool pkt_buffers;
protected:
    ~buffer() {};
    buffer();
public:
    uint8_t * getBufferPtr() {return buf;};
    uint32_t getBufferSize() {return sizeof(buf);};
    static void operator delete (void * p);
    static buffer * getPktBuffer();
    static Buffer_Stats * getStats() {return &stats;};
};

typedef AutoPtr<buffer> APBuffer;


int main()
{
    buffer * head_ptr = buffer::getPktBuffer();
    uint8_t * pkt_ptr = head_ptr->getBufferPtr();
    memcpy(pkt_ptr,payload_ptr,pkt_size);
    return 0;
}
