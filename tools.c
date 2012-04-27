#include "tools.h"

void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

int hex2dec(char *a) {
	int result = 0;
	int i;
	for(i = 0; (a[i]!='\0') && (a[i]!='\n'); i++)
		if(a[i] >= 'A' && a[i] <= 'F')
			result = (16*result)+(a[i]-'A'+10);
		else
			result = (16*result)+(a[i]-'0');
	return result;
}

unsigned int getbit(unsigned int x, int i) {
	return (x>>i) & 1;
}
