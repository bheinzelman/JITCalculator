#include <stdlib.h>
#include <stdio.h>

extern int compute();

int main() {
	int result = compute();
	printf("%d\n", result);
	return 0;
}
