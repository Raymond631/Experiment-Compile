#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;
/* 自行修改可读取代码的长度 */
char prog[200], token[20];
char ch;
int syn, p, m = 0, n, row, sum = 0;
/* 如果需要添加关键词，请修改此处的长度 */
#define N_KEYWORDS 10
const char* rwtab[N_KEYWORDS] = { "if", "int",  "using", "namespace",  "for",   "while",
                                 "do", "return", "break", "continue" };

void scaner() {
    /*
      共分为三大块，分别是标示符、数字、符号，对应下面的 if else if 和 else
    */
    for (n = 0; n < 8; n++) {
        token[n] = '\0';
    }
    ch = prog[p++];
    while (ch == ' ') {
        ch = prog[p];
        p++;
    }
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
        //可能是标示符或者关键字
        m = 0;
        while ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') ||
            (ch >= 'A' && ch <= 'Z')) {
            token[m++] = ch;
            ch = prog[p++];
        }
        token[m++] = '\0';
        p--;
        syn = 2;
        for (n = 0; n < N_KEYWORDS; n++) {
            /* 在此处填写代码，完成关键词的匹配 */
            if (strcmp(token, rwtab[n]) == 0)
            {
                syn = 1;
            }
        }

    }
    else if ((ch >= '0' && ch <= '9')) {
        //数字
        {
            sum = 0;
            while ((ch >= '0' && ch <= '9')) {
                sum = sum * 10 + ch - '0';
                ch = prog[p++];
                syn = 3;
            }
        }
        p--;
        syn = 3;
        if (sum > 32767)
            syn = -1;
    }
    else
        switch (ch) {
            //其他字符
        case '<':
            m = 0;
            token[m++] = ch;
            ch = prog[p++];
            if (ch == '=') {
                syn = 4;
                token[m++] = ch;
            }
            /* 此处完成对 << 符号的识别 */
            else if (ch == '<')
            {
                syn = 4;
                token[m++] = ch;
            }
            else {
                syn = 4;
                p--;
            }
            break;
        case '>':
            m = 0;
            token[m++] = ch;
            ch = prog[p++];
            if (ch == '=') {
                syn = 4;
                token[m++] = ch;
            }
            else {
                syn = 4;
                p--;
            }
            break;
        case ':':
            m = 0;
            token[m++] = ch;
            ch = prog[p++];
            if (ch == '=') {
                syn = 4;
                token[m++] = ch;
            }
            else {
                syn = 4;
                p--;
            }
            break;
        case '*':
            syn = 4;
            token[0] = ch;
            break;
        case '/':
            /* 此处添加对 // 的识别 */
            m = 0;
            token[m++] = ch;
            ch = prog[p++];
            if (ch == '/')
            {
                syn = 5;
                token[m++] = ch;
            }
            else
            {
                syn = 4;
                p--;
            }
            break;
        case '+':
            syn = 4;
            token[0] = ch;
            break;
        case '-':
            syn = 4;
            token[0] = ch;
            break;
        case '=':
            syn = 4;
            token[0] = ch;
            break;
        case ';':
            syn = 5;
            token[0] = ch;
            break;
        case ',':
            syn = 5;
            token[0] = ch;
            break;
        case '(':
            syn = 5;
            token[0] = ch;
            break;
        case ')':
            syn = 5;
            token[0] = ch;
            break;
        case '{':
            syn = 5;
            token[0] = ch;
            break;
        case '}':
            syn = 5;
            token[0] = ch;
            break;
        case '#':
            syn = 0;
            token[0] = ch;
            break;
            /* 此处完成对 " 的识别 */
        case '"':
            syn = 5;
            token[0] = ch;
            break;
        case '\n':
            syn = -2;
            break;
        default:
            syn = -1;
            break;
        }
}

int main() {
    p = 0;
    row = 1;
    cout << "Please input string:" << endl;
    do {
        cin.get(ch);
        prog[p++] = ch;
    } while (ch != '#');
    p = 0;
    do {
        scaner();
        switch (syn) {
        case 0:
            break;
        case 3:
            cout << "(" << syn << "," << sum << ")" << endl;
            break;
        case -1:
            cout << "Error in row " << row << "!" << endl;
            break;
        case -2:
            ++row;
            break;
        default:
            cout << "(" << syn << "," << token << ")" << endl;
            break;
        }
    } while (syn != 0);
}