#include <iostream>
#include <string>
#include <stdio.h>
#include <list>
#include <vector>
#include <stdint.h>
using namespace std;

#define __mv_jhash_mix(a, b, c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8); \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12);  \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}

/* The golden ration: an arbitrary value */
#define MV_JHASH_GOLDEN_RATIO	0x9e3779b9

/* The most generic version, hashes an arbitrary sequence
 * of bytes.  No alignment or length assumptions are made about
 * the input key.
 */
static inline uint32_t mv_jhash(const void *key, uint32_t length, uint32_t initval)
{
	uint32_t a, b, c, len;
	const uint8_t *k = (const uint8_t *)key;

	len = length;
	a = b = MV_JHASH_GOLDEN_RATIO;
	c = initval;

	while (len >= 12) {
		a += (k[0] +((uint32_t)k[1]<<8) +((uint32_t)k[2]<<16) +((uint32_t)k[3]<<24));
		b += (k[4] +((uint32_t)k[5]<<8) +((uint32_t)k[6]<<16) +((uint32_t)k[7]<<24));
		c += (k[8] +((uint32_t)k[9]<<8) +((uint32_t)k[10]<<16)+((uint32_t)k[11]<<24));

		__mv_jhash_mix(a,b,c);

		k += 12;
		len -= 12;
	}

	c += length;
	switch (len) {
	case 11: c += ((uint32_t)k[10]<<24);
	case 10: c += ((uint32_t)k[9]<<16);
	case 9 : c += ((uint32_t)k[8]<<8);
	case 8 : b += ((uint32_t)k[7]<<24);
	case 7 : b += ((uint32_t)k[6]<<16);
	case 6 : b += ((uint32_t)k[5]<<8);
	case 5 : b += k[4];
	case 4 : a += ((uint32_t)k[3]<<24);
	case 3 : a += ((uint32_t)k[2]<<16);
	case 2 : a += ((uint32_t)k[1]<<8);
	case 1 : a += k[0];
	};

	__mv_jhash_mix(a,b,c);

	return c;
}

typedef list<string *> HashNode;

#if 1
class HashMap 
{
public:
    HashMap(int size);
    ~HashMap();
    void addElem(string *value);
    void removeElem(){};
    void PrintElementsAtIndex(int hashIdx);
    uint32_t getHashIdx(const char *key, uint32_t keyLength);
    void resizeHash(int hashsize){};
private:
    vector<HashNode*> m_hashTable;
};

uint32_t HashMap::getHashIdx(const char *key, uint32_t keyLength)
{
  uint32_t hashedVal = mv_jhash(key, keyLength, 10);
  printf("hashed value is %u\n", hashedVal);
  return hashedVal;
}

void HashMap::addElem(string *value)
{
    uint32_t hashIdx = getHashIdx(value->c_str(), value->length()); 
    if(hashIdx > m_hashTable.size())
    {
      hashIdx = hashIdx%100;
    }
    HashNode *node = NULL;
    if(m_hashTable.at(hashIdx) != NULL)
    {
      node = m_hashTable.at(hashIdx);
      printf("collision happened -- creating chain\n");
      node->push_front(value); 
    }
    else
    {
      node = new HashNode();
      node->push_front(value); 
      m_hashTable.at(hashIdx)=node;
    }
}

HashMap::HashMap(int size)
{
    m_hashTable.resize(size);
    printf("sizing hash to %d element\n", size);
    for(int x =0; x < size; x++)
    {
      m_hashTable.at(x) = NULL;
    }
}

HashMap::~HashMap()
{
}

void HashMap::PrintElementsAtIndex(int _hashIdx)
{
    string value("my name is vipin");
    uint32_t hashIdx = getHashIdx(value.c_str(), value.length()); 
    if(hashIdx > m_hashTable.size())
    {
      hashIdx = hashIdx%100;
    }
    HashNode *node = m_hashTable.at(hashIdx);
    HashNode::iterator it;
    for(it = node->begin(); it != node->end(); it++)
    {
      printf("%s\n", (*it)->c_str());
    } 
}

int main()
{
  HashMap hash(100);
  string *value1 = new string("my name is vipin");
  string *value2 = new string("i love myself");
  hash.addElem(value1);
  hash.addElem(value2);
  hash.PrintElementsAtIndex(1);
return 0;
}

#else
// vector::at
#include <iostream>
#include <vector>
using namespace std;

int main ()
{
  vector<int> myvector (10);   // 10 zero-initialized ints
  unsigned int i;

  // assign some values:
  for (i=0; i<myvector.size(); i++)
    myvector.at(i)=i;

  cout << "myvector contains:";
  for (i=0; i<myvector.size(); i++)
    cout << " " << myvector.at(i);

  cout << endl;

  return 0;
}
#endif
