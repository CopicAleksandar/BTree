#pragma once

typedef void(*HANDLE)(int, int, int);

int foo(int a, int b, HANDLE handle, int c);