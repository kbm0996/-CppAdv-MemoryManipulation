#include <Windows.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <time.h>

class NewLeak
{
public:
	static NewLeak *GetInstance(void)
	{
		if (nullptr == _pNewLeak)	// ó������ ������ ���.
		{
			_pNewLeak = new NewLeak;
			atexit(Destroy);	// c��Ÿ�� ���̺귯��. �� ���μ����� ������ ��ȣ ���� �Լ��� ȣ���Ѵ�.
								// �α׸� ����ٴ��� �ϴ� �뵵��.. 
								// ���� ���߽� �̱����� ������ ������ٵ� �˾Ƽ� atexit�� �������� ���~ ȣ��ȴ�.
								//  �����쿡���� ���� �޸� ����� �־ ũ�� �Ű� �� �ʿ䰡 ������!
								// �ܼ� ���߽ÿ� �޸� ������ �� ö���� ���־���Ѵ�..
		}

		return _pNewLeak;
	}

	static void Destroy(void)	// atexit ���̺귯���̱� ������ �� �����
	{							// �����־�� �Ѵ�
		delete _pNewLeak;
	}



	struct st_ALLOC_INFO
	{
		void *pPtr;
		int iSize;
		char szFile[256];
		int iLine;

		bool bIsArray;

		bool bArrayError;
		bool bAllocError;
		bool bLeakError;

		st_ALLOC_INFO *pNext;
	};

	st_ALLOC_INFO _Head;

	void SaveAllocInfo(void *pPtr, int iSize, char *szFile, int iLine, bool bIsArray = false, bool bAllocError = false, bool bLeakError = true)
	{
		st_ALLOC_INFO* pNode = (st_ALLOC_INFO*)malloc(sizeof(st_ALLOC_INFO));

		pNode->pPtr = pPtr;
		pNode->iSize = iSize;
		if (szFile != nullptr)
			strcpy_s(pNode->szFile, szFile);
		pNode->iLine = iLine;
		pNode->bIsArray = bIsArray;

		pNode->bArrayError = false;
		pNode->bAllocError = bAllocError;
		pNode->bLeakError = bLeakError;

		pNode->pNext = _Head.pNext;
		_Head.pNext = pNode;
	}
	bool RemoveAllocInfo(void *pPtr)
	{
		st_ALLOC_INFO* pNode = _Head.pNext;
		st_ALLOC_INFO *pPrevNode = &_Head;

		if (pNode == nullptr) return false;  //  ����ó��..

		while (pNode->pNext != nullptr)
		{
			if (pPtr == pNode->pPtr)
			{
				//pPrevNode->pNext = pNode->pNext;

				if (pNode->bIsArray == true)
					pNode->bArrayError = true;
				pNode->bLeakError = false;
				return true;
			}
			pPrevNode = pNode;
			pNode = pNode->pNext;
		}
		SaveAllocInfo(pPtr, sizeof(pPtr), nullptr, 0, false, true, false);
		return false;
	}

	bool RemoveArrayAllocInfo(void *pPtr)
	{
		st_ALLOC_INFO* pNode = _Head.pNext;
		st_ALLOC_INFO *pPrevNode = &_Head;

		if (pNode == nullptr) return false;  //  ����ó��..

		while (pNode->pNext != nullptr)
		{
			if (pPtr == pNode->pPtr)
			{
				//pPrevNode->pNext = pNode->pNext;

				if (pNode->bIsArray == false)
					pNode->bArrayError = true;
				pNode->bLeakError = false;
				return true;
			}
			pPrevNode = pNode;
			pNode = pNode->pNext;
		}
		SaveAllocInfo(pPtr, sizeof(pPtr), nullptr, 0, false, true, false);
		return false;
	}
	void PrintAlloc(FILE *pfLog)
	{
		st_ALLOC_INFO *pNode = &_Head;

		if (pNode == nullptr) return; //  ����ó��..

		while (pNode != nullptr)
		{
			if (pNode->bLeakError == true)
				fprintf_s(pfLog, "LEAK	[0x%p] [%3d] %s : %d\n", pNode->pPtr, pNode->iSize, pNode->szFile, pNode->iLine);

			if (pNode->bArrayError == true)
				fprintf_s(pfLog, "ARRAY	[0x%p] [%3d] %s : %d\n", pNode->pPtr, pNode->iSize, pNode->szFile, pNode->iLine);

			if (pNode->bAllocError == true)
				fprintf_s(pfLog, "NOALLOC	[0x%p]\n", pNode->pPtr);

			pNode = pNode->pNext;
		}
		return;
	}
private:
	static NewLeak *_pNewLeak;

	NewLeak()
	{
		_Head.pNext = nullptr;
	}
	~NewLeak()
	{
		FILE *pfLog;
		char szNameBuff[256];
		time_t timer;
		struct tm stTm;

		time(&timer);
		localtime_s(&stTm, &timer);
		sprintf_s(szNameBuff, "Alloc_%04d%02d%02d_%02d%02d%02d.txt", stTm.tm_year + 1900, stTm.tm_mon + 1, stTm.tm_mday, stTm.tm_hour, stTm.tm_min, stTm.tm_sec);
		fopen_s(&pfLog, szNameBuff, "w+t");

		PrintAlloc(pfLog);

		fclose(pfLog);
	}
};

NewLeak *NewLeak::_pNewLeak = nullptr;
NewLeak *pNewLeak = NewLeak::GetInstance();

void * operator new (size_t size, char *File, int Line)
{
	void *adr = malloc(size);

	pNewLeak->SaveAllocInfo(adr, size, File, Line);

	return adr;
}
void * operator new[](size_t size, char *File, int Line)
{
	void *adr = malloc(size);

	pNewLeak->SaveAllocInfo(adr, size, File, Line, true);

	return adr;
}

void operator delete (void * p)
{
	if (pNewLeak->RemoveAllocInfo(p))
		free(p);
	else
		return;
}
void operator delete[](void * p)
{
	if (pNewLeak->RemoveArrayAllocInfo(p))
		free(p);
	else
		return;
}