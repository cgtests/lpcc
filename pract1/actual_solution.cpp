#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

class Symbol {
public:
    std::string sym;
    int index;
    int address;

    Symbol(const std::string& s, int i, int a)
        : sym(s), index(i), address(a) {}
};

bool isMnemonic(const std::string& token) {
    static const std::vector<std::string> mnemonics = {
        "START", "END", "ORIGIN", "EQU", "LTORG",
        "LOAD", "ADD", "MULT", "DS", "DC", "SUB",
        "MOVER", "MOVEM", "COMP", "BC", "DIV",
        "READ", "PRINT", "STOP"
    };
    for (const auto& m : mnemonics) {
        if (token == m) return true;
    }
    return false;
}

bool isSymbol(const std::string& token) {
    if (isMnemonic(token)) return false;
    for (char ch : token) {
        if (std::isdigit(ch)) return false;
    }
    return true;
}

bool symbolExists(const std::string& name, const std::vector<Symbol>& table) {
    for (const auto& sym : table) {
        if (sym.sym == name) return true;
    }
    return false;
}

void assignAddresses(std::vector<Symbol>& symbolTable, int& LC) {
    for (auto& sym : symbolTable) {
        if (sym.address == -1) {
            sym.address = LC++;
        }
    }
}

void processLine(const std::string& line, std::vector<Symbol>& symbolTable, int& LC, int& index) {
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> tokens;
    while (ss >> token) {
        tokens.push_back(token);
    }

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "START" && i + 1 < tokens.size()) {
            LC = std::stoi(tokens[i + 1]);
            break;
        }
        else if (isSymbol(tokens[i]) && !symbolExists(tokens[i], symbolTable)) {
            symbolTable.emplace_back(tokens[i], index++, -1);
        }
    }

    LC++; // Increment LC for each instruction line
}

int main() {
    std::ifstream inputFile("SAMPLE_input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open input file.\n";
        return 1;
    }

    std::vector<Symbol> symbolTable;
    std::string line;
    int LC = 0;
    int index = 0;

    while (std::getline(inputFile, line)) {
        if (!line.empty())
            processLine(line, symbolTable, LC, index);
    }
    inputFile.close();

    assignAddresses(symbolTable, LC);

    std::cout << "SYMBOL TABLE\n";
    std::cout << "Index\tSymbol\tAddress\n";
    for (const auto& sym : symbolTable) {
        std::cout << sym.index << "\t" << sym.sym << "\t" << sym.address << "\n";
    }

    return 0;
}
