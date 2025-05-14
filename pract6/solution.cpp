#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

class Literal {
public:
    std::string lit{};
    int ind{};
    int add{};

    Literal(std::string lit, int ind, int add)
        : lit(lit), ind(ind), add(add) {}
};

bool isLiteral(const std::string &instruction) {
    return instruction.substr(0, 2) == "='";
}

void assignAddresses(std::vector<Literal> &literalTable, int &LC) {
    for (auto &lit : literalTable) {
        if (lit.add == -1) {
            lit.add = LC++;
        }
    }
}

bool literalExists(const std::string &lit, const std::vector<Literal> &literalTable) {
    for (const auto &l : literalTable) {
        if (l.lit == lit)
            return true;
    }
    return false;
}

void makeInstrArray(const std::string &line, std::vector<std::string> &tokens) {
    std::stringstream ss(line);
    std::string word;
    while (ss >> word)
        tokens.push_back(word);
}

int main() {
    std::ifstream inFile("SAMPLE_input.txt");
    if (!inFile) {
        std::cerr << "Could not open SAMPLE_input.txt\n";
        return 1;
    }

    std::vector<Literal> literalTable;
    std::string line;
    int LC = 0;
    int litIndex = 0;

    while (std::getline(inFile, line)) {
        std::vector<std::string> tokens;
        makeInstrArray(line, tokens);
        for (const auto &token : tokens) {
            if (isLiteral(token) && !literalExists(token, literalTable)) {
                literalTable.emplace_back(token, litIndex++, -1);
            }
        }
    }
    inFile.close();

    assignAddresses(literalTable, LC);

    std::cout << "LITERAL TABLE (with addresses):\n";
    std::cout << "Index\tLiteral\tAddress\n";
    for (const auto &lit : literalTable) {
        std::cout << lit.ind << "\t" << lit.lit << "\t" << lit.add << '\n';
    }

    return 0;
}
