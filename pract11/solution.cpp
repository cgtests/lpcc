#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <iomanip>

using namespace std;

struct Symbol {
    string name;
    int address;
};

int main() {
    ifstream infile("input.asm");
    if (!infile) {
        cerr << "Error opening input file.\n";
        return 1;
    }

    map<string, string> opcodeTable = {
        {"READ", "09"},
        {"MOVER", "04"},
        {"SUB", "05"},
        {"STOP", "00"},
        {"START", "01"},
        {"END", "02"},
        {"DS", "01"}
    };

    map<string, string> classTable = {
        {"READ", "IS"},
        {"MOVER", "IS"},
        {"SUB", "IS"},
        {"STOP", "IS"},
        {"START", "AD"},
        {"END", "AD"},
        {"DS", "DL"}
    };

    map<string, int> registerTable = {
        {"AREG", 1},
        {"BREG", 2},
        {"CREG", 3}
    };

    vector<Symbol> symbolTable;
    map<string, int> symIndex; // name to index
    vector<string> intermediateCode;

    string line;
    int LC = 0;

    while (getline(infile, line)) {
        stringstream ss(line);
        vector<string> tokens;
        string word;

        while (ss >> word)
            tokens.push_back(word);

        if (tokens.empty())
            continue;

        // START
        if (tokens[0] == "START") {
            LC = stoi(tokens[1]);
            intermediateCode.push_back("(AD,01) (C," + to_string(LC) + ")");
        }

        // END
        else if (tokens[0] == "END") {
            intermediateCode.push_back("(AD,02)");
        }

        // Declarative Statement (e.g., A DS 1)
        else if (tokens.size() == 3 && tokens[1] == "DS") {
            symIndex[tokens[0]] = symbolTable.size();
            symbolTable.push_back({tokens[0], LC});
            intermediateCode.push_back("(DL,01) (C," + tokens[2] + ")");
            LC++;
        }

        // Instructions
        else {
            string opcode, reg = "", operand = "";
            int i = 0;

            // Check if it has label
            if (opcodeTable.find(tokens[0]) == opcodeTable.end()) {
                // It's a label
                i = 1;
            }

            opcode = tokens[i];

            if (tokens.size() > i + 1)
                reg = tokens[i + 1];

            if (tokens.size() > i + 2)
                operand = tokens[i + 2];

            string ic = "(" + classTable[opcode] + "," + opcodeTable[opcode] + ") ";

            if (!reg.empty()) {
                // Remove comma if exists
                if (reg.back() == ',')
                    reg.pop_back();

                ic += "(" + to_string(registerTable[reg]) + ") ";
            }

            if (!operand.empty()) {
                if (symIndex.find(operand) == symIndex.end()) {
                    // Add symbol if not found
                    symIndex[operand] = symbolTable.size();
                    symbolTable.push_back({operand, -1});
                }
                ic += "(S," + to_string(symIndex[operand]) + ")";
            }

            intermediateCode.push_back(ic);
            LC++;
        }
    }

    infile.close();

    // Output Intermediate Code
    cout << "Intermediate Code:\n";
    for (const string &line : intermediateCode)
        cout << line << endl;

    // Output Symbol Table
    cout << "\nSymbol Table:\n";
    cout << setw(10) << "Index" << setw(10) << "Symbol" << setw(10) << "Address" << endl;
    for (size_t i = 0; i < symbolTable.size(); ++i) {
        cout << setw(10) << i << setw(10) << symbolTable[i].name << setw(10) << symbolTable[i].address << endl;
    }

    return 0;
}
