#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;

bool getBoolean(const std::string& str)
{
    if (str == "true") return true;
    if (str == "playready" || str == "sample_aes" || str == "aes" ||
        str == "playready_envelope" || str == "sample_aes_streamingkeydelivery") {
        return true;
    }
    if (str.empty() || str == "false") return false;

    // Unknown value
    return false;
}

int main()
{
    //string myname = "playready_envelope";
    string myname = "aes";
    if(getBoolean(myname)) {
        printf("I am returning true;\n");
    } else {
        printf("I am returning false;\n");
    }
    return 0;
}

