#include <stdio.h>   /* required for file operations */
#include <string.h>
#include <stdlib.h>
FILE *fr;            /* declare the file pointer */

main()
{
#define NO_OF_INTERFACE 150

    char firstStr[50];
    char line[80];
    int ifIndex = 0;
    const char *switchIff[NO_OF_INTERFACE][5];
    const char *UpStatus = "UP";
    const char *DownStatus = "Down";
    fr = fopen ("interface.txt", "rt");  /* open the file for reading */

    while(fgets(line, 80, fr) != NULL)
    {
      char *name = new char[50];
      char *input_output_packet = new char[50];
      char *dropped_paket= new char[50];
      memset(firstStr,0,sizeof(firstStr));
      memset(name ,0, 50 );
      memset(input_output_packet,0, 50 );
      memset(dropped_paket,0, 50 );

      sscanf (line, "%50s %80s %30s %*s %*s %*s %20s",firstStr, name, input_output_packet, dropped_paket);
      if((strcmp(firstStr,"Interface") ==0) || (firstStr[0] == '<') &&
         (firstStr[1] == 'U') && (firstStr[2] == 'P'))
      {
        if((firstStr[0] == '<'))
        {
          switchIff[ifIndex][1] = UpStatus;
        }
        else
        {
          switchIff[ifIndex][0] = name;
        }
      }
      else if((firstStr[0] == '<'))
      {
        switchIff[ifIndex][1] = DownStatus;
      }
      else if(strcmp(firstStr, "input") == 0) 
      {
        switchIff[ifIndex][2] = input_output_packet;
        switchIff[ifIndex][3] = dropped_paket;
      }
      else if(strcmp(firstStr, "output") == 0)
      {
        switchIff[ifIndex][4] = input_output_packet;
        ifIndex++;
      }
    }

    for(int i=0; i<ifIndex; i++)
    {
      printf("%s %s %u %u %u\n ", switchIff[i][0],
             switchIff[i][1], atoi(switchIff[i][2]), atoi(switchIff[i][3]),
             atoi(switchIff[i][4]));
      delete(switchIff[i][0]);
    }
    fclose(fr); 
} /*of main*/

