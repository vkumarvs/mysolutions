//http://www.codeproject.com/Articles/23396/A-High-Performance-Multi-Threaded-LRU-Cache
//http://www.geeksforgeeks.org/implement-lru-cache/

 /// Typedef for URL/Entry pair
    typedef std::pair< std::string, Entry > EntryPair;

    /// Typedef for Cache list
    typedef std::list< EntryPair > CacheList;

    /// Typedef for URL-indexed map into the CacheList
    typedef boost::unordered_map< std::string, CacheList::iterator > CacheMap;

    /// Cache LRU list
    CacheList mCacheList;

    /// Cache map into the list
    CacheMap mCacheMap;


/ create new entry
Entry iEntry( ... );

// push it to the front;
mCacheList.push_front( std::make_pair( aURL, iEntry ) );

// add it to the cache map
mCacheMap[ aURL ] = mCacheList.begin();

// increase count of entries
mEntries++;

// check if it's time to remove the last element
if ( mEntries > mMaxEntries )
{
    // erease from the map the last cache list element
    mCacheMap.erase( mCacheList.back().first );

    // erase it from the list
    mCacheList.pop_back();

    // decrease count
    mEntries--;
}
