#define _CRT_SECURE_NO_WARNINGS

#ifndef __LONG_CALC__
#define __LONG_CALC__

#ifndef max
#define max(a,b) ((a)>(b) ? (a) : (b))
#endif

#include <stdlib.h>
#include <math.h>
#include <string>

using namespace std;

class LongNumber { 
private: 
	string data; // длинное число в строковом представлении
	string out; // ƒлинное число дл€ вывода на экран
	bool neg; // явл€етс€ ли число отрицательным
	int base; // ќснование системы счислени€

	char digit_to_char(int d) { 
		return d < 10 ? ('0' + d) : ('A' + d - 10);
	}
	int char_to_digit(char c) { 
		char cc = toupper(c); 
		return cc >= '0' && cc <= '9' ? (cc - '0') : (cc - 'A' + 10);
	}
	
	char add_digits(char d1, char d2, int & carry) {
		int sum = char_to_digit(d1) + char_to_digit(d2) + carry;
		if (sum < base) { 
			carry = 0;
			return digit_to_char(sum);
		} else { 
			carry = 1;
			return digit_to_char(sum - base);
		}
	}
	char mul_digits(char d1, char d2, int & carry) {
	int result = char_to_digit(d1) * char_to_digit(d2) + carry;
	if (result < base) { 
			carry = 0;
			return digit_to_char(result);
		} else { 
			carry = result / base;
			return digit_to_char(result % base);
		}
	}
	
	char sub_digits(char d1, char d2, int & borrow) {
		int dif = char_to_digit(d1) - char_to_digit(d2) - borrow;
		if (dif >= 0) { 
			borrow = 0;
			return digit_to_char(dif);
		} else { 
			borrow = 1;
			return digit_to_char(dif + base);
		}
	}
	void mulpow_base(int k) { 
		data += string(k, '0'); 
	}
public: 
	LongNumber(const char * number, bool neg, int base) {
		data = number;
		for (unsigned int i = 0; i < data.length(); i++)
			data[i] = toupper(data[i]);
		this->neg = neg;
		this->base = base;
	}
	bool negative() { 
		return neg || data.length() > 0 && data[0] == '-';
	}
	const char * get_with_base() {
		string end = "";
		if (base != 10) {
			char buf[10];
			end += "_";
			end += _itoa(base, buf, 10);
		}
		out = data + end;

		return out.c_str();
	}
	const char * get() {
		return data.c_str();
	}
	
	int get_int() {
		int result = 0;
		int mul = neg ? -1 : +1;
		unsigned int counter = 0;
		if (data.length() > 0 && data[0] == '-') {
			counter++;
			mul = -1;
		}
		for (; counter < data.length(); counter++)
			result = result * base + char_to_digit(data[counter]);

		return mul*result;
	}
	
	LongNumber * add(LongNumber * op2) {
		if (negative() || op2->negative()) { 
			if (negative() && op2->negative()) { 
				neg = false;
				op2->neg = false;
				LongNumber * result = op2->add(this);
				result->neg = true;
				neg = true;
				op2->neg = true;
				return result;
			} if (negative()) { 
				neg = false;
				LongNumber * result = op2->sub(this);
				neg = true;
				return result;
			} else { 
				op2->neg = false;
				LongNumber * result = sub(op2);
				op2->neg = true;
				return result;
			}
		}
		if (base != op2->base) { 
			if (base > op2->base) {
				LongNumber * buf = op2->rebase(base);
				LongNumber * result = add(buf);
				delete buf;
				return result;
			}
			else {
				LongNumber * buf = rebase(op2->base);
				LongNumber * result = buf->add(op2);
				delete buf;
				return result;
			}
		}
		string data2 = op2->get();
		string result = "";

		int L = max(data.length(), data2.length()); 
		int p1 = data.length() - 1;
		int p2 = data2.length() - 1;
		int carry = 0; 
		for (int i = 0; i < L; i++) { 
			char digit[2]; 
			if (p1 >= 0 && p2 >= 0)
				digit[0] = add_digits(data[p1], data2[p2], carry);
			else if (p1 >= 0)
				digit[0] = add_digits(data[p1], '0', carry);
			else if (p2 >= 0)
				digit[0] = add_digits('0', data2[p2], carry);
			digit[1] = 0; 

			result.insert(0, digit);

			p1--;
			p2--;
		}
		
		if (carry > 0) {
			result.insert(0, "1");
		}

		while (result.length() > 1 && result[0] == '0')
			result.erase(0, 1);

		return new LongNumber(result.c_str(), false, base);
	}
	
	LongNumber * sub(LongNumber * op2) {
		if (negative() || op2->negative()) { 
			if (negative() && op2->negative()) { 
				neg = false;
				op2->neg = false;
				LongNumber * result = op2->sub(this);
				neg = true;
				op2->neg = true;
				return result;
			} if (negative()) { 
				neg = false;
				LongNumber * result = op2->add(this);
				result->neg = true;
				neg = true;
				return result;
			} else { 
				op2->neg = false;
				LongNumber * result = add(op2);
				op2->neg = true;
				return result;
			}
		}
		if (base != op2->base) { 
			if (base > op2->base) {
				LongNumber * buf = op2->rebase(base);
				LongNumber * result = sub(buf);
				delete buf;
				return result;
			}
			else {
				LongNumber * buf = rebase(op2->base);
				LongNumber * result = buf->sub(op2);
				delete buf;
				return result;
			}
		}
		string data2 = op2->get();
		string result = "";

		int L = max(data.length(), data2.length()); 
		int p1 = data.length() - 1;
		int p2 = data2.length() - 1;
		int borrow = 0; 
		for (int i = 0; i < L; i++) { 
			char digit[2]; 
			if (p1 >= 0 && p2 >= 0) 
				digit[0] = sub_digits(data[p1], data2[p2], borrow);
			else if (p1 >= 0) 
				digit[0] = sub_digits(data[p1], '0', borrow);
			else if (p2 >= 0) 
				digit[0] = sub_digits('0', data2[p2], borrow);
			digit[1] = 0; 

			result.insert(0, digit);

			p1--;
			p2--;
		}

		while (result.length() > 1 && result[0] == '0')
			result.erase(0, 1);

		if (borrow > 0) {
			LongNumber * rev_rslt = op2->sub(this);
			result = rev_rslt->get();
			delete rev_rslt;
		}

		return new LongNumber(result.c_str(), borrow > 0, base);
	}
	
	LongNumber * mul(LongNumber * op2) {
		if (negative() || op2->negative()) { 
			if (negative() && op2->negative()) { 
				neg = false;
				op2->neg = false;
				LongNumber * result = mul(op2);
				neg = true;
				op2->neg = true;
				return result;
			} if (negative()) { 
				neg = false;
				LongNumber * result = mul(op2);
				result->neg = true;
				neg = true;
				return result;
			} else { 
				op2->neg = false;
				LongNumber * result = mul(op2);
				result->neg = true;
				op2->neg = true;
				return result;
			}
		}
		if (base != op2->base) { 
			if (base > op2->base) {
				LongNumber * buf = op2->rebase(base);
				LongNumber * result = mul(buf);
				delete buf;
				return result;
			}
			else {
				LongNumber * buf = rebase(op2->base);
				LongNumber * result = buf->mul(op2);
				delete buf;
				return result;
			}
		}
		string data2 = op2->get(); 
		LongNumber * result = new LongNumber("0", false, base); 
		
		for (int p = data2.length() - 1; p >= 0; p--) {
			string _result = ""; 
			int carry = 0;
			for (int i = data.length() - 1; i >= 0; i--) {
				char digit[2]; 
				digit[0] = mul_digits(data[i], data2[p], carry);
				digit[1] = 0; 

				_result.insert(0, digit);
			}
			
			if (carry > 0) {
				_result = digit_to_char(carry) + _result;
			}
			
			LongNumber * addition = new LongNumber(_result.c_str(), false, base);
			addition->mulpow_base(data2.length() - p -1);
			LongNumber * prev_result = result; 
			result = result->add(addition); 
			delete prev_result; 
			delete addition; 
		}
		return result;
	}
	
	LongNumber * div(LongNumber * op2) {
		if (negative() || op2->negative()) { 
			if (negative() && op2->negative()) {
				neg = false;
				op2->neg = false;
				LongNumber * result = div(op2);
				neg = true;
				op2->neg = true;
				return result;
			} if (negative()) { 
				neg = false;
				LongNumber * result = div(op2);
				result->neg = true;
				neg = true;
				return result;
			} else { 
				op2->neg = false;
				LongNumber * result = div(op2);
				result->neg = true;
				op2->neg = true;
				return result;
			}
		}
		if (base != op2->base) { 
			if (base > op2->base) {
				LongNumber * buf = op2->rebase(base);
				LongNumber * result = div(buf);
				delete buf;
				return result;
			}
			else {
				LongNumber * buf = rebase(op2->base);
				LongNumber * result = buf->div(op2);
				delete buf;
				return result;
			}
		}
		string result = "0"; 
		string op1 = ""; 
		for (unsigned int k = 0; k < data.length(); k++) {
			op1 += data[k]; 
			LongNumber * _op1 = new LongNumber(op1.c_str(), false, base);
			int digit = 0; 
			do {
				LongNumber * buf = _op1->sub(op2);
				if (buf->negative()) { 
					delete buf;
					break; 
				}
				_op1 = buf;
				digit++;
				if (digit > base) {
					return new LongNumber("INFINITY", false, base);
				}
			} while (true);
			result += digit_to_char(digit);
			op1 = _op1->data;
		}
	
		while (result.length() > 1 && result[0] == '0')
			result.erase(0, 1);
		return new LongNumber(result.c_str(), false, base);
	}
	
	LongNumber * rem(LongNumber * op2) {
		LongNumber * quot = div(op2); 
		if (string(quot->get()).compare("INFINITY") == 0) { 
			delete quot;
			return new LongNumber("NOT A NUMBER", false, base);
		}
		LongNumber * buf = quot->mul(op2); 
		LongNumber * result = sub(buf); 
		delete quot;
		delete buf;
		return result;
	}
	
	LongNumber * rebase(int new_base) {
		if (new_base < 2 || new_base > 36) { 
			return new LongNumber("NOT A NUMBER", false, base);
		}
		if (negative()) { 
			neg = false;
			LongNumber * result = rebase(new_base);
			result->neg = true;
			neg = true;
			return result;
		}
		else {
			string nb = "";
			int _nb = new_base;
			while (_nb > 0) {
				nb = digit_to_char(_nb % base) + nb;
				_nb /= base;
			}
			if (nb.length() == 0) nb = "0";
			LongNumber * rest = new LongNumber(*this);
			LongNumber * _new_base = new LongNumber(nb.c_str(), false, base);
			string result = "";
			while (rest->data.length() > 1 || rest->get_int() > 0) {
				LongNumber * digit = rest->rem(_new_base);
				if (digit->data == "NOT A NUMBER") {
					delete rest;
					delete _new_base;
					return digit;
				}
				result = digit_to_char(digit->get_int()) + result;
				LongNumber * rest2 = rest->div(_new_base);
				delete digit;
				delete rest;
				rest = rest2;
			}
			delete rest;
			delete _new_base;
			return new LongNumber(result.c_str(), false, new_base);
		}
	}
};
#endif
