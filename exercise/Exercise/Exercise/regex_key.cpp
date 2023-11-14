#include <iostream>
#include <windows.h>
using namespace std;

#define __T(x) L ## x
#define AfxMessageBox printf
void read_cpu() {
  long lRet;
  HKEY hKey;
  TCHAR tchData[64];
  DWORD dwSize;
  lRet = RegOpenKeyExA(
           HKEY_LOCAL_MACHINE,         // handle to open key
           "Hardware\\Description\\System\\CentralProcessor\\0",  // subkey name
           0,   // reserved
           KEY_QUERY_VALUE, // security access mask
           &hKey    // handle to open key
         );
  if (lRet == ERROR_SUCCESS) {
    dwSize = sizeof(tchData);
    lRet = RegQueryValueExA(
             hKey,            // handle to key
             "ProcessorNameString",  // value name
             NULL,   // reserved
             NULL,       // type buffer
             (LPBYTE)tchData,        // data buffer
             &dwSize      // size of data buffer
           );
    if (lRet == ERROR_SUCCESS) {
      printf("\n CPU INFO:");
      printf("%s\n", tchData);
    }
    //以下是失败的内容
    else {
      printf("\nCPU INFO:");
      printf("UNKNOWN\n");
    }
  }
  //以下是打开失败
  else {
    printf("\n CPU INFO:");
    printf("UNKNOWN\n");
  }
  RegCloseKey(hKey);
}

void read_folder() {
  long lRet;
  HKEY hKey;
  TCHAR tchData[128];
  DWORD dwSize;
  lRet = RegOpenKeyExA(
           HKEY_LOCAL_MACHINE,         // handle to open key
           "SOFTWARE\\DEXIS IS\\ScanFlow",  // subkey name
           0,   // reserved
           KEY_QUERY_VALUE, // security access mask
           &hKey    // handle to open key
         );
  if (lRet != ERROR_SUCCESS) {
    printf("read key failed.\n");
    return;
  }

  dwSize = sizeof(tchData);
  lRet = RegQueryValueExA(
           hKey,            // handle to key
           "ApplicationDataPath",  // value name
           NULL,   // reserved
           NULL,       // type buffer
           (LPBYTE)tchData,        // data buffer
           &dwSize      // size of data buffer
         );
  if (lRet == ERROR_SUCCESS) {
    printf("\n DEXIS INFO:");
    printf("%s\n", tchData);
  } else {
    printf("read key failed.\n");
  }

  RegCloseKey(hKey);
}
int main() {
  //read_cpu();
  read_folder();
  return 0;
}