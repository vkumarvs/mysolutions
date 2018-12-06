#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

struct Key
{
  std::string first;

  bool operator==(const Key &other) const
  {
    return (first == other.first);
  }
};

struct KeyHasher
{
  std::size_t operator()(const Key& k) const
  {
    using std::hash;

    return (hash<string>()(k.first));
  }
};

int main()
{
  std::unordered_map<Key,std::string,KeyHasher> m6 = {
    { {"John"}, "example"},
    { {"Mary"}, "another"}
  };
}
