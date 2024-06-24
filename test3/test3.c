#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <wchar.h>
#include <conio.h>
#include <locale.h> // 로케일을 설정하기 위해 필요
int main()
{

    setlocale(LC_ALL, "");

    wchar_t str[20];
    int idx = 0;

    str[0] = L'안';
    str[1] = L'녕';
    str[2] = L'\n';
    str[3] = L'하';
    str[4] = L'삼';
    str[5] = L'\0';


    wprintf(L"%ls", str);

    while (1);
    return 0;
}