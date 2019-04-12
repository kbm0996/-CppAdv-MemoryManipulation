#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// ÁÖ¼® Ç®¸é new, delete ÃßÀû
#include "NewLeak.h"



void main()
{
	char *p1 = new char;
	int *p4 = new int;
	int *p400 = new int[100];
	char *pZ1 = new char[50];
	char *pZ2 = new char[150];
	char *pZ3 = new char[60];
	char *pZ4 = new char[70];
	int *pZ5 = 0;

	delete p4;
	//delete[] p400;
	delete pZ1;
	//delete[] pZ2;
	delete[] pZ3;
	delete[] pZ4;
	delete pZ5;
}