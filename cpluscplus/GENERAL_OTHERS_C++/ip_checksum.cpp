#include<stdio.h>
#include<iostream>
#include<stdint.h>
/*
Before changing IP header 

45  0   0   54  0   0   40  0   40  1   25  a8  a   0   0   
1   b   0   0   1   
 
 
After changing IP header 

45  0   0   54  0   0   40  0   40  1   25  a8  a   0   0   
1   14  0   0   1   
*/
typedef unsigned short u16;
typedef unsigned long u32;

u16 ip_sum_calc(u16 len_ip_header, u16 buff[])
{
u16 word16;
u32 sum=0;
u16 i;
    
    // make 16 bit words out of every two adjacent 8 bit words in the packet
    // and add them up
    for (i=0;i<len_ip_header;i=i+2){
        word16 =((buff[i]<<8)&0xFF00)+(buff[i+1]&0xFF);
        sum = sum + (u32) word16;   
    }
    
    // take only 16 bits out of the 32 bit sum
    while (sum>>16)
      sum = (sum & 0xFFFF)+(sum >> 16);

    // one's complement the result
    sum = ~sum;
    
return ((u16) sum);
}

struct iphdr {
    //unsigned int ihl : 4;
    //unsigned int version : 4;
    uint8_t x;
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
};
#include <sys/types.h>
#include <netinet/in.h>

typedef unsigned int IpAddress;
typedef unsigned short TcpUdpPort;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
unsigned short checksum (const char* buf, unsigned short buflen)
{
       uint sum=0;
       unsigned short index = 0;

       // make 16 bit words out of every two adjacent 8 bit words in the packet
       // and add them up
       for (index=0;index<buflen;index=index+2)
       {
                ushort word16 =((buf[index]<<8)&0xFF00)+(buf[index+1]&0xFF);
       // printf("%08x %04x %08x\n", sum, word16, sum + word16);
                sum = sum + (uint) word16;
       }

        // take only 16 bits out of the 32 bit sum and add up the carries
        while (sum>>16)
          sum = (sum & 0xFFFF)+(sum >> 16);

        // one's complement the result
        sum = ~sum; 

        return htons(((ushort) sum));
}

int main ()
{
iphdr iph;
iph.x = 0x45;
iph.tos =0x00; 
iph.tot_len =0x0054;
iph.id=0x0000;
iph.frag_off=0x4000;
iph.ttl=0x40;
iph.protocol=0x01;
iph.check=0x25a8;
iph.saddr=0x0a000001;
iph.daddr=0x0b000001;
//printf("checksum is %x\n", iph.check);
//printf("before changing IP checksum is  %x \n",ip_sum_calc(20, (u16 *)&iph));
/*
45,00,00,54,00,00,40,00,40,01,25,a8,0a,00,00,01,0b,00,00,01
 */

char buf[20];
buf[0] = 0x45;
buf[1] = 0x00;
buf[2] = 0x00;
buf[3] = 0x54;
buf[4] = 0x00;
buf[5] = 0x00;
buf[6] = 0x40;
buf[7] = 0x00;
buf[8] = 0x40;
buf[9] = 0x01;
//buf[10] = 0x25;
//buf[11] = 0xa8;
buf[10] = 0x00;
buf[11] = 0x00;
buf[12] = 0x0a;
buf[13] = 0x00;
buf[14] = 0x00;
buf[15] = 0x01;
buf[16] = 0x0b;
buf[17] = 0x00;
buf[18] = 0x00;
buf[19] = 0x01;

printf("after changing IP checksum is  %x \n", checksum(buf, 20));
//printf("size of unsigned short %u %u\n", sizeof(unsigned short), sizeof(uint8_t));
//printf("after changing IP checksum is  %x \n",ip_sum_calc(10, (u16*)buf));
#if 0
u16 buff1[20] = {69, 0, 0, 84,  0,  0,  64, 0, 64, 1, 37, 168, 10, 0, 0, 1 ,11, 0, 0,1};
printf("before changing IP checksum is %x  %x\n",buff1[0], ip_sum_calc(20, buff1));
iphdr *iph = (iphdr *)buff1;
printf("checksum is %u %x %u %u \n", iph->tot_len,iph->check , iph->saddr, iph->protocol);
printf("checksum is %u %x %u %u \n", iph->tot_len,iph->check , iph->saddr, iph->protocol);
u16 buff2[20] = {69, 0, 0, 84,  0,  0,  64, 0, 64, 1, 37, 168, 10, 0, 0, 1 ,20, 0, 0,1};
printf("after changing IP checksum is %x\n", ip_sum_calc(20, buff2));
#endif
return 0;
}
