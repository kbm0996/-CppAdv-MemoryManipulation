#include <cstdio> 
#include <cstdlib> 
#include <cstring> 
#include <Windows.h>
#include <conio.h>

// Dest File(Packing File) Header
struct st_LOCAL_FILE_HEADER
{
	int iSignature;  // fixed 0x03034a50
	int iFileCnt;
};

// Source File Header
struct st_CENTRAL_FILE_HEADER
{
	WCHAR szFileName[128];
	int iFileSize;			
};

//struct st_END_HEADER
//{
//	int iSignature;
//};

void FilePacking(WCHAR *szPackingFile, int iElementFileCnt, WCHAR **szElementNameArray);
void FileUnpacking(WCHAR *szPackingFile);

void main(void)
{
	int iSel;
	while (1)
	{
		system("cls");

		wprintf(L"1. Pack\n2. Unpack\n3. Exit\n");
		iSel = _getwch() - 48;
		if (iSel < 0 || iSel > 4)
			continue;

		switch (iSel)
		{
		case 1:
		{
			int iFileCnt;
			wprintf(L"Input Source File Count : ");
			wscanf_s(L"%d", &iFileCnt);

			WCHAR ** szNameArray = (WCHAR**)malloc(sizeof(WCHAR*) * iFileCnt);
			for (int iCnt = 0; iCnt < iFileCnt; ++iCnt) 
			{
				szNameArray[iCnt] = (WCHAR*)malloc(sizeof(st_CENTRAL_FILE_HEADER::szFileName));
				memset(szNameArray[iCnt], 0, sizeof(st_CENTRAL_FILE_HEADER::szFileName));

				wprintf(L"Input %d File Name : ", iCnt);
				wscanf_s(L"%s", szNameArray[iCnt], (unsigned)_countof(st_CENTRAL_FILE_HEADER::szFileName));
			}

			WCHAR szPackingFile[128];
			wprintf(L"Input Packing File Name : ");
			wscanf_s(L"%s", szPackingFile, (unsigned)_countof(szPackingFile));
			FilePacking(szPackingFile, iFileCnt, szNameArray);

			for (int iCnt = 0; iCnt < iFileCnt; ++iCnt)
				free(szNameArray[iCnt]);
			free(szNameArray);
		}
		break;
		case 2:
		{
			WCHAR szPackingFile[128];
			wprintf(L"Input Packing File Name : ");
			wscanf_s(L"%s", szPackingFile, (unsigned)_countof(szPackingFile));

			FileUnpacking(szPackingFile);
		}
		break;
		case 3:
			return;
		}

		_getch();
	}
	return;
}

void FilePacking(WCHAR * szPackingFile, int iElementFileCnt, WCHAR ** szElementNameArray)
{
	// 0. Packing File Header Setting
	st_LOCAL_FILE_HEADER PackHeader;
	PackHeader.iSignature = 0x03034a50;
	PackHeader.iFileCnt = iElementFileCnt;

	// 1. Packing File Open
	FILE* pDestFile;
	if(_wfopen_s(&pDestFile, szPackingFile, L"w+b") != 0)
	{
		wprintf(L"Can't Open Packing File `%s`\n", szPackingFile);
		return;
	}

	// 2. Source File Open
	FILE** pSourceFile = (FILE**)malloc(sizeof(FILE*) * iElementFileCnt);
	for (int iCnt = 0; iCnt < iElementFileCnt; ++iCnt)
	{
		if (_wfopen_s(&pSourceFile[iCnt], szElementNameArray[iCnt], L"r+b") != 0)
		{
			wprintf(L"Can't Open Source File `%s`\n", szElementNameArray[iCnt]);
			fclose(pDestFile);
			free(pSourceFile);
			return;
		}
	}

	// 3. write st_LOCAL_FILE_HEADER
	int iMaxFile = 0;
	st_CENTRAL_FILE_HEADER FileInfo;
	fwrite(&PackHeader, sizeof(st_LOCAL_FILE_HEADER), 1, pDestFile);
	for (int iCnt = 0; iCnt < iElementFileCnt; ++iCnt)
	{
		fseek(pSourceFile[iCnt], 0, SEEK_END); // file size
		FileInfo.iFileSize = ftell(pSourceFile[iCnt]);
		wcscpy_s(FileInfo.szFileName, szElementNameArray[iCnt]);

		fwrite(&FileInfo, sizeof(FileInfo), 1, pDestFile);

		if (iMaxFile < FileInfo.iFileSize)
			iMaxFile = FileInfo.iFileSize;
	}

	// 4. Source File Data ------send-----> Packing File
	int iFileSize;
	char* pBuffer = (char*)malloc(iMaxFile);
	for (int iCnt = 0; iCnt < iElementFileCnt; ++iCnt) {
		fseek(pSourceFile[iCnt], 0, SEEK_END);
		iFileSize = ftell(pSourceFile[iCnt]);
		rewind(pSourceFile[iCnt]);

		fread(pBuffer, iFileSize, 1, pSourceFile[iCnt]);
		fwrite(pBuffer, iFileSize, 1, pDestFile);
		fclose(pSourceFile[iCnt]);
	}

	fclose(pDestFile);
	free(pBuffer);
	free(pSourceFile);
}

void FileUnpacking(WCHAR * szPackingFile)
{
	// 0. Open Packing File
	FILE* pDestFile;
	if (_wfopen_s(&pDestFile, szPackingFile, L"r+b") != 0)
	{
		wprintf(L"Can't Open Packing File `%s`\n", szPackingFile);
		return;
	}

	// 1. Header Check
	st_LOCAL_FILE_HEADER PackHeader;
	size_t iReadPos = fread(&PackHeader, sizeof(st_LOCAL_FILE_HEADER), 1, pDestFile);
	if (iReadPos != 1)
	{
		wprintf(L"Packing File Local Header Error\n");
		fclose(pDestFile);
		return;
	}

	if (PackHeader.iSignature != 0x03034a50)
	{
		wprintf(L"Packing File Local Header Signature Error\n");
		fclose(pDestFile);
		return;
	}


	// 2. Read Header - Get Source File Info
	iReadPos = sizeof(st_CENTRAL_FILE_HEADER) * PackHeader.iFileCnt;
	st_CENTRAL_FILE_HEADER * FileInfo = (st_CENTRAL_FILE_HEADER*)malloc(iReadPos);
	memset(FileInfo, 0, iReadPos);

	iReadPos = fread(FileInfo, iReadPos, 1, pDestFile);
	if (iReadPos != 1) {
		wprintf(L"Packing File Central Header Source File Info Error\n");
		fclose(pDestFile);
		free(FileInfo);
		return;
	}

	// 3. Get Source File Size. Read Buffer Size = Source File Maximum Size
	int iMaxSize = 0;
	for (int iCnt = 0; iCnt < PackHeader.iFileCnt; ++iCnt)
	{
		if (iMaxSize < FileInfo[iCnt].iFileSize)
			iMaxSize = FileInfo[iCnt].iFileSize;
	}
	char* pBuffer = (char*)malloc(iMaxSize);

	// 4. Get Source File
	for (int iCnt = 0; iCnt < PackHeader.iFileCnt; ++iCnt)
	{
		wprintf(L"`%s` Size : %d \n", FileInfo[iCnt].szFileName, FileInfo[iCnt].iFileSize);

		FILE* pSourceFile;
		if (_wfopen_s(&pSourceFile, FileInfo[iCnt].szFileName, L"wb") != 0)
		{
			wprintf(L"Can't Open Source File `%s`\n", FileInfo[iCnt].szFileName);
		}
		fread(pBuffer, FileInfo[iCnt].iFileSize, 1, pDestFile);
		fwrite(pBuffer, FileInfo[iCnt].iFileSize, 1, pSourceFile);
		fclose(pSourceFile);
	}

	fclose(pDestFile);
	free(pBuffer);
	free(FileInfo);
}
