#include <stdio.h>
#include <string>
#include <iostream>
#include <map>
#include <string.h>
using namespace std;

#define GROUP_SEPARATOR_QUESTION_MARK   "?"

int main()
{
   for (int x = 0; x < 100; x++)
{
    std::string grpNames = "";
    char grpNamesArr[256];
 //   memset(grpNamesArr,0,256);
    strncpy(grpNamesArr, grpNames.c_str(), 255);                                                                      
    char *pGroupName = strtok(grpNamesArr, GROUP_SEPARATOR_QUESTION_MARK);                                            
    while(pGroupName)                                                                                                 
    { 
      pGroupName = strtok(NULL, GROUP_SEPARATOR_QUESTION_MARK);                                                     
    }
    printf("groupname %s \n", pGroupName); 
}
    return 0;
}
