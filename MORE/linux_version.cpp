#include <sstream>
#include <string.h>
#include <unistd.h>
#include <string>
#include <stdio.h>
using namespace std;

void GetCommandResult(string &cmd, string &result)
{
    FILE* pipe = NULL;
    pipe = popen(cmd.c_str(), "r");
    if (!pipe) return ;
    char buffer[128];
    memset(buffer,0,128);
    while(!feof(pipe)) {
      if(fgets(buffer, 128, pipe) != NULL)
        result += buffer;
    }
    pclose(pipe);
    //trim any left and right space due to excessive buffer
    std::stringstream trimmer;
    trimmer << result;
    trimmer >> result;

}

std::string CheckForVersion()
{
    FILE* pipe = NULL;
    string result = "";
    std::string platform = "which yum apt-get zypper 2";
    std::string cmd = "";
    GetCommandResult(platform, result);
    printf("platform result is %s \n", result.c_str());
    size_t pos1 = result.find_last_of('/');
    result  = result.substr(pos1+1);
    printf("2.platform result is %s \n", result.c_str());
    string m_version;
    if(result.compare("yum") == 0)
    {
      cmd = "cat /etc/redhat-release | grep -o '[0-9].[0-9]'";
      result.clear();
      GetCommandResult(cmd, result);
      if((result.compare("6.5") == 0) || ((result.compare("6.6")==0) ))
      {

        m_version="centosversion=1.6.6";
      }
      else
      {
        m_version="centosversion=1.7.11";
      }
    }
    else if(result.compare("apt-get") == 0)
    {
      cmd = "lsb_release -sr | awk '{print substr($0,0,3)}'";
      result.clear();
      GetCommandResult(cmd, result);
      if((result.compare("14") == 0))
      {

        m_version="ubuntu 14";
      }
      else
      {
        m_version="ubuntu 12";
      }
    }
    else
    {
      return "invalid";
    }

    return m_version;
}

int main()
{
   printf("Linux version is %s \n", CheckForVersion().c_str());
   return 0;
}

