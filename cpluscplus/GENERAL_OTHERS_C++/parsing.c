#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void parseShellVariable(const char *str, char *parsed_val_str)
{
    const char* temp = str;
    char temp1[100];
    char numeric_val[10];
    int str_len =strlen(str);
    int env_var_char_index = 0,num_index = 0;
    /*Flag is used to separate shell variable string and numeric string */
    char flag = 0;
    printf("string for parsed%s\n",temp);
    while(str_len  > 0)
    {
        if((*temp != '+') && (!flag) )
        {
            if(*temp == '$')
            {
                temp = temp + 1;
            }
            else
            {
                temp1[env_var_char_index++] = *temp;
                temp = temp + 1;
            }
        }
        else if(*temp == '+')
        {
            flag = 1;
            temp = temp + 1;
        }
        else
        {
            numeric_val[num_index++] = *temp;
            temp = temp + 1;
        }
        str_len--;
        if(str_len == 0)
        {
            temp1[env_var_char_index] = '\0';
            numeric_val[num_index] = '\0';
            const char* val = (const char *)getenv(temp1);
            int num_val = atoi(numeric_val);
            if(val)
            {
                num_val = num_val+atoi(val);
            }
            /*prepare final val for token */
            sprintf(parsed_val_str, "%d", num_val);
    printf("parsed value string %s\n",parsed_val_str);
        }
    }
}

int main()
{
//const char* str = "$MTP3_LINKID_START_INDEX+1";
    //const char* str = "$MTP3_LINKID_START_INDEX";
    const char* str = "$RNC_PC1";
    const char val[50];
    parseShellVariable(str, (char *)val);
    if(val)
    {
        printf("aafinal value is %s\n", val);
    }
    return 0;
}
