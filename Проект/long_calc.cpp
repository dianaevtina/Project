#include "long_nums.h"
#include <string.h>
#include <ctype.h>
#include <iostream>

using namespace std;

int main() {
	setlocale(LC_ALL, "RUS");
	cout << "Введите 2 числа по типу число1 операция число2 без пробелов" << endl;
	cout << "Возможные операции: +,-,*,>" << endl; // > - операция приведения 1-го числа к основанию 2-го числа
	cout << "Для числа можно указать основание системы исчисления по типу число_основание" << endl;
	cout << "Если основание не указано, оно по умолчанию 10" << endl;
	cout << "Для выхода наберите exit" << endl;
	bool ExitFlag = false; 
	do {
		char buf[200]; // буферный массив для ввода команды
		cout << "Ввод: ";
		cin.getline(buf, sizeof(buf)); 
		if (_stricmp(buf, "exit") == 0)
			ExitFlag = true;
		else {
			char op1[100]; // Первое число
			char * _op1 = op1;
			bool neg1 = false; // Отрицательно ли первое число
			int base1 = 10; // Основание первого числа
			char op2[100]; // Второе число
			char * _op2 = op2;
			bool neg2 = false; // Отрицательно ли второе число
			int base2 = 10; // Основание второго числа
			char * cur = buf; 
			
			for (char * cur1 = buf; *cur1 != 0; cur1++) {
				if (*cur1 != ' ') { 
					*cur = *cur1; 
					cur++;
				}
			}
			*cur = 0;
			cur = buf;
		
			while (*cur == '-') {
				neg1 = !neg1;
				cur++;
			}
			
			while (*cur != 0 && (isdigit(*(unsigned char *)cur) ||
				                 *cur >= 'A' && *cur <= 'Z' ||
								 *cur >= 'a' && *cur <= 'z')) {
				*_op1 = *cur; 
				_op1++;
				cur++;
			}
			*_op1 = 0; 
			if (strlen(op1) == 0) { 
				cout << "Нет первого числа"<<endl;
				continue; 
			}
			if (*cur == '_') { 
				cur++;
				base1 = 0;
				while (isdigit(*cur)) {
					base1 = base1 * 10 + *cur - '0';
					cur++;
				}
				if (base1 < 2 || base1 > 36) {
					cout << "Основание должно быть в пределах [2,36]" << endl;
					continue; 
				}
			}
			
			if (*cur == 0 || strchr("+-*>", *cur) == NULL) {
				cout << "Неизвестная операция" << endl;
				continue; 
			}
			
			char Oper = *cur;
			cur++;
			
			while (*cur == '-') {
				neg2 = !neg2;
				cur++;
			}
			
			while (*cur != 0 && (isdigit(*(unsigned char *)cur) ||
				                     *cur >= 'A' && *cur <= 'Z' ||
									 *cur >= 'a' && *cur <= 'z')) {
				*_op2 = *cur; 
				_op2++;
				cur++;
			}
			*_op2 = 0; 
			if (strlen(op2) == 0) { 
				cout << "Нет второго числа" << endl;
				continue; 
			}
			if (*cur == '_') { 
				cur++;
				base2 = 0;
				while (isdigit(*cur)) {
					base2 = base2 * 10 + *cur - '0';
					cur++;
				}
				if (base2 < 2 || base2 > 36) {
					cout << "Основание должно быть в пределах [2,36]" << endl;
					continue; 
				}
			}
		
			LongNumber * A = new LongNumber(op1, neg1, base1);
			LongNumber * B = new LongNumber(op2, neg2, base2);
			LongNumber * C = NULL;
			
			switch (Oper) {
				case '+': C = A->add(B); break;
				case '-': C = A->sub(B); break;
				case '*': C = A->mul(B); break;
				case '>': C = A->rebase(B->get_int()); break;
			}
			
			if (C != NULL)
				cout << "= " << (C->negative() ? "-" : "") << C->get_with_base() << endl;
	
			delete A;
			delete B;
			delete C;
		}
	} while (!ExitFlag);

	return 0;
}