#include <iostream>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>

using namespace std;
using namespace boost;
using namespace boost::multi_index; 
using  boost::multi_index_container;

class UserpageLogType
{
public:
    std::string GetUser() const {return user;}
    int GetTimestamp() const {return timestamp;}
private:
    std::string user; 
    int url; 
    int timestamp; 
};

typedef multi_index_container<
    UserpageLogType*,
    indexed_by <
        //ordered_unique <
        ordered_non_unique<
            composite_key <
                UserpageLogType,
                const_mem_fun< UserpageLogType, std::string , &UserpageLogType::GetUser>,
                const_mem_fun< UserpageLogType, int , &UserpageLogType::GetTimestamp>
           >
        >
   >
> CspmSessionContainer;

int main()
{
    return 0;
}
/* Sessions ordered by IMSI-Index */
//typedef CspmSessionContainer::nth_index<0>::type UserInSequence;

#if 0
   const CspmSessionSetByImplicitType& getSessionSetByImplicit()
   {
      CspmSessionSetByImplicitType& set = m_sessContainer.get<2>();
      return set;
   }
    SpmSession *getImplicitSession(const SessionKey &sessionId)
    {
      CspmSessionSetByImplicitType::iterator it;

      AutoLock lock(m_sessContainerLock);
      /* find Session with invalid IP */
      it = getSessionSetByImplicit().find(boost::make_tuple(false, (SessionRatType)sessionId.GetRatType(), sessionId.GetUid(), sessionId.GetSessionIndex()));
      if (it != getSessionSetByImplicit().end())
      {
        SpmSession * pSession = (*it);
        return pSession;
      }
      return NULL;
    }
#endif
#if 0
    pair <CspmSessionContainer::iterator, bool> insertRet =
        m_sessContainer.insert(pdpSession);

    if (!insertRet.second)
    {
        LogError("Failed to add Session:%"PRIu64" - %d\n", _sessId, _subSessId);
        return false;
    }
#endif
