#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
using namespace std;
bool checkDupDiffValues(string &s) {
    size_t pos = 0;
    std::string delimiter = ",";
    int tokenCnt=0;
    map<string, bool> uniqueTokens;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        //token = s.substr(0, pos);
        uniqueTokens.insert(std::pair<string, bool>(s.substr(0, pos),true));
        //s = s.substr(pos + delimiter.length());
        //std::cout << token << std::endl;
        //std::cout << s << std::endl;
        //s.erase(0, pos + delimiter.length());
        s.erase(0, s.find(delimiter) + delimiter.length());
        tokenCnt++;
    }
    // insert the last token
    uniqueTokens.insert(std::pair<string, bool>(s,true));
    tokenCnt++;
    s.clear();
    std::map<string, bool>::iterator it = uniqueTokens.begin();
    while(it != uniqueTokens.end()) {
        s.append(it->first);
        it++;
        if(it != uniqueTokens.end())
            s.append(",");
    }
    if(tokenCnt > uniqueTokens.size()) {
        cout << "found different values" << endl;
        return false;
    }
    return true;
}

int main() {
    std::string s = "3700,700,3700";
    checkDupDiffValues(s);
    std::cout << "Normalized string:" << s << std::endl;
    return 0;
}
