#include <Windows.h>
#include <stdio.h>
#include <string>

class UniqueInstance {
public:
	UniqueInstance(const char* name) {
		hHandle = CreateMutexA(NULL, TRUE, name);
		if (ERROR_ALREADY_EXISTS == GetLastError()) {
			success = false;                
			std::string message = std::string("Only one instance of ") + name + " can run.";
			printf("[Error] %s\n", message.c_str());
			MessageBoxA(NULL, message.c_str(), "Error", MB_OK | MB_ICONHAND);
		} 
	}

	bool Success() {
		return success;
	}

	~UniqueInstance() {
		if (success) {
			ReleaseMutex(hHandle);
		}
		CloseHandle(hHandle);
	}

private:
	HANDLE hHandle;
	bool success = true;
};
UniqueInstance s_instance("UniqueInstance.dll");

int main() {
  printf("%s\n",__FILE__);
	if (s_instance.Success()) {
		for (int i = 6; i; i--) {
			printf("wait %d s",i);
      printf("\r");
			Sleep(i*1000);
		}
	}
	return 0;
}