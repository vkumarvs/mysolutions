#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
typedef int BOOL;

BOOL isPalindrome(const char *str) {
    int l = 0;
    int h = strlen(str) - 1;
    while (h > l) {
        if (str[l++] != str[h--]) {
            return FALSE;
        }
    }
    return TRUE;
}

int main() {
	//const char *mystr="rotor";
	const char *mystr="rottor";
	//const char *mystr="hello";
	if(isPalindrome (mystr)) {
       printf("input string (%s) is palindrome\n", mystr);
	}
	else {
       printf("input string (%s) is not palindrome\n", mystr);
	}
	return 0;
}
