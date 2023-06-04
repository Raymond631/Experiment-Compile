
#include <iostream>
#include<stdio.h>
#include<cstring>
using namespace std;
char str[10];
int index_a = 0;
void E();			//E->TG;
void X();			//G->+TG | e
void T();			//T->FS
void Y();			//S->*FS | e
void F();			//F->(E) | i
int main()
{
	int len;
	cin >> str;
	len = strlen(str);
	str[len + 1] = '\0';
	E();
	if (str[index_a] == '#') {
		cout << str;
		cout << "为合法符号串" << endl;
	}
	else {
		cout << "非法的符号串!" << endl;
	}
	index_a = 0;
	return 0;
}
/*******Beign*******/
/*构造递归关系及子程序*/
void E()
{
    T();
    X();
}
void X()
{
	if (str[index_a] == '+')
	{
		index_a++;
		T();
		X();
	}
	else if (str[index_a] == '-')
		{
			index_a++;
			T();
			X();
		}
	else if (str[index_a] == 'e') {
		index_a++;
	}
}
void T()
{
    F();
    Y();
}
void Y()
{
	if (str[index_a] == '*')
	{
		index_a++;
		F();
		Y();
	}
	else if (str[index_a] == '/')
	{
		index_a++;
		F();
		Y();
	}
	else if (str[index_a] == 'e') {
		index_a++;
	}
}
void F()
{
	if (str[index_a] == '(')
	{
		index_a++;
		E();
		if (str[index_a] == ')') {
			index_a++;
		}
		else {
			cout << "非法的符号串!" << endl;
            exit(1);
		}
	}
	else if (str[index_a] == 'i') {
		index_a++;
	}
	else {
		cout << "非法的符号串!" << endl;
		exit(1);
	}
}
/*******End*******/
