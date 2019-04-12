#include <cstdio>
#include <cstdlib>
#include <cstring>

struct NODE
{
	void *ptr;
	int iSize;
	char chpFileName[100];
	int iFileLine;

	NODE* pNext;
	NODE *pPrev;
};

NODE g_Head;
NODE g_Tail;

void Init();
void InsertHead(void* ptr, int iSize, char* chpFileName, int iFileLine);
void InsertTail(void* ptr, int iSize, char* chpFileName, int iFileLine);
void PrintAlloc();
bool Remove(void *ptr);

int g_iTotalAllocSize = 0;
int g_iTotalAllocCount = 0;

template <typename T>
T MemoryAlloc(T ptr, int Size, char* FileName, int FileLine)
{
	g_iTotalAllocSize += Size;
	g_iTotalAllocCount++;
	InsertHead(ptr, Size, FileName, FileLine);
	return ptr;
}

template <typename T>
void MemoryRelease(T *ptr)
{
	Remove(ptr);
}


#define _MemoryAlloc(T, size)	MemoryAlloc((T*)malloc(sizeof(T) * size), sizeof(T)*size, __FILE__, __LINE__);


void main()
{
	Init();

	int *p4 = _MemoryAlloc(int, 1);
	int *p400 = _MemoryAlloc(int, 100);
	char *pZ1 = _MemoryAlloc(char, 50);
	char *pZ2 = _MemoryAlloc(char, 150);
	char *pZ3 = _MemoryAlloc(char, 60);
	char *pZ4 = _MemoryAlloc(char, 70);

	MemoryRelease(p4);
	//MemoryRelease(p400);
	MemoryRelease(pZ1);
	//MemoryRelease(pZ2);
	MemoryRelease(pZ3);
	MemoryRelease(pZ3);
	MemoryRelease(pZ4);

	PrintAlloc();
}

void Init()
{
	g_Head.pNext = &g_Tail;
	g_Head.pPrev = NULL;
	g_Tail.pNext = NULL;
	g_Tail.pPrev = &g_Head;
}

void InsertHead(void* ptr, int iSize, char* chpFileName, int iFileLine)
{
	NODE* pNode = (NODE*)malloc(sizeof(NODE));

	pNode->ptr = ptr;
	pNode->iSize = iSize;
	strcpy_s(pNode->chpFileName, chpFileName);
	pNode->iFileLine = iFileLine;

	pNode->pNext = g_Head.pNext;
	pNode->pPrev = &g_Head;
	pNode->pPrev->pNext = pNode;
	pNode->pNext->pPrev = pNode;
}

void InsertTail(void* ptr, int iSize, char* chpFileName, int iFileLine)
{
	NODE* pNode = (NODE*)malloc(sizeof(NODE));

	pNode->ptr = ptr;
	pNode->iSize = iSize;
	strcpy_s(pNode->chpFileName, chpFileName);
	pNode->iFileLine = iFileLine;

	pNode->pNext = &g_Tail;
	pNode->pPrev = g_Tail.pPrev;
	pNode->pPrev->pNext = pNode;
	pNode->pNext->pPrev = pNode;
}

void PrintAlloc()
{
	NODE* pNode = g_Head.pNext;
	printf("\nTotal Alloc Size : %d\n", g_iTotalAllocSize);
	printf("Total Alloc Count : %d\n", g_iTotalAllocCount);

	while (pNode->pNext != NULL)
	{
		printf("Not Release Memory : [0x%x] %d Bytes\n", pNode->ptr, pNode->iSize);
		printf("File: %s : %d \n\n", pNode->chpFileName, pNode->iFileLine);
		pNode = pNode->pNext;
	}
	return;
}

bool Remove(void *ptr)
{
	NODE* pNode = g_Head.pNext;
	while (pNode->pNext != NULL)
	{
		if (ptr == pNode->ptr)
		{
			pNode->pPrev->pNext = pNode->pNext;
			pNode->pNext->pPrev = pNode->pPrev;
			free(pNode->ptr);
			free(pNode);
			return true;
		}
		pNode = pNode->pNext;
	}
	return false;
}