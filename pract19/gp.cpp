#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>

using namespace std;

struct Macro {
    string name;
    int mdt_index;
    vector<string> parameters;
};

vector<string> MDT;
map<string, Macro> MNT;

void processPass1(ifstream& infile) {
    string line;
    bool inMacro = false;
    Macro currentMacro;
    int mdt_ptr = 0;

    while (getline(infile, line)) {
        stringstream ss(line);
        string word;
        ss >> word;

        if (word == "MACRO") {
            inMacro = true;
            getline(infile, line); // macro header line (e.g., ABC p, q)
            stringstream macroLine(line);
            macroLine >> currentMacro.name;

            // Extract formal parameters
            string param;
            while (getline(macroLine, param, ',')) {
                size_t space = param.find(' ');
                if (space != string::npos) param = param.substr(space + 1);
                currentMacro.parameters.push_back(param);
            }

            currentMacro.mdt_index = MDT.size();
            MNT[currentMacro.name] = currentMacro;
        }
        else if (word == "MEND") {
            MDT.push_back("MEND");
            inMacro = false;
        }
        else if (inMacro) {
            MDT.push_back(line);
        }
    }
}

vector<string> expandMacro(const string& macroName, const vector<string>& actualArgs) {
    vector<string> expanded;
    if (MNT.find(macroName) == MNT.end()) return {macroName}; // Unknown macro

    Macro macro = MNT[macroName];
    map<string, string> ALA;
    for (size_t i = 0; i < macro.parameters.size(); ++i) {
        if (i < actualArgs.size())
            ALA[macro.parameters[i]] = actualArgs[i];
    }

    for (int i = macro.mdt_index; MDT[i] != "MEND"; ++i) {
        string line = MDT[i];
        stringstream ss(line);
        string token, result;
        while (ss >> token) {
            if (ALA.find(token) != ALA.end())
                result += ALA[token] + " ";
            else
                result += token + " ";
        }
        expanded.push_back(result);
    }

    return expanded;
}

void processPass2(const string& filename) {
    ifstream infile(filename);
    string line;
    bool inMacro = false;
    vector<string> output;

    while (getline(infile, line)) {
        stringstream ss(line);
        string word;
        ss >> word;

        if (word == "MACRO") {
            inMacro = true;
        }
        else if (word == "MEND") {
            inMacro = false;
        }
        else if (inMacro) {
            continue; // skip macro definitions in pass 2
        }
        else {
            string macroName = word;

            if (MNT.find(macroName) != MNT.end()) {
                // Macro invocation
                vector<string> actualArgs;
                string arg;
                while (getline(ss, arg, ',')) {
                    actualArgs.push_back(arg);
                }
                vector<string> expanded = expandMacro(macroName, actualArgs);
                output.insert(output.end(), expanded.begin(), expanded.end());
            } else {
                output.push_back(line);
            }
        }
    }

    // Output Intermediate Code (IC)
    cout << "\n--- Intermediate Code ---\n";
    for (const string& line : output) {
        cout << line << endl;
    }

    // Output MNT
    cout << "\n--- Macro Name Table (MNT) ---\n";
    cout << setw(10) << "Name" << setw(15) << "MDT Index" << setw(20) << "Parameters" << endl;
    for (const auto& entry : MNT) {
        cout << setw(10) << entry.second.name << setw(15) << entry.second.mdt_index;
        for (const auto& p : entry.second.parameters) {
            cout << " " << p;
        }
        cout << endl;
    }

    // Output MDT
    cout << "\n--- Macro Definition Table (MDT) ---\n";
    for (size_t i = 0; i < MDT.size(); ++i) {
        cout << i << " : " << MDT[i] << endl;
    }
}

int main() {
    ifstream infile("input.asm");
    if (!infile) {
        cerr << "Cannot open input.asm\n";
        return 1;
    }

    // Pass 1: Process macro definitions and build MDT, MNT
    processPass1(infile);
    infile.close();

    // Pass 2: Expand macros and generate intermediate code
    processPass2("input.asm");

    return 0;
}
