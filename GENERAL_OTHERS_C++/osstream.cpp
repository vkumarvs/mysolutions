#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::ostringstream

int main () {
  int x = 101;
  const char *par = "Hi My name is vipin";
  std::ostringstream oss;
  oss.str("");
  oss << "One hundred and one: " << x <<par;
  std::string s = oss.str();
  std::cout << s << '\n';
  return 0;
}
