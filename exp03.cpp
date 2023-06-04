#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <iomanip>
#include <string>
using namespace std;
string V[100][2];//存储拆分后的文法
string terminator;//存储统计终结符
int vi = 0; //存储拆分后有多少行
int t = 0;
int s = 0;
int l;
int r;
string FIRSTVT[20][2]; //存储firstvt集
string LASTVT[20][2]; //存储lastvt集
string str; //存储输入文法
string str_a = "#"; // 下堆栈
string str_b; // 剩余输入串
string analysis_table[100][5]; // 文法分析过程
char table[100][100]; // 算符优先关系表
void init_string(string& a) // 删除字符串的第一个元素
{
    for (int i = 1; i <= a.length(); ++i)
    {
        a[i - 1] = a[i];
    }
}
bool is_CHAR(char c) // 判断是否为大写字母
{
    if (c >= 'A' && c <= 'Z')
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_in(int i, string x) // 判断从字符串x从最好一个开始算起连续的i个字符是否含有非大写字母
{
    bool flag = false;
    for (int j = 0; j < i; j++)
    {
        if (!is_CHAR(x[x.length() - j - 1]))
        {
            flag = true;
        }
    }
    return flag;
}
void split(string a) // 拆分文法，使其不含有｜
{
    for (int i = 3; i < a.length(); ++i)
    {
        V[vi][0] = a[0];
        while (a[i] != '|' && i < a.length())
        {
            V[vi][1] += a[i];
            i++;
        }
        vi++;
    }
}
void read_file(string file_path) //按行读取文件
{
    fstream f(file_path);
    vector<string> words;
    string line;
    while (getline(f, line))
    {
        words.push_back(line);
    }
    cout << "输入文法:" << endl;
    for (int i = 0; i < words.size(); i++)
    {
        cout << words[i] << endl;
        split(words[i]);
    }
}
int find_index(char a) //寻找字符a在firstvt或者lastvt中的位置
{
    for (int i = 0; i < t; ++i)
    {
        if (FIRSTVT[i][0][0] == a)
        {
            return i;
        }
    }
    return -1;
}
int find_table_index(char a) //寻找字符a在算符优先关系表中的位置
{
    for (int i = 0; i <= s; ++i)
    {
        if (table[i][0] == a)
        {
            return i;
        }
    }
    return -1;
}
void get_start() //获取非终结符
{
    for (int i = 0; i < vi; ++i)
    {
        bool flag = true;
        for (int j = 0; j < t; ++j)
        {
            if (FIRSTVT[j][0] == V[i][0])
            {
                flag = false;
            }
        }
        if (flag)
        {
            FIRSTVT[t][0] = V[i][0];
            LASTVT[t][0] = V[i][0];
            t++;
        }
    }
}
void get_final() //获取终结符
{
    char tempchar;
    for (int i = 0; i < vi; ++i)
    {
        for (int j = 0; j < V[i][1].length(); ++j)
        {
            bool flag = false;
            tempchar = V[i][1][j];
            string tempstr(1, tempchar);
            if (!is_CHAR(tempchar))
            {
                //如果当前终结符串中已存在该终结符
                if (terminator.find(tempstr) == -1)
                {
                    flag = true;
                }
            }
            if (flag)
            {
                terminator += tempchar;
                s++;
            }
        }
    }
}
/********Beign********/
/*构造firstvt,lastvt*/
void add_firstvt(string b, int a) //判断字符串b是否在序号为a的firstvt中，没有则加入
{
    if (FIRSTVT[a][1].find(b) == -1)
    {
        FIRSTVT[a][1] += b;
    }
}
void add_firstvt(char c, int a) //判断字符c是否在序号为a的firstvt中，没有则加入
{
    string str(1, c);
    if (FIRSTVT[a][1].find(str) == -1)
    {
        FIRSTVT[a][1] += str;
    }
}
void add_lastvt(string b, int a) //判断字符串b是否在序号为a的lastvt中，没有则加入
{
    if (LASTVT[a][1].find(b) == -1)
    {
        LASTVT[a][1] += b;
    }
}
void add_lastvt(char c, int a) //判断字符串c是否在序号为a的lastvt中，没有则加入
{
    string str(1, c);
    if (LASTVT[a][1].find(str) == -1)
    {
        LASTVT[a][1] += str;
    }
}
string get_cur_firstvt(char c, int a) //获取当前字符的firstvt，并放入序号为a的firstvt中
{
    for (int j = 0; j < vi; j++)
    {
        if (V[j][0][0] == c)
        {
            // 若P->a,则把a加入进first(P)中
            if (!is_CHAR(V[j][1][0]))
                add_firstvt(V[j][1][0], a);
            else
            {
                //若P->Q,则把first(Q)加入进first(P)中
                if (V[j][1][0] != c)
                {
                    get_cur_firstvt(V[j][1][0], find_index(V[j][1][0]));
                    add_firstvt(FIRSTVT[find_index(V[j][1][0])][1], a);
                }
                //若P->Qa,则把a加入进first(P)中
                if (V[j][1].length() != 1)
                    if (!is_CHAR(V[j][1][1]))
                        add_firstvt(V[j][1][1], a);
            }
        }
    }
    return FIRSTVT[a][1];
}
string get_cur_lastvt(char c, int a) //获取当前字符的lastvt，并放入序号为a的lastvt中
{
    //定义一个新的字符串，用于存储去掉末尾非终结符的字符串
    string newstr, tempstr;
    for (int j = 0; j < vi; j++)
    {
        if (V[j][0][0] == c)
        {
            if (is_in(1, V[j][1]))
                add_lastvt(V[j][1][V[j][1].length() - 1], a);
            else
            {
                if (V[j][1][V[j][1].length() - 1] != c)
                {
                    get_cur_lastvt(V[j][1][V[j][1].length() - 1], find_index(V[j][1][V[j][1].length() - 1]));
                    add_lastvt(LASTVT[find_index(V[j][1][V[j][1].length() - 1])][1], a);
                }
                tempstr = V[j][1];
                newstr = tempstr.erase(V[j][1].length() - 1);
                if (newstr.length() != 0)
                    if (is_in(1, newstr))
                        add_lastvt(newstr[newstr.length() - 1], a);
            }
        }
    }
    return LASTVT[a][1];
}
/*********End*********/
void get_firstvt() //获取所有文法的firstvt
{
    for (int i = 0; i < t; i++)
    {
        get_cur_firstvt(FIRSTVT[i][0][0], i);
    }
}
void get_lastvt() //获取所有文法的lastvt
{
    for (int i = 0; i < t; i++)
    {
        get_cur_lastvt(LASTVT[i][0][0], i);
    }
}
void print_firstvt(string t, string a) //打印非终极符为t的firstvt
{
    cout << "FIRSTVT(" << t << ") = {";
    for (int i = 0; i < a.length(); ++i)
    {
        if (i == a.length() - 1)
        {
            cout << "\"" << a[i] << "\"";
        }
        else
        {
            cout << "\"" << a[i] << "\"" << ", ";
        }
    }
    cout << "}" << endl;
}
void print_lastvt(string t, string a) //打印非终结符为t的lastvt
{
    cout << "LASTVT(" << t << ") = {";
    for (int i = 0; i < a.length(); ++i)
    {
        if (i == a.length() - 1)
        {
            cout << "\"" << a[i] << "\"";
        }
        else
        {
            cout << "\"" << a[i] << "\"" << ", ";
        }
    }
    cout << "}" << endl;
}
/********Beign********/
/*构造算符优先表*/

void init_table() //初始化算符优先关系表
{
    table[0][0] = '\\';
    for (int j = 1; j < s + 1; j++)
    {
        table[0][j] = terminator[j - 1];
        table[j][0] = terminator[j - 1];
    }
    for (int i = 1; i < s + 1; ++i)
    {
        for (int j = 1; j < s + 1; ++j)
        {
            table[i][j] = ' ';
        }
    }
}

void get_table() //生成算符优先关系表
{
    for (int i = 0; i < vi; ++i)
    {
        for (int j = 0; j < V[i][1].length() - 1; ++j)
        {
            if (!is_CHAR(V[i][1][j]) && !is_CHAR(V[i][1][j + 1]))
            {
                string tempstr1(1, V[i][1][j]);
                string tempstr2(1, V[i][1][j + 1]);
                table[terminator.find(tempstr1) + 1][terminator.find(tempstr2) + 1] = '=';
            }
            if (!is_CHAR(V[i][1][j]) && is_CHAR(V[i][1][j + 1]) && !is_CHAR(V[i][1][j + 2]) && V[i][1][j + 2])
            {
                string tempstr1(1, V[i][1][j]);
                string tempstr2(1, V[i][1][j + 2]);
                table[terminator.find(tempstr1) + 1][terminator.find(tempstr2) + 1] = '=';
            }
            if (!is_CHAR(V[i][1][j]) && is_CHAR(V[i][1][j + 1]))
            {
                string tempstr1(1, V[i][1][j]);
                for (int k = 1; k < s + 1; k++)
                {
                    int a=terminator.find(tempstr1) + 1;
                    string tempstr2(1, table[0][k]);
                    if (FIRSTVT[find_index(V[i][1][j + 1])][1].find(tempstr2) != -1)
                    {
                        table[terminator.find(tempstr1) + 1][k] = '<';
                    }
                }
            }
            if (is_CHAR(V[i][1][j]) && !is_CHAR(V[i][1][j + 1]))
            {
                string tempstr1(1, V[i][1][j + 1]);
                for (int k = 1; k < s + 1; k++)
                {
                    int a = terminator.find(tempstr1) + 1;
                    string tempstr2(1, table[0][k]);
                    if (LASTVT[find_index(V[i][1][j])][1].find(tempstr2) != -1)
                    {
                        table[k][terminator.find(tempstr1) + 1] = '>';
                    }
                }
            }
        }
    }
}
/*********End*********/
void print_table() //打印算符优先关系表
{
    for (int i = 0; i < s + 1; ++i)
    {
        for (int j = 0; j < s + 1; ++j)
        {
            cout << table[i][j] << " ";
        }
        cout << endl;
    }
}
char get_relationship(char a, char b) //获取终结符a，b的优先关系
{
    return table[find_table_index(a)][find_table_index(b)];
}
bool is_reduce() //判断是否可以规约
{
    for (int i = 0; i < vi; ++i)
    {
        int count = 0;
        int f = str_a.length() - 1;
        for (int j = V[i][1].length() - 1; j >= 0 && f >= 0; j--, f--)
        {
            if (is_in(V[i][1].length(), str_a))
            {
                if (is_CHAR(str_a[f]) && is_CHAR(V[i][1][j]))
                {
                    count++;
                }
                else if (str_a[f] == V[i][1][j])
                {
                    count++;
                }
            }
            else
            {
                continue;
            }
        }
        if (count == V[i][1].length())
        {
            r = i;
            return true;
        }
    }
    return false;
}
void analyze_input_string() // 生成算符优先文法的分析过程
{
    analysis_table[0][0] = "步骤";
    analysis_table[0][1] = "下堆栈";
    analysis_table[0][2] = "优先关系";
    analysis_table[0][3] = "剩余输入串";
    analysis_table[0][4] = "移进或规约";
    str_b = str;
    char relationship;
    l = 1;
    int x;
    stringstream ss;
    while (true)
    {
        ss << l;
        int index = str_a.length() - 1;
        analysis_table[l][0] = ss.str();
        analysis_table[l][3] = str_b;
        analysis_table[l][1] = str_a;
        ss.clear();
        ss.str("");
        if (is_CHAR(str_a[index]))
        {
            for (int i = str_a.length() - 1; i >= 0; i--)
            {
                if (!is_CHAR(str_a[i]))
                {
                    index = i;
                    break;
                }
            }
        }
        relationship = get_relationship(str_a[index], str_b[0]);
        analysis_table[l][2] = relationship;
        if (relationship == '=')
        {
            if (str_a[index] == '#' && str_b[0] == '#')
            {
                analysis_table[l][4] = "完成";
                break;
            }
            else
            {
                analysis_table[l][4] = "移进";
                str_a += str_b[0];
                analysis_table[l + 1][1] = str_a;
                init_string(str_b);
            }
        }
        else if (relationship == '<')
        {
            analysis_table[l][4] = "移进";
            str_a += str_b[0];
            analysis_table[l + 1][1] = str_a;
            init_string(str_b);
        }
        else if (relationship == '>')
        {
            if (is_reduce())
            {
                analysis_table[l][4] = "规约";
                str_a[str_a.length() - V[r][1].length()] = V[r][0][0];
                str_a.erase(str_a.length() - V[r][1].length() + 1, V[r][1].length() - 1);
            }
            else
            {
                cout << "输入串非法" << endl;
                exit(-1);
            }
        }
        l++;
    }
}
void print_analyze_process() //打印算符优先文法的分析过程
{
    cout << "算符优先分析过程" << endl;
    cout << setw(12) << analysis_table[0][0] << setw(16) << analysis_table[0][1] << setw(16) << analysis_table[0][2]
        << setw(24)
        << analysis_table[0][3] << setw(20)
        << analysis_table[0][4] << endl;
    for (int i = 1; i <= l; ++i)
    {
        cout.width(10);
        cout << analysis_table[i][0];
        cout.width(12);
        cout << analysis_table[i][1];
        cout.width(10);
        cout << analysis_table[i][2];
        cout.width(20);
        cout << analysis_table[i][3];
        cout << analysis_table[i][4];
        cout << endl;
    }
}
int main(int argv, char* arg[])
{
    cout.setf(std::ios::left);
    read_file("in.txt");
    cout << "拆分后文法:" << endl;
    for (int i = 0; i < vi; ++i)
    {
        cout << V[i][0] << "->" << V[i][1] << endl;
    }
    cout << "非终结符:" << endl;
    get_start();
    get_final();
    for (int j = 0; j < t; ++j)
    {
        cout << FIRSTVT[j][0] << endl;
    }
    cout << "FIRSTVT:" << endl;
    get_firstvt();
    for (int k = 0; k < t; ++k)
    {
        print_firstvt(FIRSTVT[k][0], FIRSTVT[k][1]);
    }
    cout << "LASTVT:" << endl;
    get_lastvt();
    for (int k = 0; k < t; ++k)
    {
        print_lastvt(LASTVT[k][0], LASTVT[k][1]);
    }
    cout << "算符优先关系表" << endl;
    init_table();
    get_table();
    print_table();
    cout << "请输入文法并以#结束:" << endl;
    cin >> str;
    analyze_input_string();
    print_analyze_process();
    return 0;
}