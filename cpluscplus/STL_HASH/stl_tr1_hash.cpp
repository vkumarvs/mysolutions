#include <tr1/unordered_map>
#include<iostream>
#include <utility>
#include<stdint.h>
using namespace std;

// RAT types movik supports
#define MOVIK_RAT_NONE 0
#define MOVIK_RAT_EVDO 1
#define MOVIK_RAT_UMTS 2
#define MOVIK_RAT_LTE 3
#define MOVIK_RAT_NIP 4
#define MOVIK_RAT_MAX 5

struct SessionKey 
{
    uint64_t sessionId;
    uint8_t  subSessionId;
    uint8_t eRatType;

    explicit SessionKey(uint64_t userId=0, uint8_t sIndex=0, uint8_t eRat=MOVIK_RAT_NONE )
         : sessionId(userId), subSessionId(sIndex), eRatType(eRat) {}

    bool operator<(const SessionKey& rhs) const;
    bool operator==(const SessionKey& rhs) const;
    SessionKey& operator=(const SessionKey& rhs);
 
};

inline SessionKey& SessionKey::operator=(const SessionKey& rhs)                                                       
{   
    sessionId  = rhs.sessionId;
    subSessionId = rhs.subSessionId;                                                                                  
    eRatType = rhs.eRatType;                                                                                          
    return *this;                                                                                                     
}

inline bool SessionKey::operator<(const SessionKey& rhs) const                                                        
{   
    return (sessionId < rhs.sessionId) ? true :
           (eRatType < rhs.eRatType) ? true:
            (sessionId > rhs.sessionId) ? false : (subSessionId < rhs.subSessionId);
}
 
inline bool SessionKey::operator==(const SessionKey& rhs) const
{
    return sessionId == rhs.sessionId && subSessionId == rhs.subSessionId && eRatType == rhs.eRatType;
}
          
namespace std {                                                                                                       
    namespace tr1 {                                                                                                   
        template<>                                                                                                    
        struct hash<SessionKey> {                                                                                     
            size_t operator()(const SessionKey& key) const {                                                          
                return key.sessionId;                                                                                 
            }                                                                                                         
        };                                                                                                            
    }                                                                                                                 
}       

typedef tr1::unordered_map<SessionKey, int> SessionIDMapType;

int main()
{
    SessionIDMapType m_sessionIdMap;
    SessionKey first(1000, 5, 2);
    SessionKey second(1000, 6, 2);
    SessionKey third(1000, 5, 3);
    SessionKey fourth(2000, 5, 2);

    m_sessionIdMap.insert(std::make_pair<SessionKey, int>(first, 100));
    m_sessionIdMap.insert(std::make_pair<SessionKey, int>(second, 100));
    m_sessionIdMap.insert(std::make_pair<SessionKey, int>(third, 100));
    m_sessionIdMap.insert(std::make_pair<SessionKey, int>(fourth, 100));
    
    cout<<"size of"<<m_sessionIdMap.size()<<endl;
    SessionIDMapType::iterator it;
    it = m_sessionIdMap.begin();
    while(it != m_sessionIdMap.end())                                                                             
    { 
      int x  = (*it).second;
      cout<<x<<endl;
      it++;
    }            
    return 0;
}
