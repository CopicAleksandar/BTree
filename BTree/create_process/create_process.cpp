// create_process.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

int main(int argc, char **argv) {
//      "C:\\Users\\e31-2015\\Desktop\\btree_new\\btree\\Debug\\";
	LPCWSTR app = L"C:\\Users\\e31-2015\\Desktop\\btree_new\\btree\\Debug\\node.exe";
  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  for (int i = 0; i < 10; ++i) {
	  if (!CreateProcess(app, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		  printf("Create proccess failed (%d).\n", GetLastError());
  }

  return 0;
}
