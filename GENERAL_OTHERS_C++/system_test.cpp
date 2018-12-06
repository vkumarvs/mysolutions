#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main()
{
    char cmd[1024];
    //snprintf(cmd, sizeof(cmd), "ping 192.168.112.89");
    //snprintf(cmd, sizeof(cmd), "ping 127.0.0.1 -c 1");
    snprintf(cmd, sizeof(cmd), "ssh -n vipin@169.254.91.1");
    //snprintf(cmd, sizeof(cmd), "ssh -n vipin@127.0.0.1");
    int status = 0;
    status = system(cmd);
    if (WIFEXITED(status))
    {
      printf("\n\nhi my name is vipin*******\n\n\n");
      if (WEXITSTATUS(status))
      {
        printf("failed to exe cmd [%s] status[%d]\n",
                 cmd, WEXITSTATUS(status));
        return WEXITSTATUS(status);
      }
    }
    else
    {
      printf("failed to exe cmd [%s] \n",cmd);
      return -1;
    }
    return 0;
}
