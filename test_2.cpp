#include "stdio.h"
#include "stdlib.h"

void main()
{
	int a[3],b;
	FILE *f;

	fopen_s(&f,"qaz.txt","r");
	for (int i=0; i<=2; i++)
		fscanf(f,"%u",&a[i]);
	fclose(f);

	b=a[0]+a[1]+a[2];

	fopen_s(&f,"wsx.txt","w");
	fprintf(f,"%u",b);
	fclose(f);

	system("pause");
}
