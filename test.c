#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
	void *hi = malloc(10);
	free(hi);
	free(hi);
	return (0);
}