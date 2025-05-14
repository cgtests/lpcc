#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

class Literal {
public:
    std::string lit;
    int ind;
    int add;
    Literal(std::string lit, int ind, int add) : lit(lit), ind(ind), add(add) {}
};

bool isLiteral(const std::string &token) {
    return token.substr(0, 2) == "='";
}

bool literalExists(const std::string &lit, const std::vector<Literal> &table) {
    return std::any_of(table.begin(), table.end(), [&](const Literal &l) { return l.lit == lit; });
}

void assignAddresses(std::vector<Literal> &literalTable, int &LC) {
    for (auto &lit : literalTable) {
        if (lit.add == -1) {
            lit.add = LC++;
        }
    }
}

void makeTokens(const std::string &line, std::vector<std::string> &tokens) {
    std::stringstream ss(line);
    std::string tok;
    while (ss >> tok)
        tokens.push_back(tok);
}

bool isPseudo(const std::string &tok) {
    return tok == "START" || tok == "END" || tok == "LTORG";
}

bool isStorageDirective(const std::string &tok) {
    return tok == "DS" || tok == "DC";
}

bool isInstruction(const std::string &tok) {
    static std::vector<std::string> ops = {
        "ADD", "SUB", "MULT", "MOVER", "MOVEM", "COMP", "BC", "DIV", "READ", "PRINT", "LOAD", "STOP"
    };
    return std::find(ops.begin(), ops.end(), tok) != ops.end();
}

int main() {
    std::ifstream inFile("SAMPLE_input.txt");
    if (!inFile) {
        std::cerr << "File open error.\n";
        return 1;
    }

    std::vector<Literal> literalTable;
    std::string line;
    int LC = 0, litIndex = 0;
    bool startFound = false;

    while (std::getline(inFile, line)) {
        std::vector<std::string> tokens;
        makeTokens(line, tokens);
        if (tokens.empty()) continue;

        // START Directive
        if (tokens[0] == "START" && tokens.size() > 1 && !startFound) {
            LC = std::stoi(tokens[1]);
            startFound = true;
            continue;
        }

        // Assign literals at LTORG or END
        if (tokens[0] == "LTORG" || tokens[0] == "END") {
            assignAddresses(literalTable, LC);
            continue;
        }

        // Label present
        int tokIndex = (isInstruction(tokens[0]) || isPseudo(tokens[0]) || isStorageDirective(tokens[0])) ? 0 : 1;

        // Instructions or Storage directive
        if (tokIndex < tokens.size()) {
            std::string op = tokens[tokIndex];
            if (isInstruction(op) || isStorageDirective(op)) {
                LC += 1;
            }
        }

        // Add any literal
        for (const auto &tok : tokens) {
            if (isLiteral(tok) && !literalExists(tok, literalTable)) {
                literalTable.emplace_back(tok, litIndex++, -1);
            }
        }
    }

    inFile.close();

    std::cout << "LITERAL TABLE (with addresses):\n";
    std::cout << "Index\tLiteral\tAddress\n";
    for (const auto &lit : literalTable) {
        std::cout << lit.ind << "\t" << lit.lit << "\t" << lit.add << '\n';
    }

    return 0;
}
