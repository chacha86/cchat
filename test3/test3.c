#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <wchar.h>
#include <conio.h>
#include <locale.h> // �������� �����ϱ� ���� �ʿ�
int main()
{

    setlocale(LC_ALL, "");

    wchar_t str[20];
    int idx = 0;

    str[0] = L'��';
    str[1] = L'��';
    str[2] = L'\n';
    str[3] = L'��';
    str[4] = L'��';
    str[5] = L'\0';


    wprintf(L"%ls", str);

    while (1);
    return 0;
}