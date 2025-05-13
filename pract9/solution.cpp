#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <string>

using namespace std;

// Check if a token is a literal
bool isLiteral(const string &token) {
    return token.size() > 2 && token.substr(0, 2) == "='";
}

int main() {
    ifstream infile("input.asm");  // Open the asm file
    if (!infile.is_open()) {
        cerr << "Error: Could not open file." << endl;
        return 1;
    }

    string line;
    vector<string> literalTable;
    set<string> literalSet;
    vector<int> poolTable;
    int literalIndex = 0;

    while (getline(infile, line)) {
        stringstream ss(line);
        string token;
        while (ss >> token) {
            if (isLiteral(token)) {
                if (literalSet.find(token) == literalSet.end()) {
                    literalSet.insert(token);
                    literalTable.push_back(token);
                }
            } else if (token == "LTORG" || token == "END") {
                if (literalIndex < literalTable.size()) {
                    poolTable.push_back(literalIndex);
                    literalIndex = literalTable.size(); // update literal pool index
                }
            }
        }
    }

    infile.close();

    // Output Pool Table
    cout << "POOL TABLE" << endl;
    for (int i = 0; i < poolTable.size(); ++i) {
        cout << "#" << (i + 1) << " -> " << poolTable[i] << endl;
    }

    return 0;
}
