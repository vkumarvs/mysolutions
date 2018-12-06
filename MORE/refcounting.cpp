#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include <iostream>

using Poco::RefCountedObject;
using Poco::AutoPtr;
class RCO: public RefCountedObject {
    public:
        RCO() {
        }
        void greet() const {
            std::cout << "Hello, world!" << std::endl;
        }
    protected:
        ~RCO() {
            std::cout << "destroying RCO!" << std::endl;
        }
};

int main(int argc, char** argv)
{
    AutoPtr<RCO> pRCO = new RCO();
    std::cout << "refcount: " << pRCO->referenceCount() << std::endl;
    pRCO = new RCO();

#if 0
    pRCO->greet(); // AutoPtr has -> operator

    (*pRCO).greet(); // AutoPtr has * operator

    std::cout << "refcount: " << pRCO->referenceCount() << std::endl;

    RCO* p1 = pRCO; // AutoPtr supports conversion to plain pointer

    RCO* p2 = pRCO.get();
#endif
    return 0;
}
