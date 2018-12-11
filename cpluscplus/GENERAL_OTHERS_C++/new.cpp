#include<iostream>
#include<stdint.h>
#include<stdio.h>
using namespace std;
#define MAX_LINKS_IN_LINKSET 16
typedef struct mtp3_link_prop
{     
    uint8_t id;
    int8_t prio;
    uint8_t state;
}mtp3_link_prop_st;
int main()
{
    mtp3_link_prop_st *pLink;
    mtp3_link_prop_st links[MAX_LINKS_IN_LINKSET];
    for(int x=0; x<16; x++)
    {
      links[x].id = 1;
      cout<< "link id is "<< links[x].id <<endl;
      printf("link id is %d \n",links[x].id);
    }
    pLink = new mtp3_link_prop_st[MAX_LINKS_IN_LINKSET];
    memcpy(pLink,links,sizeof(links));
    for(int t=0; t<16; t++)
    {
      cout<< "pLink id is "<<pLink[t].id <<endl;
      printf("pLink id is %d \n",pLink[t].id);
    }

    return 0;
}
