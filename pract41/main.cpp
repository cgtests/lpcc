#include <iostream>
#include <stack>
#include <vector>
#include <sstream>
#include <cctype>
#include <map>
#include <regex>
using namespace std;

// Operator precedence
map<char, int> precedence = {
    {'^', 3},
    {'*', 2}, {'/', 2},
    {'+', 1}, {'-', 1}
};

// Check if a character is an operator
bool isOperator(char c) {
    return precedence.count(c);
}

// Split string into tokens (variables, operators, parentheses)
vector<string> tokenize(const string& expr) {
    vector<string> tokens;
    regex token_re(R"([a-zA-Z_]\w*|\d+|[+*/^()-])");
    auto words_begin = sregex_iterator(expr.begin(), expr.end(), token_re);
    auto words_end = sregex_iterator();
    for (auto it = words_begin; it != words_end; ++it)
        tokens.push_back(it->str());
    return tokens;
}

// Convert infix to postfix (Shunting Yard Algorithm)
vector<string> infixToPostfix(const string& expr) {
    stack<string> opStack;
    vector<string> postfix;
    vector<string> tokens = tokenize(expr);

    for (string token : tokens) {
        if (isalnum(token[0])) {
            postfix.push_back(token);
        }
        else if (token == "(") {
            opStack.push(token);
        }
        else if (token == ")") {
            while (!opStack.empty() && opStack.top() != "(") {
                postfix.push_back(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop(); // pop "("
        }
        else if (isOperator(token[0])) {
            while (!opStack.empty() && opStack.top() != "(" &&
                   precedence[token[0]] <= precedence[opStack.top()[0]]) {
                postfix.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(token);
        }
    }

    while (!opStack.empty()) {
        postfix.push_back(opStack.top());
        opStack.pop();
    }

    return postfix;
}

// Generate Three Address Code
void generateTAC(const vector<string>& postfix, const string& lhs) {
    stack<string> evalStack;
    int tempCount = 1;
    vector<string> code;

    for (string token : postfix) {
        if (isalnum(token[0])) {
            evalStack.push(token);
        }
        else if (isOperator(token[0])) {
            string op2 = evalStack.top(); evalStack.pop();
            string op1 = evalStack.top(); evalStack.pop();
            string temp = "t" + to_string(tempCount++);
            cout << temp << " = " << op1 << " " << token << " " << op2 << endl;
            evalStack.push(temp);
        }
    }

    cout << lhs << " = " << evalStack.top() << endl;
}

// Main function
int main() {
    string input;
    cout << "Enter an expression (e.g., a = b*b + c*c): ";
    getline(cin, input);

    size_t eq = input.find('=');
    if (eq == string::npos) {
        cout << "Invalid expression format. Use: variable = expression" << endl;
        return 1;
    }

    string lhs = input.substr(0, eq);
    string rhs = input.substr(eq + 1);

    lhs.erase(remove(lhs.begin(), lhs.end(), ' '), lhs.end());
    vector<string> postfix = infixToPostfix(rhs);
    
    cout << "\nThree Address Code:\n";
    generateTAC(postfix, lhs);

    return 0;
}
