#include <iostream>
#include <vector>
#include <string>
#include <string.h>

#define MAXSIZE 1000000

using namespace std;

struct Token {
	string type;
	int pos;
	int len;
	string value;
};

int main(int argc, char *argv[]) {
	string input;
	vector<string> output;
	vector<Token> tokens;
	char chars[MAXSIZE];
	int len = 0;
	if (argc > 2 || (argc == 2 && *argv[1] != '0' && *argv[1] != '1' && *argv[1] != '2')) {
		Token newOne;
		newOne.len = 0;
		newOne.pos = 0;
		newOne.type = "97(ERR1)";
		newOne.value = "[]";
		tokens.push_back(newOne);

		cout << endl;
		int printed = 0;;
		for (size_t vecSize = 0; vecSize < tokens.size(); vecSize++) {
			printed++;
			cout << "TID: " << vecSize + 1 << ", TYPE: " << tokens[vecSize].type << ", POS: " << tokens[vecSize].pos << ", LEN: " << tokens[vecSize].len;
			if (tokens[vecSize].value != "[]") {
				cout << ", VALUE: " << tokens[vecSize].value;
			}
			cout << endl;
		}

		cout << endl;
		cout << "Totals: len = " << len << ", tokens = " << tokens.size() << ", printed = " << printed << endl;
	}
	else {
		cin.getline(chars, MAXSIZE, -1);
		len = strlen(chars);
		bool eofchar = false;
		if (chars[len - 1] == 26) {
			len--;
		}
		if (chars[len - 1] != '\n') {
			eofchar = true;
		}
		for (int i = 0; i < len; i++) {
			if (chars[i] == ' ') {
				int j = i;
				string dummy(1, chars[i]);
				while (chars[j + 1] == ' ') {
					j++;
					string dum(1, j + 1);
					dummy += dum;
				}
				Token newOne;
				newOne.type = "5(WS)";
				newOne.pos = i;
				newOne.len = j - i + 1;
				newOne.value = "[]";
				i = j;
				tokens.push_back(newOne);
			}//Checked
			else if (chars[i] == '\n') {
				string dummy(1, chars[i]);
				Token newOne;
				newOne.type = "6(NEWLINE)";
				newOne.pos = i;
				newOne.len = 1;
				newOne.value = "[]";
				tokens.push_back(newOne);
			}//Checked
			else if (chars[i] == '"') {
				int j = i;
				string dummy = "";
				bool fail = false;
				Token newOne;
				while (j + 1 < len && chars[j + 1] != '"' && chars[j + 1] != '\n') {
					if (chars[j + 1] != 9 && chars[j + 1] != 10 && (chars[j + 1] < 32 && chars[j + 1] > 126)) {
						fail = true;
						break;
					}
					string dum(1, chars[j + 1]);
					dummy += dum;
					j++;
				}
				if (chars[j + 1] == '"') {
					j++;
					newOne.type = "2(STR)";
					newOne.value = dummy;
					newOne.pos = i;
					newOne.len = j - i + 1;
					i = j;
				}
				else if (j + 1 >= len || chars[j + 1] == '\n' || fail) {
					newOne.value = "[]";
					newOne.type = "98(ERR2)";
					newOne.pos = i;
					newOne.len = j - i + 1;
					i = j;
				}
				tokens.push_back(newOne);
			}//Checked
			else if ((chars[i] >= 48 && chars[i] <= 57) || chars[i] == 46) {
				string dummy(1, chars[i]);
				int j = i;
				int dot = 0;
				if (chars[i] == 46) {
					dot++;
				}
				Token newOne;
				while (j + 1 < len && ((chars[j + 1] >= 48 && chars[j + 1] <= 57) || (dot == 0 && chars[j + 1] == 46))) {
					string dum(1, chars[j + 1]);
					if (chars[j + 1] == 46) {
						dot++;
					}
					dummy += dum;
					j++;
				}
				newOne.len = j - i + 1;
				newOne.pos = i;
				if (dot == 1 && newOne.len == 1) {
					newOne.type = "100(ERR4)";
					newOne.value = "[]";
				}
				else if (dot == 1 && newOne.len > 1) {
					newOne.value = dummy;
					newOne.type = "4(FLOAT)";
				}
				else {
					newOne.value = dummy;
					newOne.type = "3(INT)";
				}
				tokens.push_back(newOne);
				i = j;
			}//Checked
			else if (chars[i] != 9 && chars[i] != 10 && (chars[i] < 32 || chars[i] > 126)) {
				int j = i;
				Token newOne;
				while (j + 1 < len && (chars[j + 1] != 9 && chars[j + 1] != 10 && (chars[j + 1] < 32 || chars[j + 1] > 126))) {
					j++;
				}
				newOne.len = j - i + 1;
				newOne.type = "99(ERR3)";
				newOne.value = "[]";
				newOne.pos = i;
				i = j;
				tokens.push_back(newOne);
			}//Checked
			else if ((chars[i] >= 65 && chars[i] <= 90) || (chars[i] >= 97 && chars[i] <= 122) || chars[i] == 95) {
				Token newOne;
				if (chars[i] == 'f' && chars[i + 1] == 'o' && chars[i + 2] == 'r' && (chars[i + 3] < 65 || chars[i + 3] > 90) && (chars[i + 3] < 97 || chars[i + 3] > 122) && (chars[i + 3] < 48 || chars[i + 3] > 57) && chars[i + 3] != 95) {
					newOne.len = 3;
					newOne.pos = i;
					newOne.type = "8(FOR)";
					newOne.value = "for";
					tokens.push_back(newOne);
					i = i + 2;
				}
				else if (chars[i] == 'i' && chars[i + 1] == 'f' && (chars[i + 2] < 65 || chars[i + 2] > 90) && (chars[i + 2] < 97 || chars[i + 2] > 122) && (chars[i + 2] < 48 || chars[i + 2] > 57) && chars[i + 2] != 95) {
					newOne.len = 2;
					newOne.pos = i;
					newOne.type = "10(IF)";
					newOne.value = "if";
					tokens.push_back(newOne);
					i = i + 1;
				}
				else if (chars[i] == 'w' && chars[i + 1] == 'h' && chars[i + 2] == 'i' && chars[i + 3] == 'l' && chars[i + 4] == 'e' && (chars[i + 5] < 65 || chars[i + 5] > 90) && (chars[i + 5] < 97 || chars[i + 5] > 122) && (chars[i + 5] < 48 || chars[i + 5] > 57) && chars[i + 5] != 95) {
					newOne.len = 5;
					newOne.pos = i;
					newOne.type = "9(WHILE)";
					newOne.value = "while";
					tokens.push_back(newOne);
					i = i + 4;
				}
				else if (chars[i] == 'e' && chars[i + 1] == 'l' && chars[i + 2] == 's' && chars[i + 3] == 'e' && (chars[i + 4] < 65 || chars[i + 4] > 90) && (chars[i + 4] < 97 || chars[i + 4] > 122) && (chars[i + 4] < 48 || chars[i + 4] > 57) && chars[i + 4] != 95) {
					newOne.len = 4;
					newOne.pos = i;
					newOne.type = "11(ELSE)";
					newOne.value = "else";
					tokens.push_back(newOne);
					i = i + 3;
				}
				else {
					int j = i;
					string dummy(1, chars[i]);
					while ((chars[j + 1] >= 65 && chars[j + 1] <= 90) || (chars[j + 1] >= 97 && chars[j + 1] <= 122) || chars[j + 1] == 95 || (chars[j + 1] >= 48 && chars[j + 1] <= 57)) {
						string dum(1, chars[j + 1]);
						dummy += dum;
						j++;
					}
					newOne.len = j - i + 1;
					newOne.pos = i;
					newOne.type = "1(ID)";
					newOne.value = dummy;
					tokens.push_back(newOne);
					i = j;
				}
			}//Checked
			else if (chars[i] == '#') {
				int j = i;
				string dummy(1, chars[i]);
				Token newOne;
				while (j + 1 < len && chars[j + 1] != '\n') {
					string dum(1, chars[j + 1]);
					dummy += dum;
					j++;
				}
				newOne.len = j - i + 1;
				newOne.pos = i;
				newOne.type = "7(COMMENT)";
				newOne.value = dummy;
				tokens.push_back(newOne);
				i = j;
			}//Checked
			else if (chars[i] == '+') {
				Token newOne;
				newOne.len = 1;
				newOne.pos = i;
				newOne.type = "16(+)";
				newOne.value = "[]";
				tokens.push_back(newOne);
			}//Checked
			else if (chars[i] == '-') {
				Token newOne;
				newOne.len = 1;
				newOne.pos = i;
				newOne.type = "17(-)";
				newOne.value = "[]";
				tokens.push_back(newOne);
			}//Checked
			else if (chars[i] == '*') {
				Token newOne;
				newOne.len = 1;
				newOne.pos = i;
				newOne.type = "18(*)";
				newOne.value = "[]";
				tokens.push_back(newOne);
			}//Checked
			else if (chars[i] == '/') {
				Token newOne;
				newOne.len = 1;
				newOne.pos = i;
				newOne.type = "19(/)";
				newOne.value = "[]";
				tokens.push_back(newOne);
			}//Checked
			else if (chars[i] == '{') {
				Token newOne;
				newOne.len = 1;
				newOne.pos = i;
				newOne.type = "20({)";
				newOne.value = "[]";
				tokens.push_back(newOne);
			}//Checked
			else if (chars[i] == '}') {
				Token newOne;
				newOne.len = 1;
				newOne.pos = i;
				newOne.type = "21(})";
				newOne.value = "[]";
				tokens.push_back(newOne);
			}//Checked
			else if (chars[i] == '!') {
				Token newOne;
				newOne.len = 1;
				newOne.pos = i;
				newOne.value = "[]";
				if (chars[i + 1] == '=') {
					newOne.len = 2;
					newOne.type = "31(!=)";
					i++;
				}
				else {
					newOne.type = "12(!)";
				}
				tokens.push_back(newOne);
			}//Checked
			else if (chars[i] == '%') {
				Token newOne;
				newOne.len = 1;
				newOne.pos = i;
				newOne.type = "13(%)";
				newOne.value = "[]";
				tokens.push_back(newOne);
			}//Checked
			else if (chars[i] == '&') {
				Token newOne;
				newOne.len = 1;
				newOne.pos = i;
				newOne.type = "14(&)";
				newOne.value = "[]";
				tokens.push_back(newOne);
			}//Checked
			else if (chars[i] == '|') {
				Token newOne;
				newOne.len = 1;
				newOne.pos = i;
				newOne.type = "15(|)";
				newOne.value = "[]";
				tokens.push_back(newOne);
			}//Checked
			else if (chars[i] == '[') {
				Token newOne;
				newOne.len = 1;
				newOne.pos = i;
				newOne.type = "22([)";
				newOne.value = "[]";
				tokens.push_back(newOne);
			}//Checked
			else if (chars[i] == ']') {
				Token newOne;
				newOne.len = 1;
				newOne.pos = i;
				newOne.type = "23(])";
				newOne.value = "[]";
				tokens.push_back(newOne);
			}//Checked
			else if (chars[i] == ';') {
				Token newOne;
				newOne.len = 1;
				newOne.pos = i;
				newOne.type = "24(;)";
				newOne.value = "[]";
				tokens.push_back(newOne);
			}//Checked
			else if (chars[i] == ',') {
				Token newOne;
				newOne.len = 1;
				newOne.pos = i;
				newOne.type = "25(,)";
				newOne.value = "[]";
				tokens.push_back(newOne);
			}//Checked
			else if (chars[i] == '>') {
				Token newOne;
				newOne.len = 1;
				newOne.pos = i;
				newOne.value = "[]";
				if (chars[i + 1] == '=') {
					newOne.len = 2;
					newOne.type = "30(>=)";
					i++;
				}
				else {
					newOne.type = "27(>)";
				}
				tokens.push_back(newOne);
			}//Checked
			else if (chars[i] == '<') {
				Token newOne;
				newOne.len = 1;
				newOne.pos = i;
				newOne.value = "[]";
				if (chars[i + 1] == '=') {
					newOne.len = 2;
					newOne.type = "29(<=)";
					i++;
				}
				else {
					newOne.type = "26(<)";
				}
				tokens.push_back(newOne);
			}//Checked
			else if (chars[i] == '=') {
				Token newOne;
				newOne.len = 1;
				newOne.pos = i;
				newOne.type = "28(=)";
				newOne.value = "[]";
				tokens.push_back(newOne);
			}//Checked
			else if (chars[i] == ':') {
				Token newOne;
				newOne.len = 1;
				newOne.pos = i;
				newOne.value = "[]";
				if (chars[i + 1] == '=') {
					newOne.len = 2;
					newOne.type = "32(:=)";
					i++;
				}
				else {
					newOne.type = "100(ERR4)";
				}
				tokens.push_back(newOne);
			}//Checked
			else if (chars[i] == '(' || chars[i] == ')' || chars[i] == '$' || chars[i] == '?' || chars[i] == '@' || chars[i] == '\\' || chars[i] == '^' || chars[i] == '`' || chars[i] == '~') {
				Token newOne;
				newOne.pos = i;
				newOne.len = 1;
				newOne.type = "100(ERR4)";
				newOne.value = "[]";
				tokens.push_back(newOne);
			}//Checked
		}
		if (eofchar) {
			cout << endl;
		}
		int printed = 0;;
		for (size_t vecSize = 0; vecSize < tokens.size(); vecSize++) {
			if ( argc == 1 || *argv[1] == '0') {
				printed++;
				cout << "TID: " << vecSize + 1 << ", TYPE: " << tokens[vecSize].type << ", POS: " << tokens[vecSize].pos << ", LEN: " << tokens[vecSize].len;
				if (tokens[vecSize].value != "[]") {
					cout << ", VALUE: " << tokens[vecSize].value;
				}
				cout << endl;
			}
			else if (*argv[1] == '1' && tokens[vecSize].type != "7(COMMENT)" && tokens[vecSize].type != "5(WS)" && tokens[vecSize].type != "6(NEWLINE)" && tokens[vecSize].type != "97(ERR1)" && tokens[vecSize].type != "98(ERR2)" && tokens[vecSize].type != "99(ERR3)" && tokens[vecSize].type != "100(ERR4)") {
				printed++;
				cout << "TID: " << vecSize + 1 << ", TYPE: " << tokens[vecSize].type << ", POS: " << tokens[vecSize].pos << ", LEN: " << tokens[vecSize].len;
				if (tokens[vecSize].value != "[]") {
					cout << ", VALUE: " << tokens[vecSize].value;
				}
				cout << endl;
			}
			else if (*argv[1] == '2' && (tokens[vecSize].type == "7(COMMENT)" || tokens[vecSize].type == "5(WS)" || tokens[vecSize].type == "6(NEWLINE)" || tokens[vecSize].type == "97(ERR1)" || tokens[vecSize].type == "98(ERR2)" || tokens[vecSize].type == "99(ERR3)" || tokens[vecSize].type == "100(ERR4)")) {
				printed++;
				cout << "TID: " << vecSize + 1 << ", TYPE: " << tokens[vecSize].type << ", POS: " << tokens[vecSize].pos << ", LEN: " << tokens[vecSize].len;
				if (tokens[vecSize].value != "[]") {
					cout << ", VALUE: " << tokens[vecSize].value;
				}
				cout << endl;
			}
		}

		cout << endl;
		cout << "Totals: len = " << len << ", tokens = " << tokens.size() << ", printed = " << printed << endl;
	}
	
}
