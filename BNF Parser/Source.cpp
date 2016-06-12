#include <iostream>
#include <vector>
#include <string>
#include <string.h>

#define MAXSIZE 1000000

using namespace std;

struct Token {
	//string type;
	string numType;
	string engType;
	int pos;
	int len;
	int tid;
	string value;
};

struct Node {
	string stmtType;
	int id;
	string numType;
	string engType;
	int position;
	int length;
	string value;
	vector<Node*> children;
};

void parseStmt(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseWhile(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseCondExpr(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseAndExpr(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseEqExpr(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseCmpExpr(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseExpr(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseMultExpr(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseUnaryExpr(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseParenExpr(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseMultExprR(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseExprR(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseCmpExprR(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseEqExprR(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseCondExprR(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseIf(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseOptElse(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseBasicExpr(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseAndExprR(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseFor(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseAssgn(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void parseOutput(vector<Token>& parseVec, int& index, Node& root, Node* parent);
void displayTree(const Node& root);

int main(int argc, char *argv[]) {
	string input;
	vector<string> output;
	vector<Token> tokens;
	char chars[MAXSIZE];
	int len = 0;
	//if (argc > 2 || (argc == 2 && *argv[1] != '0' && *argv[1] != '1' && *argv[1] != '2')) {
	//	Token newOne;
	//	newOne.len = 0;
	//	newOne.pos = 0;
	//	//newOne.type = "97(ERR1)";
	//	newOne.numType = "97";
	//	newOne.engType = "ERR1";
	//	newOne.value = "[]";
	//	tokens.push_back(newOne);

	//	int printed = 0;;
	//	for (size_t vecSize = 0; vecSize < tokens.size(); vecSize++) {
	//		printed++;
	//		cout << "TID: " << vecSize + 1 << ", TYPE: " << tokens[vecSize].numType << "(" << tokens[vecSize].engType << ")" << ", POS: " << tokens[vecSize].pos << ", LEN: " << tokens[vecSize].len;
	//		if (tokens[vecSize].value != "[]") {
	//			cout << ", VALUE: " << tokens[vecSize].value;
	//		}
	//		cout << endl;
	//	}

	//	cout << endl;
	//	cout << "Totals: len = " << len << ", tokens = " << tokens.size() << ", printed = " << printed << endl;
	//}
	//else {
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
		if (chars[i] == ' ' || chars[i] == '\t') {
			int j = i;
			string dummy(1, chars[i]);
			while (chars[j + 1] == ' ' || chars[j + 1] == '\t') {
				j++;
				string dum(1, j + 1);
				dummy += dum;
			}
			Token newOne;
			//newOne.type = "5(WS)";
			newOne.numType = "5";
			newOne.engType = "WS";
			newOne.pos = i;
			newOne.len = j - i + 1;
			newOne.value = "[]";
			i = j;
			tokens.push_back(newOne);
		}//Checked
		else if (chars[i] == '\n') {
			string dummy(1, chars[i]);
			Token newOne;
			//newOne.type = "6(NEWLINE)";
			newOne.numType = "6";
			newOne.engType = "NEWLINE";
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
				if (chars[j + 1] != 9 && chars[j + 1] != 10 && (chars[j + 1] < 32 || chars[j + 1] > 126)) {
					fail = true;
					break;
				}
				string dum(1, chars[j + 1]);
				dummy += dum;
				j++;
			}
			if (chars[j + 1] == '"') {
				j++;
				//newOne.type = "2(STR)";
				newOne.numType = "2";
				newOne.engType = "STR";
				newOne.value = dummy;
				newOne.pos = i;
				newOne.len = j - i + 1;
				i = j;
			}
			else if (j + 1 >= len || chars[j + 1] == '\n' || fail) {
				newOne.value = "[]";
				//newOne.type = "98(ERR2)";
				newOne.numType = "98";
				newOne.engType = "ERR2";
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
				//newOne.type = "100(ERR4)";
				newOne.numType = "100";
				newOne.engType = "ERR4";
				newOne.value = "[]";
			}
			else if (dot == 1 && newOne.len > 1) {
				newOne.value = dummy;
				//newOne.type = "4(FLOAT)";
				newOne.numType = "4";
				newOne.engType = "FLOAT";
			}
			else {
				newOne.value = dummy;
				//newOne.type = "3(INT)";
				newOne.numType = "3";
				newOne.engType = "INT";
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
			//newOne.type = "99(ERR3)";
			newOne.numType = "99";
			newOne.engType = "ERR3";
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
				//newOne.type = "8(FOR)";
				newOne.numType = "8";
				newOne.engType = "FOR";
				newOne.value = "for";
				tokens.push_back(newOne);
				i = i + 2;
			}
			else if (chars[i] == 'i' && chars[i + 1] == 'f' && (chars[i + 2] < 65 || chars[i + 2] > 90) && (chars[i + 2] < 97 || chars[i + 2] > 122) && (chars[i + 2] < 48 || chars[i + 2] > 57) && chars[i + 2] != 95) {
				newOne.len = 2;
				newOne.pos = i;
				//newOne.type = "10(IF)";
				newOne.numType = "10";
				newOne.engType = "IF";
				newOne.value = "if";
				tokens.push_back(newOne);
				i = i + 1;
			}
			else if (chars[i] == 'w' && chars[i + 1] == 'h' && chars[i + 2] == 'i' && chars[i + 3] == 'l' && chars[i + 4] == 'e' && (chars[i + 5] < 65 || chars[i + 5] > 90) && (chars[i + 5] < 97 || chars[i + 5] > 122) && (chars[i + 5] < 48 || chars[i + 5] > 57) && chars[i + 5] != 95) {
				newOne.len = 5;
				newOne.pos = i;
				//newOne.type = "9(WHILE)";
				newOne.numType = "9";
				newOne.engType = "WHILE";
				newOne.value = "while";
				tokens.push_back(newOne);
				i = i + 4;
			}
			else if (chars[i] == 'e' && chars[i + 1] == 'l' && chars[i + 2] == 's' && chars[i + 3] == 'e' && (chars[i + 4] < 65 || chars[i + 4] > 90) && (chars[i + 4] < 97 || chars[i + 4] > 122) && (chars[i + 4] < 48 || chars[i + 4] > 57) && chars[i + 4] != 95) {
				newOne.len = 4;
				newOne.pos = i;
				//newOne.type = "11(ELSE)";
				newOne.numType = "11";
				newOne.engType = "ELSE";
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
				//newOne.type = "1(ID)";
				newOne.numType = "1";
				newOne.engType = "ID";
				newOne.value = dummy;
				tokens.push_back(newOne);
				i = j;
			}
		}//Checked
		else if (chars[i] == '#') {
			int j = i;
			string dummy(1, chars[i]);
			Token newOne;
			while (j + 1 < len && chars[j + 1] != '\n' && (chars[j + 1] == '\t' || (chars[j + 1] >= 32 && chars[j + 1] <= 126))) {
				string dum(1, chars[j + 1]);
				dummy += dum;
				j++;
			}
			newOne.len = j - i + 1;
			newOne.pos = i;
			//newOne.type = "7(COMMENT)";
			newOne.numType = "7";
			newOne.engType = "COMMENT";
			newOne.value = dummy;
			tokens.push_back(newOne);
			i = j;
		}//Checked
		else if (chars[i] == '+') {
			Token newOne;
			newOne.len = 1;
			newOne.pos = i;
			//newOne.type = "16(+)";
			newOne.numType = "16";
			newOne.engType = "+";
			newOne.value = "[]";
			tokens.push_back(newOne);
		}//Checked
		else if (chars[i] == '-') {
			Token newOne;
			newOne.len = 1;
			newOne.pos = i;
			//newOne.type = "17(-)";
			newOne.numType = "17";
			newOne.engType = "-";
			newOne.value = "[]";
			tokens.push_back(newOne);
		}//Checked
		else if (chars[i] == '*') {
			Token newOne;
			newOne.len = 1;
			newOne.pos = i;
			//newOne.type = "18(*)";
			newOne.numType = "18";
			newOne.engType = "*";
			newOne.value = "[]";
			tokens.push_back(newOne);
		}//Checked
		else if (chars[i] == '/') {
			Token newOne;
			newOne.len = 1;
			newOne.pos = i;
			//newOne.type = "19(/)";
			newOne.numType = "19";
			newOne.engType = "/";
			newOne.value = "[]";
			tokens.push_back(newOne);
		}//Checked
		else if (chars[i] == '{') {
			Token newOne;
			newOne.len = 1;
			newOne.pos = i;
			//newOne.type = "20({)";
			newOne.numType = "20";
			newOne.engType = "{";
			newOne.value = "[]";
			tokens.push_back(newOne);
		}//Checked
		else if (chars[i] == '}') {
			Token newOne;
			newOne.len = 1;
			newOne.pos = i;
			//newOne.type = "21(})";
			newOne.numType = "21";
			newOne.engType = "}";
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
				//newOne.type = "31(!=)";
			newOne.numType = "31";
			newOne.engType = "!=";
				i++;
			}
			else {
				//newOne.type = "12(!)";
				newOne.numType = "12";
				newOne.engType = "!";
			}
			tokens.push_back(newOne);
		}//Checked
		else if (chars[i] == '%') {
			Token newOne;
			newOne.len = 1;
			newOne.pos = i;
			//newOne.type = "13(%)";
			newOne.numType = "13";
			newOne.engType = "%";
			newOne.value = "[]";
			tokens.push_back(newOne);
		}//Checked
		else if (chars[i] == '&') {
			Token newOne;
			newOne.len = 1;
			newOne.pos = i;
			//newOne.type = "14(&)";
			newOne.numType = "14";
			newOne.engType = "&";
			newOne.value = "[]";
			tokens.push_back(newOne);
		}//Checked
		else if (chars[i] == '|') {
			Token newOne;
			newOne.len = 1;
			newOne.pos = i;
			//newOne.type = "15(|)";
			newOne.numType = "15";
			newOne.engType = "|";
			newOne.value = "[]";
			tokens.push_back(newOne);
		}//Checked
		else if (chars[i] == '[') {
			Token newOne;
			newOne.len = 1;
			newOne.pos = i;
			//newOne.type = "22([)";
			newOne.numType = "22";
			newOne.engType = "[";
			newOne.value = "[]";
			tokens.push_back(newOne);
		}//Checked
		else if (chars[i] == ']') {
			Token newOne;
			newOne.len = 1;
			newOne.pos = i;
			//newOne.type = "23(])";
			newOne.numType = "23";
			newOne.engType = "]";
			newOne.value = "[]";
			tokens.push_back(newOne);
		}//Checked
		else if (chars[i] == ';') {
			Token newOne;
			newOne.len = 1;
			newOne.pos = i;
			//newOne.type = "24(;)";
			newOne.numType = "24";
			newOne.engType = ";";
			newOne.value = "[]";
			tokens.push_back(newOne);
		}//Checked
		else if (chars[i] == ',') {
			Token newOne;
			newOne.len = 1;
			newOne.pos = i;
			//newOne.type = "25(,)";
			newOne.numType = "25";
			newOne.engType = ",";
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
				//newOne.type = "30(>=)";
				newOne.numType = "30";
				newOne.engType = ">=";
				i++;
			}
			else {
				//newOne.type = "27(>)";
				newOne.numType = "27";
				newOne.engType = ">";
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
				//newOne.type = "29(<=)";
				newOne.numType = "29";
				newOne.engType = "<=";
				i++;
			}
			else {
				//newOne.type = "26(<)";
				newOne.numType = "26";
				newOne.engType = "<";
			}
			tokens.push_back(newOne);
		}//Checked
		else if (chars[i] == '=') {
			Token newOne;
			newOne.len = 1;
			newOne.pos = i;
			//newOne.type = "28(=)";
			newOne.numType = "28";
			newOne.engType = "=";
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
				//newOne.type = "32(:=)";
				newOne.numType = "32";
				newOne.engType = ":=";
				i++;
			}
			else {
				//newOne.type = "100(ERR4)";
				newOne.numType = "100";
				newOne.engType = "ERR4";
			}
			tokens.push_back(newOne);
		}//Checked
		else if (chars[i] == '(') {
			Token newOne;
			newOne.len = 1;
			newOne.pos = i;
			//newOne.type = "33(()";
			newOne.numType = "33";
			newOne.engType = "(";
			newOne.value = "[]";
			tokens.push_back(newOne);
		}//Checked
		else if (chars[i] == ')') {
			Token newOne;
			newOne.len = 1;
			newOne.pos = i;
			//newOne.type = "34())";
			newOne.numType = "34";
			newOne.engType = ")";
			newOne.value = "[]";
			tokens.push_back(newOne);
		}//Checked
		else if (chars[i] == '$' || chars[i] == '?' || chars[i] == '@' || chars[i] == '\\' || chars[i] == '^' || chars[i] == '`' || chars[i] == '~' || chars[i] == '\'') {
			Token newOne;
			newOne.pos = i;
			newOne.len = 1;
			//newOne.type = "100(ERR4)";
			newOne.numType = "100";
			newOne.engType = "ERR4";
			newOne.value = "[]";
			tokens.push_back(newOne);
		}//Checked
	}


	if (eofchar && len > 0) {
		cout << endl;
	}
/*	int printed = 0;
	for (size_t vecSize = 0; vecSize < tokens.size(); vecSize++) {
		if (argc == 1 || *argv[1] == '0') {
			printed++;
			cout << "TID: " << vecSize + 1 << ", TYPE: " << tokens[vecSize].numType << "(" << tokens[vecSize].engType << ")" << ", POS: " << tokens[vecSize].pos << ", LEN: " << tokens[vecSize].len;
			if (tokens[vecSize].value != "[]") {
				cout << ", VALUE: " << tokens[vecSize].value;
			}
			cout << endl;
		}
		else if (*argv[1] == '1' && tokens[vecSize].numType != "7" && tokens[vecSize].numType != "5" && tokens[vecSize].numType != "6" && tokens[vecSize].numType != "97" && tokens[vecSize].numType != "98" && tokens[vecSize].numType != "99" && tokens[vecSize].numType != "100") {
			printed++;
			cout << "TID: " << vecSize + 1 << ", TYPE: " << tokens[vecSize].numType << "(" << tokens[vecSize].engType << ")" << ", POS: " << tokens[vecSize].pos << ", LEN: " << tokens[vecSize].len;
			if (tokens[vecSize].value != "[]") {
				cout << ", VALUE: " << tokens[vecSize].value;
			}
			cout << endl;
		}
		else if (*argv[1] == '2' && (tokens[vecSize].numType == "7" || tokens[vecSize].numType == "5" || tokens[vecSize].numType == "6" || tokens[vecSize].numType == "97" || tokens[vecSize].numType == "98" || tokens[vecSize].numType == "99" || tokens[vecSize].numType == "100")) {
			printed++;
			cout << "TID: " << vecSize + 1 << ", TYPE: " << tokens[vecSize].numType << "(" << tokens[vecSize].engType << ")" << ", POS: " << tokens[vecSize].pos << ", LEN: " << tokens[vecSize].len;
			if (tokens[vecSize].value != "[]") {
			cout << ", VALUE: " << tokens[vecSize].value;
			}
			cout << endl;
		}
	}

	cout << endl;
	cout << "Totals: len = " << len << ", tokens = " << tokens.size() << ", printed = " << printed << endl;*/

	vector<Token> parseVec;
	bool unimplemented = false;
	int count = 1;
	for (Token parseInp : tokens) {
		if (parseInp.numType != "5" && parseInp.numType != "6" && parseInp.numType != "7") {
			if (parseInp.numType == "97" || parseInp.numType == "98" || parseInp.numType == "99" || parseInp.numType == "100") {
				cout << "Lex error" << endl;
				exit(0);
			}
			else if (parseInp.numType == "13" || parseInp.numType == "19" || parseInp.numType == "25") {
				unimplemented = true;
			}
			parseInp.tid = count;
			parseVec.push_back(parseInp);
		}
		count++;
	}

	if (unimplemented) {
		cout << "Unimplemented error" << endl;
		exit(0);
	}

	Node root;
	Node* parent = &root;
	root.stmtType = "stmt";
	int idx = 0;
	parseStmt(parseVec, idx, root, parent);

	if (idx < parseVec.size()){
		root.value = "error";
	}

	if (root.value == "error") {
		cout << "Parse error" << endl;
		exit(0);
	}

	displayTree(root);

}

void displayTree(const Node& root) {
	cout << "<node>";
	if (root.children.size() > 0) {
		cout << "<nt>" << root.stmtType << "</nt><children>" << endl;
		for (Node* n : root.children) {
			displayTree(*n);
		}
		cout << "</children></node>" << endl;
	}
	else {
		if (root.stmtType == "epsilon") {
			cout << "<epsilon></epsilon>";
		}
		else {
			cout << "<id>" << root.id << "</id><typenum>" << root.numType << "</typenum><typename>" << root.engType << "</typename><position>" << root.position << "</position><length>" << root.length << "</length>";
			if (root.value != "[]") {
				cout << "<value>" << root.value << "</value>";
			}
		}
		cout << "</node>" << endl;
	}
}

void parseStmt(vector<Token>& parseVec, int& index, Node& root, Node* parent) {
	if (index < parseVec.size()){
		if (parseVec[index].engType == "WHILE") {
			Node* newNode = new Node();
			newNode->stmtType = "while_stmt";
			parent->children.push_back(newNode);
			Node* temp = parent;
			parent = newNode;
			parseWhile(parseVec, index, root, parent);
			if (root.value == "error") {
				return;
			}
			parent = temp;
			Node* newNode1 = new Node();
			newNode1->stmtType = "stmt";
			parent->children.push_back(newNode1);
			parent = newNode1;
			parseStmt(parseVec, index, root, parent);
			if (root.value == "error") {
				return;
			}
			parent = temp;
		}
		else if (parseVec[index].engType == "FOR") {
			Node* newNode = new Node();
			newNode->stmtType = "for_stmt";
			parent->children.push_back(newNode);
			Node* temp = parent;
			parent = newNode;
			parseFor(parseVec, index, root, parent);
			if (root.value == "error") {
				return;
			}
			parent = temp;
			Node* newNode1 = new Node();
			newNode1->stmtType = "stmt";
			parent->children.push_back(newNode1);
			parent = newNode1;
			parseStmt(parseVec, index, root, parent);
			if (root.value == "error") {
				return;
			}
			parent = temp;
		}
		else if (parseVec[index].engType == "IF") {
			Node* newNode = new Node();
			newNode->stmtType = "if_stmt";
			parent->children.push_back(newNode);
			Node* temp = parent;
			parent = newNode;
			parseIf(parseVec, index, root, parent);
			if (root.value == "error") {
				return;
			}
			parent = temp;
			Node* newNode1 = new Node();
			newNode1->stmtType = "stmt";
			parent->children.push_back(newNode1);
			parent = newNode1;
			parseStmt(parseVec, index, root, parent);
			if (root.value == "error") {
				return;
			}
			parent = temp;
		}
		else if (parseVec[index].engType == "ID") {
			Node* newNode = new Node();
			newNode->stmtType = "assgn_stmt";
			parent->children.push_back(newNode);
			Node* temp = parent;
			parent = newNode;
			parseAssgn(parseVec, index, root, parent);
			if (root.value == "error") {
				return;
			}
			if (index >= parseVec.size()) {
				root.value = "error";
				return;
			}

			if (parseVec[index].engType != ";") {
				root.value = "error";
				return;
			}
			parent = temp;
			Node* newChild = new Node();
			newChild->id = parseVec[index].tid;
			newChild->length = parseVec[index].len;
			newChild->position = parseVec[index].pos;
			newChild->engType = parseVec[index].engType;
			newChild->numType = parseVec[index].numType;
			newChild->value = parseVec[index].value;
			parent->children.push_back(newChild);
			index++;
			Node* newNode1 = new Node();
			newNode1->stmtType = "stmt";
			parent->children.push_back(newNode1);
			parent = newNode1;
			parseStmt(parseVec, index, root, parent);
			if (root.value == "error") {
				return;
			}
			parent = temp;
		}
		else if (parseVec[index].engType == "[") {
			Node* newNode = new Node();
			newNode->stmtType = "output_stmt";
			parent->children.push_back(newNode);
			Node* temp = parent;
			parent = newNode;
			parseOutput(parseVec, index, root, parent);
			if (root.value == "error") {
				return;
			}
			parent = temp;
			Node* newNode1 = new Node();
			newNode1->stmtType = "stmt";
			parent->children.push_back(newNode1);
			parent = newNode1;
			parseStmt(parseVec, index, root, parent);
			if (root.value == "error") {
				return;
			}
			parent = temp;
		}
		else {
			Node* newNode = new Node();
			newNode->stmtType = "epsilon";
			parent->children.push_back(newNode);
		}
	}
	else {
		Node* newNode = new Node();
		newNode->stmtType = "epsilon";
		parent->children.push_back(newNode);
	}
		
}

void parseWhile(vector<Token>& parseVec, int& index, Node& root, Node* parent) {
	Node* newChild = new Node();
	newChild->id = parseVec[index].tid;
	newChild->length = parseVec[index].len;
	newChild->position = parseVec[index].pos;
	newChild->engType = parseVec[index].engType;
	newChild->numType = parseVec[index].numType;
	newChild->value = parseVec[index].value;
	parent->children.push_back(newChild);
	index++;
	if (index < parseVec.size() && parseVec[index].engType == "(") {
		Node* newNode1 = new Node();
		newNode1->id = parseVec[index].tid;
		newNode1->numType = parseVec[index].numType;
		newNode1->engType = parseVec[index].engType;
		newNode1->position = parseVec[index].pos;
		newNode1->length = parseVec[index].len;
		parent->children.push_back(newNode1);
		index++;
	}
	else {
		root.value = "error";
		return;
	}

	Node* temp1 = parent;
	parseCondExpr(parseVec, index, root, parent);
	if (root.value == "error") {
		return;
	}
	parent = temp1;

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}

	if (parseVec[index].engType != ")") {
		root.value = "error";
		return;
	}

	Node* newNode = new Node();
	newNode->id = parseVec[index].tid;
	newNode->length = parseVec[index].len;
	newNode->numType = parseVec[index].numType;
	newNode->engType = parseVec[index].engType;
	newNode->value = parseVec[index].value;
	newNode->position = parseVec[index].pos;
	parent->children.push_back(newNode);
	index++;

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}

	if (parseVec[index].engType != "{") {
		root.value = "error";
		return;
	}

	Node* newNode3 = new Node();
	newNode3->id = parseVec[index].tid;
	newNode3->length = parseVec[index].len;
	newNode3->numType = parseVec[index].numType;
	newNode3->engType = parseVec[index].engType;
	newNode3->value = parseVec[index].value;
	newNode3->position = parseVec[index].pos;
	parent->children.push_back(newNode3);
	index++;

	Node* newNode1 = new Node();
	newNode1->stmtType = "stmt";
	parent->children.push_back(newNode1);
	Node* temp = parent;
	parent = newNode1;
	parseStmt(parseVec, index, root, parent);

	if (root.value == "error") {
		return;
	}

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}

	if (parseVec[index].engType != "}") {
		root.value = "error";
		return;
	}
	parent = temp;
	Node* newNode4 = new Node();
	newNode4->id = parseVec[index].tid;
	newNode4->length = parseVec[index].len;
	newNode4->numType = parseVec[index].numType;
	newNode4->engType = parseVec[index].engType;
	newNode4->value = parseVec[index].value;
	newNode4->position = parseVec[index].pos;
	parent->children.push_back(newNode4);
	index++;
	
}

void parseFor(vector<Token>& parseVec, int& index, Node& root, Node* parent) {

	Node* newChild = new Node();
	newChild->id = parseVec[index].tid;
	newChild->length = parseVec[index].len;
	newChild->position = parseVec[index].pos;
	newChild->engType = parseVec[index].engType;
	newChild->numType = parseVec[index].numType;
	newChild->value = parseVec[index].value;
	parent->children.push_back(newChild);
	index++;
	if (index < parseVec.size() && parseVec[index].engType == "(") {
		Node* newNode1 = new Node();
		newNode1->id = parseVec[index].tid;
		newNode1->numType = parseVec[index].numType;
		newNode1->engType = parseVec[index].engType;
		newNode1->position = parseVec[index].pos;
		newNode1->length = parseVec[index].len;
		parent->children.push_back(newNode1);
		index++;
	}
	else {
		root.value = "error";
		return;
	}

	//Opt Assgn
	Node* newNode2 = new Node();
	newNode2->stmtType = "opt_assign";
	parent->children.push_back(newNode2);
	Node* temp1 = parent;
	parent = newNode2;

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}

	if (parseVec[index].engType == ";") {
		Node* newNode5 = new Node();
		newNode5->stmtType = "epsilon";
		parent->children.push_back(newNode5);
	}
	else {
		Node* newNode13 = new Node();
		newNode13->stmtType = "assgn_stmt";
		parent->children.push_back(newNode13);
		Node* temp10 = parent;
		parent = newNode13;
		parseAssgn(parseVec, index, root, parent);
		if (index >= parseVec.size() || parseVec[index].engType != ";") {
			root.value = "error";
			return;
		}
		parent = temp10;
	}

	if (root.value == "error") {
		return;
	}

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}

	parent = temp1;

	Node* newNode6 = new Node();
	newNode6->id = parseVec[index].tid;
	newNode6->length = parseVec[index].len;
	newNode6->numType = parseVec[index].numType;
	newNode6->engType = parseVec[index].engType;
	newNode6->value = parseVec[index].value;
	newNode6->position = parseVec[index].pos;
	parent->children.push_back(newNode6);
	index++;

	//Opt Cond
	Node* newNode7 = new Node();
	newNode7->stmtType = "opt_cond";
	parent->children.push_back(newNode7);
	Node* temp2 = parent;
	parent = newNode7;

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}

	if (parseVec[index].engType == ";") {
		Node* newNode8 = new Node();
		newNode8->stmtType = "epsilon";
		parent->children.push_back(newNode8);
	}
	else {
		parseCondExpr(parseVec, index, root, parent);
		if (index >= parseVec.size() || parseVec[index].engType != ";") {
			root.value = "error";
			return;
		}
	}

	if (root.value == "error") {
		return;
	}

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}

	parent = temp2;

	Node* newNode9 = new Node();
	newNode9->id = parseVec[index].tid;
	newNode9->length = parseVec[index].len;
	newNode9->numType = parseVec[index].numType;
	newNode9->engType = parseVec[index].engType;
	newNode9->value = parseVec[index].value;
	newNode9->position = parseVec[index].pos;
	parent->children.push_back(newNode9);
	index++;
	
	if (root.value == "error") {
		return;
	}
	
	//Opt Assgn
	Node* newNode10 = new Node();
	newNode10->stmtType = "opt_assign";
	parent->children.push_back(newNode10);
	Node* temp3 = parent;
	parent = newNode10;

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}

	if (parseVec[index].engType == ")") {
		Node* newNode11 = new Node();
		newNode11->stmtType = "epsilon";
		parent->children.push_back(newNode11);
	}
	else {
		Node* newNode14 = new Node();
		newNode14->stmtType = "assgn_stmt";
		parent->children.push_back(newNode14);
		Node* temp9 = parent;
		parent = newNode14;
		parseAssgn(parseVec, index, root, parent);
		if (index >= parseVec.size() || parseVec[index].engType != ")") {
			root.value = "error";
			return;
		}
		parent = temp9;
	}

	if (root.value == "error") {
		return;
	}

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}

	parent = temp3;

	Node* newNode12 = new Node();
	newNode12->id = parseVec[index].tid;
	newNode12->length = parseVec[index].len;
	newNode12->numType = parseVec[index].numType;
	newNode12->engType = parseVec[index].engType;
	newNode12->value = parseVec[index].value;
	newNode12->position = parseVec[index].pos;
	parent->children.push_back(newNode12);
	index++;

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}

	if (parseVec[index].engType != "{") {
		root.value = "error";
		return;
	}

	Node* newNode3 = new Node();
	newNode3->id = parseVec[index].tid;
	newNode3->length = parseVec[index].len;
	newNode3->numType = parseVec[index].numType;
	newNode3->engType = parseVec[index].engType;
	newNode3->value = parseVec[index].value;
	newNode3->position = parseVec[index].pos;
	parent->children.push_back(newNode3);
	index++;

	Node* newNode1 = new Node();
	newNode1->stmtType = "stmt";
	parent->children.push_back(newNode1);
	Node* temp4 = parent;
	parent = newNode1;
	parseStmt(parseVec, index, root, parent);

	if (root.value == "error") {
		return;
	}

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}

	if (parseVec[index].engType != "}") {
		root.value = "error";
		return;
	}

	parent = temp4;

	Node* newNode4 = new Node();
	newNode4->id = parseVec[index].tid;
	newNode4->length = parseVec[index].len;
	newNode4->numType = parseVec[index].numType;
	newNode4->engType = parseVec[index].engType;
	newNode4->value = parseVec[index].value;
	newNode4->position = parseVec[index].pos;
	parent->children.push_back(newNode4);
	index++;

}

void parseIf(vector<Token>& parseVec, int& index, Node& root, Node* parent) {
	Node* newChild = new Node();
	newChild->id = parseVec[index].tid;
	newChild->length = parseVec[index].len;
	newChild->position = parseVec[index].pos;
	newChild->engType = parseVec[index].engType;
	newChild->numType = parseVec[index].numType;
	newChild->value = parseVec[index].value;
	parent->children.push_back(newChild);
	index++;
	if (index < parseVec.size() && parseVec[index].engType == "(") {
		Node* newNode1 = new Node();
		newNode1->id = parseVec[index].tid;
		newNode1->numType = parseVec[index].numType;
		newNode1->engType = parseVec[index].engType;
		newNode1->position = parseVec[index].pos;
		newNode1->length = parseVec[index].len;
		parent->children.push_back(newNode1);
		index++;
	}
	else {
		root.value = "error";
		return;
	}

	Node* temp1 = parent;
	parseCondExpr(parseVec, index, root, parent);
	if (root.value == "error") {
		return;
	}
	parent = temp1;

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}

	if (parseVec[index].engType != ")") {
		root.value = "error";
		return;
	}

	Node* newNode = new Node();
	newNode->id = parseVec[index].tid;
	newNode->length = parseVec[index].len;
	newNode->numType = parseVec[index].numType;
	newNode->engType = parseVec[index].engType;
	newNode->value = parseVec[index].value;
	newNode->position = parseVec[index].pos;
	parent->children.push_back(newNode);
	index++;

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}

	if (parseVec[index].engType != "{") {
		root.value = "error";
		return;
	}

	Node* newNode3 = new Node();
	newNode3->id = parseVec[index].tid;
	newNode3->length = parseVec[index].len;
	newNode3->numType = parseVec[index].numType;
	newNode3->engType = parseVec[index].engType;
	newNode3->value = parseVec[index].value;
	newNode3->position = parseVec[index].pos;
	parent->children.push_back(newNode3);
	index++;

	Node* newNode1 = new Node();
	newNode1->stmtType = "stmt";
	parent->children.push_back(newNode1);
	Node* temp = parent;
	parent = newNode1;
	parseStmt(parseVec, index, root, parent);

	if (root.value == "error") {
		return;
	}

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}

	if (parseVec[index].engType != "}") {
		root.value = "error";
		return;
	}
	parent = temp;
	Node* newNode4 = new Node();
	newNode4->id = parseVec[index].tid;
	newNode4->length = parseVec[index].len;
	newNode4->numType = parseVec[index].numType;
	newNode4->engType = parseVec[index].engType;
	newNode4->value = parseVec[index].value;
	newNode4->position = parseVec[index].pos;
	parent->children.push_back(newNode4);
	index++;

	parseOptElse(parseVec, index, root, parent);

	if (root.value == "error") {
		return;
	}
	parent = temp;

}

void parseOptElse(vector<Token>& parseVec, int& index, Node& root, Node* parent) {
	if (index < parseVec.size()) {
		Node* newNode = new Node();
		newNode->stmtType = "opt_else";
		parent->children.push_back(newNode);
		Node* temp = parent;
		parent = newNode;
		if (parseVec[index].engType == "ELSE") {

			Node* newNode1 = new Node();
			newNode1->id = parseVec[index].tid;
			newNode1->length = parseVec[index].len;
			newNode1->numType = parseVec[index].numType;
			newNode1->engType = parseVec[index].engType;
			newNode1->value = parseVec[index].value;
			newNode1->position = parseVec[index].pos;
			parent->children.push_back(newNode1);
			index++;

			if (index >= parseVec.size()) {
				root.value = "error";
				return;
			}

			if (parseVec[index].engType != "{") {
				root.value = "error";
				return;
			}

			Node* newNode3 = new Node();
			newNode3->id = parseVec[index].tid;
			newNode3->length = parseVec[index].len;
			newNode3->numType = parseVec[index].numType;
			newNode3->engType = parseVec[index].engType;
			newNode3->value = parseVec[index].value;
			newNode3->position = parseVec[index].pos;
			parent->children.push_back(newNode3);
			index++;

			Node* newNode4 = new Node();
			newNode4->stmtType = "stmt";
			parent->children.push_back(newNode4);
			Node* temp1 = parent;
			parent = newNode4;
			parseStmt(parseVec, index, root, parent);

			if (root.value == "error") {
				return;
			}

			if (index >= parseVec.size()) {
				root.value = "error";
				return;
			}

			if (parseVec[index].engType != "}") {
				root.value = "error";
				return;
			}
			parent = temp1;
			Node* newNode2 = new Node();
			newNode2->id = parseVec[index].tid;
			newNode2->length = parseVec[index].len;
			newNode2->numType = parseVec[index].numType;
			newNode2->engType = parseVec[index].engType;
			newNode2->value = parseVec[index].value;
			newNode2->position = parseVec[index].pos;
			parent->children.push_back(newNode2);
			index++;

		}
		else {
			Node* newNode1 = new Node();
			newNode1->stmtType = "epsilon";
			parent->children.push_back(newNode1);
		}
		parent = temp;
	}
	else {
		Node* newNode = new Node();
		newNode->stmtType = "opt_else";
		parent->children.push_back(newNode);
		Node* temp = parent;
		parent = newNode;
		Node* newNode1 = new Node();
		newNode1->stmtType = "epsilon";
		parent->children.push_back(newNode1);
		parent = temp;
	}
}

void parseAssgn(vector<Token>& parseVec, int& index, Node& root, Node* parent) {

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}

	if (parseVec[index].engType == "ID") {
		Node* newNode2 = new Node();
		newNode2->id = parseVec[index].tid;
		newNode2->length = parseVec[index].len;
		newNode2->numType = parseVec[index].numType;
		newNode2->engType = parseVec[index].engType;
		newNode2->value = parseVec[index].value;
		newNode2->position = parseVec[index].pos;
		parent->children.push_back(newNode2);
		index++;
	}
	else {
		root.value = "error";
		return;
	}

	if (parseVec[index].engType == ":=") {
		Node* newNode2 = new Node();
		newNode2->id = parseVec[index].tid;
		newNode2->length = parseVec[index].len;
		newNode2->numType = parseVec[index].numType;
		newNode2->engType = parseVec[index].engType;
		newNode2->value = parseVec[index].value;
		newNode2->position = parseVec[index].pos;
		parent->children.push_back(newNode2);
		index++;
	}
	else {
		root.value = "error";
		return;
	}
	Node* temp = parent;
	parseExpr(parseVec, index, root, parent);
	parent = temp;
	if (root.value == "error") {
		return;
	}

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}

}

void parseOutput(vector<Token>& parseVec, int& index, Node& root, Node* parent) {

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}

	if (parseVec[index].engType == "[") {
		Node* newNode2 = new Node();
		newNode2->id = parseVec[index].tid;
		newNode2->length = parseVec[index].len;
		newNode2->numType = parseVec[index].numType;
		newNode2->engType = parseVec[index].engType;
		newNode2->value = parseVec[index].value;
		newNode2->position = parseVec[index].pos;
		parent->children.push_back(newNode2);
		index++;
	}
	else {
		root.value = "error";
		return;
	}
	Node* temp = parent;
	parseExpr(parseVec, index, root, parent);
	parent = temp;
	if (root.value == "error") {
		return;
	}

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}

	if (parseVec[index].engType == "]") {
		Node* newNode2 = new Node();
		newNode2->id = parseVec[index].tid;
		newNode2->length = parseVec[index].len;
		newNode2->numType = parseVec[index].numType;
		newNode2->engType = parseVec[index].engType;
		newNode2->value = parseVec[index].value;
		newNode2->position = parseVec[index].pos;
		parent->children.push_back(newNode2);
		index++;
	}
	else {
		root.value = "error";
		return;
	}

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}

	if (parseVec[index].engType == ";") {
		Node* newNode2 = new Node();
		newNode2->id = parseVec[index].tid;
		newNode2->length = parseVec[index].len;
		newNode2->numType = parseVec[index].numType;
		newNode2->engType = parseVec[index].engType;
		newNode2->value = parseVec[index].value;
		newNode2->position = parseVec[index].pos;
		parent->children.push_back(newNode2);
		index++;
	}
	else {
		root.value = "error";
		return;
	}

}

void parseCondExpr(vector<Token>& parseVec, int& index, Node& root, Node* parent) {

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}
	else {
		Node* newNode2 = new Node();
		newNode2->stmtType = "cond_expr";
		parent->children.push_back(newNode2);
		Node* temp = parent;
		parent = newNode2;
		parseAndExpr(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
		parent = newNode2;
		parseCondExprR(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
		parent = temp;
	}

}

void parseCondExprR(vector<Token>& parseVec, int& index, Node& root, Node* parent) {

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}
	Node* newNode2 = new Node();
	newNode2->stmtType = "cond_expr_r";
	parent->children.push_back(newNode2);
	Node* temp = parent;
	parent = newNode2;
	if (parseVec[index].engType == "|") {
		Node* newNode1 = new Node();
		newNode1->id = parseVec[index].tid;
		newNode1->length = parseVec[index].len;
		newNode1->numType = parseVec[index].numType;
		newNode1->engType = parseVec[index].engType;
		newNode1->value = parseVec[index].value;
		newNode1->position = parseVec[index].pos;
		parent->children.push_back(newNode1);
		index++;
		parseAndExpr(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
		parent = newNode2;
		parseCondExprR(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
	}
	else {
		Node* newNode = new Node();
		newNode->stmtType = "epsilon";
		parent->children.push_back(newNode);
	}
	parent = temp;

}

void parseAndExpr(vector<Token>& parseVec, int& index, Node& root, Node* parent) {

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}
	else {
		Node* newNode2 = new Node();
		newNode2->stmtType = "and_expr";
		parent->children.push_back(newNode2);
		Node* temp = parent;
		parent = newNode2;
		parseEqExpr(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
		parent = newNode2;
		parseAndExprR(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
		parent = temp;
	}
	
}

void parseAndExprR(vector<Token>& parseVec, int& index, Node& root, Node* parent) {
	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}
	Node* newNode2 = new Node();
	newNode2->stmtType = "and_expr_r";
	parent->children.push_back(newNode2);
	Node* temp = parent;
	parent = newNode2;
	if (parseVec[index].engType == "&") {
		Node* newNode1 = new Node();
		newNode1->id = parseVec[index].tid;
		newNode1->length = parseVec[index].len;
		newNode1->numType = parseVec[index].numType;
		newNode1->engType = parseVec[index].engType;
		newNode1->value = parseVec[index].value;
		newNode1->position = parseVec[index].pos;
		parent->children.push_back(newNode1);
		index++;
		parseCmpExpr(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
		parent = newNode2;
		parseAndExprR(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
	}
	else {
		Node* newNode = new Node();
		newNode->stmtType = "epsilon";
		parent->children.push_back(newNode);
	}
	parent = temp;
}

void parseEqExpr(vector<Token>& parseVec, int& index, Node& root, Node* parent) {

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}
	else {
		Node* newNode2 = new Node();
		newNode2->stmtType = "eq_expr";
		parent->children.push_back(newNode2);
		Node* temp = parent;
		parent = newNode2;
		parseCmpExpr(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
		parent = newNode2;
		parseEqExprR(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
		parent = temp;
	}

}

void parseEqExprR(vector<Token>& parseVec, int& index, Node& root, Node* parent) {
	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}
	Node* newNode2 = new Node();
	newNode2->stmtType = "eq_expr_r";
	parent->children.push_back(newNode2);
	Node* temp = parent;
	parent = newNode2;
	if (parseVec[index].engType == "=" || parseVec[index].engType == "!=") {
		Node* newNode1 = new Node();
		newNode1->id = parseVec[index].tid;
		newNode1->length = parseVec[index].len;
		newNode1->numType = parseVec[index].numType;
		newNode1->engType = parseVec[index].engType;
		newNode1->value = parseVec[index].value;
		newNode1->position = parseVec[index].pos;
		parent->children.push_back(newNode1);
		index++;
		parseCmpExpr(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
		parent = newNode2;
		parseEqExprR(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
	}
	else {
		Node* newNode = new Node();
		newNode->stmtType = "epsilon";
		parent->children.push_back(newNode);
	}
	parent = temp;
}

void parseCmpExpr(vector<Token>& parseVec, int& index, Node& root, Node* parent) {

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}
	else {
		Node* newNode2 = new Node();
		newNode2->stmtType = "cmp_expr";
		parent->children.push_back(newNode2);
		Node* temp = parent;
		parent = newNode2;
		parseExpr(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
		parent = newNode2;
		parseCmpExprR(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
		parent = temp;
	}

}

void parseCmpExprR(vector<Token>& parseVec, int& index, Node& root, Node* parent) {

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}
	Node* newNode2 = new Node();
	newNode2->stmtType = "cmp_expr_r";
	parent->children.push_back(newNode2);
	Node* temp = parent;
	parent = newNode2;
	if (parseVec[index].engType == "<" || parseVec[index].engType == ">" || parseVec[index].engType == "<=" || parseVec[index].engType == ">=") {
		Node* newNode1 = new Node();
		newNode1->id = parseVec[index].tid;
		newNode1->length = parseVec[index].len;
		newNode1->numType = parseVec[index].numType;
		newNode1->engType = parseVec[index].engType;
		newNode1->value = parseVec[index].value;
		newNode1->position = parseVec[index].pos;
		parent->children.push_back(newNode1);
		Node* temp1 = parent;
		index++;
		parseExpr(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
		parent = temp1;
		parseCmpExprR(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
	}
	else {
		Node* newNode = new Node();
		newNode->stmtType = "epsilon";
		parent->children.push_back(newNode);
	}
	parent = temp;
}

void parseExpr(vector<Token>& parseVec, int& index, Node& root, Node* parent) {

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}
	else {
		Node* newNode2 = new Node();
		newNode2->stmtType = "expr";
		parent->children.push_back(newNode2);
		Node* temp = parent;
		parent = newNode2;
		parseMultExpr(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
		parent = newNode2;
		parseExprR(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
		parent = temp;
	}

}

void parseExprR(vector<Token>& parseVec, int& index, Node& root, Node* parent) {
	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}
	Node* newNode2 = new Node();
	newNode2->stmtType = "expr_r";
	parent->children.push_back(newNode2);
	Node* temp = parent;
	parent = newNode2;
	if (parseVec[index].engType == "+") {
		Node* newNode1 = new Node();
		newNode1->id = parseVec[index].tid;
		newNode1->length = parseVec[index].len;
		newNode1->numType = parseVec[index].numType;
		newNode1->engType = parseVec[index].engType;
		newNode1->value = parseVec[index].value;
		newNode1->position = parseVec[index].pos;
		parent->children.push_back(newNode1);
		index++;
		Node* temp1 = parent;
		parseMultExpr(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
		parent = temp1;
		parseExprR(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
	}
	else {
		Node* newNode = new Node();
		newNode->stmtType = "epsilon";
		parent->children.push_back(newNode);
	}
	parent = temp;
}

void parseMultExpr(vector<Token>& parseVec, int& index, Node& root, Node* parent) {

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}
	else {
		Node* newNode2 = new Node();
		newNode2->stmtType = "mult_expr";
		parent->children.push_back(newNode2);
		Node* temp = parent;
		parent = newNode2;
		parseUnaryExpr(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
		parent = newNode2;
		parseMultExprR(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
		parent = temp;
	}

}

void parseMultExprR(vector<Token>& parseVec, int& index, Node& root, Node* parent) {
	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}
	Node* newNode2 = new Node();
	newNode2->stmtType = "mult_expr_r";
	parent->children.push_back(newNode2);
	Node* temp = parent;
	parent = newNode2;
	if (parseVec[index].engType == "*") {
		Node* newNode1 = new Node();
		newNode1->id = parseVec[index].tid;
		newNode1->length = parseVec[index].len;
		newNode1->numType = parseVec[index].numType;
		newNode1->engType = parseVec[index].engType;
		newNode1->value = parseVec[index].value;
		newNode1->position = parseVec[index].pos;
		parent->children.push_back(newNode1);
		Node* temp1 = parent;
		index++;
		parseUnaryExpr(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
		parent = temp1;
		parseMultExprR(parseVec, index, root, parent);
		if (root.value == "error") {
			return;
		}
	}
	else {
		Node* newNode = new Node();
		newNode->stmtType = "epsilon";
		parent->children.push_back(newNode);
	}
	parent = temp;
}

void parseUnaryExpr(vector<Token>& parseVec, int& index, Node& root, Node* parent) {
	
	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}
	else {
		Node* newNode2 = new Node();
		newNode2->stmtType = "unary_expr";
		parent->children.push_back(newNode2);
		Node* temp = parent;
		parent = newNode2;
		if (parseVec[index].engType == "!" || parseVec[index].engType == "+" || parseVec[index].engType == "-") {
			Node* newNode1 = new Node();
			newNode1->id = parseVec[index].tid;
			newNode1->length = parseVec[index].len;
			newNode1->numType = parseVec[index].numType;
			newNode1->engType = parseVec[index].engType;
			newNode1->value = parseVec[index].value;
			newNode1->position = parseVec[index].pos;
			parent->children.push_back(newNode1);
			index++;
			parseExpr(parseVec, index, root, parent);
		}
		else {
			parseParenExpr(parseVec, index, root, parent);
		}
		if (root.value == "error") {
			return;
		}
		parent = temp;
	}
}

void parseParenExpr(vector<Token>& parseVec, int& index, Node& root, Node* parent) {

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}
	else {
		Node* newNode2 = new Node();
		newNode2->stmtType = "paren_expr";
		parent->children.push_back(newNode2);
		Node* temp1 = parent;
		parent = newNode2;
		if (parseVec[index].engType == "(") {
			Node* newNode1 = new Node();
			newNode1->id = parseVec[index].tid;
			newNode1->length = parseVec[index].len;
			newNode1->numType = parseVec[index].numType;
			newNode1->engType = parseVec[index].engType;
			newNode1->value = parseVec[index].value;
			newNode1->position = parseVec[index].pos;
			parent->children.push_back(newNode1);
			index++;
			parseExpr(parseVec, index, root, parent);
			if (index >= parseVec.size()) {
				root.value = "error";
				return;
			}
			if (parseVec[index].engType == ")") {
				Node* newNode = new Node();
				newNode->id = parseVec[index].tid;
				newNode->length = parseVec[index].len;
				newNode->numType = parseVec[index].numType;
				newNode->engType = parseVec[index].engType;
				newNode->value = parseVec[index].value;
				newNode->position = parseVec[index].pos;
				parent->children.push_back(newNode);
				index++;
			}
			else {
				root.value = "error";
				return;
			}
		}
		else {
			Node* newNode4 = new Node();
			newNode4->stmtType = "basic_expr";
			parent->children.push_back(newNode4);
			Node* temp = parent;
			parent = newNode4;
			parseBasicExpr(parseVec, index, root, parent);
			parent = temp;
		}
		parent = temp1;
		if (root.value == "error") {
			return;
		}
	}

}

void parseBasicExpr(vector<Token>& parseVec, int& index, Node& root, Node* parent) {

	if (index >= parseVec.size()) {
		root.value = "error";
		return;
	}
	else {
		if (parseVec[index].engType == "ID" || parseVec[index].engType == "STR" || parseVec[index].engType == "INT" || parseVec[index].engType == "FLOAT") {
			Node* newNode1 = new Node();
			newNode1->id = parseVec[index].tid;
			newNode1->length = parseVec[index].len;
			newNode1->numType = parseVec[index].numType;
			newNode1->engType = parseVec[index].engType;
			newNode1->value = parseVec[index].value;
			newNode1->position = parseVec[index].pos;
			parent->children.push_back(newNode1);
			index++;
		}
		else {
			root.value = "error";
			return;
		}
	}

}
