// string::substr
#include <iostream>
#include <string>

int main ()
{
  std::string str="We think in generalities, but we live in details.";
                                           // (quoting Alfred N. Whitehead)

  std::string str2 = str.substr (13);   // "generalities"

 // unsigned pos = str.find("live");         // position of "live" in str

  //std::string str3 = str.substr (pos);     // get from "live" to the end

  //std::cout << str2 << ' ' << str3 << '\n';
  std::cout << str2 <<'\n';

  return 0;
}
