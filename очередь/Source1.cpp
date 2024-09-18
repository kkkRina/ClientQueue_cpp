#include <iostream>
#include <fstream>
#include "Header.h"

using namespace std;

//void distribute(const char* filename, queue& credit_q, queue& deposit_q);
man create_man(char* man_data);
bool is_credit(man m);
void save(const char* filename, queue& q);

char* decode(char* word);
char* substr(char* arr, int begin, int len); 
int append(char* s, size_t size, char c);
char* get_menu(const char* menu_filename); 
char* get_content();
char* parse(const char* buf);
void load_content(const char* template_name);


/*void distribute(const char* filename, queue& credit_q, queue& deposit_q)
{
	ifstream f(filename);
	if (f.is_open())
	{
		const int linelen = 256;
		char* line = new char[linelen];
		while (!f.eof())
		{
			f.getline(line, linelen);
			auto man = create_man(line);
			if (is_credit(man))
			{
				enqueue(credit_q, man);
			}
			else
			{
				enqueue(deposit_q, man);
			}
		}
		delete[] line;
		f.close();
	}
}*/
man create_man(char* man_data)
{
	man m;

	char* name = new char[150];
	char* sex = new char[10];
	char* job = new char[150];
	char* aim = new char[150];

	sscanf_s(man_data, "%s %s %d %s %d %s", name, 150, sex, 10, &m.age, job, 150, &m.cash, aim,150);

	strcpy_s(m.name, name);
	strcpy_s(m.sex, sex);
	strcpy_s(m.job, job);
	strcpy_s(m.aim, aim);
	
	delete[] name;
	delete[] sex;
	delete[] job;
	delete[] aim;
	return m;
}
bool is_credit(man m)
{
	const char* aim = "Кредит";
	if (strcmp(m.aim, aim) == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
bool is_deposit(man m)
{
	const char* aim = "Вклад";
	if (strcmp(m.aim, aim) == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void save(const char* filename, queue& q)
{
	ofstream f(filename);
	if (f.is_open())
	{
		man m;
		while (dequeue(q, m))
		{
			f << m.name << " " << m.sex << " " << m.age << " " << m.job << " " << m.cash << " " << m.aim << endl;
		}
		f.close();
	}
}
void main()
{
	setlocale(LC_ALL, "Rus");
	cout << "Content-type: text/html; charset=windows-1251\n\n";
	load_content("queue.html");
}
char* decode(char* word)
{
	const int sz = 4096;
	char* new_str = new char[sz];
	new_str[0] = 0;
	char a;
	int j;

	for (int i = 0; i < strlen(word); i++)
	{
		if (word[i] == '+')
		{
			append(new_str, sz, ' ');
		}
		else if (word[i] == '%')
		{
			sscanf_s(substr(word, i + 1, 2), "%x", &j);
			a = static_cast<char>(j);
			append(new_str, sz, a);
			i = i + 2;
		}
		else
		{
			append(new_str, sz, word[i]);
		}
	}
	return (new_str);
}
char* substr(char* arr, int begin, int len)
{
	char* res = new char[len + 1];
	for (int i = 0; i < len; i++)
		res[i] = *(arr + begin + i);
	res[len] = 0;
	return res;
}
int append(char* s, size_t size, char c) 
{
	if (strlen(s) + 1 >= size)
	{
		return 1;
	}
	int len = strlen(s);
	s[len] = c;
	s[len + 1] = '\0';
	return 0;
}
char* get_menu(const char* menu_filename)
{
	ifstream mf(menu_filename);
	const size_t res_sz = 65535;
	char* res = new char[res_sz];
	res[0] = 0;
	if (mf.is_open())
	{
		char* menuline = new char[1024];
		while (!mf.eof())
		{
			mf.getline(menuline, 1024);
			char* title;
			char* page = strtok_s(menuline, " ", &title);
			strcat_s(res, res_sz, "<div class='menuitem'>");
			strcat_s(res, res_sz, "<a href='");
			strcat_s(res, res_sz, page);
			strcat_s(res, res_sz, "'>");
			strcat_s(res, res_sz, title);
			strcat_s(res, res_sz, "</a></div>\n");
		}
		delete[] menuline;
		mf.close();
	}
	return res;
}
char* get_content()
{
	const size_t res_sz = 6553655;
	char* res = new char[res_sz];
	res[0] = 0;
	strcat_s(res, res_sz, "<form method='get' action='queue.cgi'>\n ");
	strcat_s(res, res_sz, "<label for='exp'' class = 'form - ooo color - text''>Введите список : <br>\n");
	strcat_s(res, res_sz,"Формат: И.О.Фамилия, пол, возраст, профессия, зарплата, цель обращения\n </label><br>\n");
	strcat_s(res, res_sz,"<textarea type='text' name='exp' id='exp' rows='20' cols='80' required></textarea>\n");
	strcat_s(res, res_sz,"<div class='form - button''>\n");
	strcat_s(res, res_sz,"<input type='submit' class='submit - button' value='Распределить'>\n");
	strcat_s(res, res_sz,"</div>\n");
	strcat_s(res, res_sz, "</form>\n");

	queue q1, q2;
	char* text = new char[40960];
	size_t len;
	errno_t err = _dupenv_s(&text, &len, "QUERY_STRING");

	if (strcmp(text, "") != 0)
	{
		char* next_token;
		char* exp = strtok_s(text, "=", &next_token);
		char* pepel = strtok_s(NULL, "=", &next_token);
		char* dec_pepel = decode(pepel);
		char* chel = strtok_s(dec_pepel, "\n", &next_token);
		while (chel != NULL)
		{
			auto man = create_man(chel);
			if (is_credit(man))
			{
				enqueue(q1, man);
			}
			if (is_deposit(man))
			{
				enqueue(q2, man);
			}
			chel = strtok_s(NULL, "\n", &next_token);

		}
		save("Credit.txt", q1);
		save("C:/Users/Darina/source/repos/очередь/очередь/Credit.txt", q1);
		save("Deposit.txt", q2);
		save("C:/Users/Darina/source/repos/очередь/очередь/Deposit.txt", q2);
	}
	int k = 10000;
	strcat_s(res,res_sz, "Клиенты, желающие оформить кредит: <br>\n");
	ifstream credit("Credit.txt");
	while (!credit.eof())
	{
		char* str = new char[k];
		credit.getline(str, k);
		strcat_s(res, res_sz, str);
		strcat_s(res,res_sz,"<br>\n");
	}
	strcat_s(res,res_sz, "<br>\n Клиенты, желающие оформить вклад: <br>\n");
	ifstream dep("Deposit.txt");
	while (!dep.eof())
	{
		char* stt = new char[k];
		dep.getline(stt, k);
		strcat_s(res, res_sz, stt);
		strcat_s(res, res_sz, "<br>\n");
	}
	return res;
}
char* parse(const char* buf)
{
	char* res;
	if (!strcmp(buf, "<!--menu-->"))
	{
		res = get_menu("menu.txt");
	}
	else if (!strcmp(buf, "<!--content-->"))
	{
		res = get_content();
	}
	else
	{
		res = new char[strlen(buf) + 1];
		strcpy_s(res, strlen(buf) + 1, buf);
	}
	return res;
}
void load_content(const char* template_name)
{
	ifstream tf(template_name);
	if (tf.is_open())
	{
		const size_t buf_sz = 65535;
		char* buf = new char[buf_sz];
		while (!tf.eof())
		{
			tf.getline(buf, buf_sz);
			char* parsed_buf = parse(buf);
			cout << parsed_buf << endl;
			delete[] parsed_buf;
		}
		delete[] buf;
		tf.close();
	}
}
/*queue q1, q2;
	char* text = new char[4096];
	size_t len;
	errno_t err = _dupenv_s(&text, &len, "QUERY_STRING");

	if (text != 0)
	{
		char* next_token;
		char* exp = strtok_s(text, "=", &next_token);
		char* pepel = strtok_s(NULL, "=", &next_token);
		char* dec_pepel = decode(pepel);
		char* chel = strtok_s(dec_pepel, "\n", &next_token);
		while (chel != NULL)
		{
			auto man = create_man(chel);
			if (is_credit(man))
			{
				enqueue(q1, man);
			}
			if (is_deposit(man))
			{
				enqueue(q2, man);
			}
			chel = strtok_s(NULL, "\n", &next_token);

		}
		save("Credit.txt", q1);
		save("C:/Users/Darina/source/repos/очередь/очередь/Credit.txt", q1);
		save("Deposit.txt", q2);
		save("C:/Users/Darina/source/repos/очередь/очередь/Deposit.txt", q2);
		//load_content("queue.html");
	}*/
