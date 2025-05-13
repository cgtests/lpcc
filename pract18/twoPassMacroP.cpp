#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>

struct Macro {
    int index;
    std::vector<std::string> definition;
};

int main() {
    std::ifstream br("input.asm");
    if (!br) {
        std::cerr << "Error: Could not open input.asm\n";
        return 1;
    }

    std::unordered_map<std::string, int> pntab;
    std::unordered_map<std::string, Macro> macroTable;
    std::string line, Macroname;
    int mdtp = 1, paramNo = 1, pp = 0, kp = 0, flag = 0;

    std::vector<std::string> intermediateCode;
    std::vector<std::string> expandedCode;

    while (std::getline(br, line)) {
        std::istringstream iss(line);
        std::vector<std::string> parts;
        std::string part;

        while (iss >> part) {
            parts.push_back(part);
        }

        if (parts.empty()) continue; // Skip empty lines

        if (parts[0] == "MACRO" || parts[0] == "macro") {
            flag = 1;
            std::getline(br, line);
            std::istringstream iss2(line);
            parts.clear();
            while (iss2 >> part) {
                parts.push_back(part);
            }
            Macroname = parts[0];

            std::cout << "\nMacro Name Table (MNT):\n";
            std::cout << Macroname << " " << pp << " " << mdtp << "\n";

            if (parts.size() > 1) {
                std::cout << "\nParameter Name Table (PNTAB):\n";
                for (size_t i = 1; i < parts.size(); i++) {
                    parts[i].erase(std::remove(parts[i].begin(), parts[i].end(), '&'), parts[i].end());
                    parts[i].erase(std::remove(parts[i].begin(), parts[i].end(), ','), parts[i].end());

                    if (parts[i].find('=') != std::string::npos) {
                        kp++;
                        std::string keywordParam = parts[i].substr(0, parts[i].find('='));
                        pntab[keywordParam] = paramNo++;
                    } else {
                        pntab[parts[i]] = paramNo++;
                        pp++;
                    }
                    std::cout << parts[i] << " #" << i << "\n";
                }
            }
            macroTable[Macroname] = {mdtp, {}};
        } else if (parts[0] == "MEND" || parts[0] == "mend") {
            std::cout << "\nMacro Definition Table (MDT):\nMEND\n";
            flag = kp = pp = 0;
            mdtp++;
            paramNo = 1;
            std::cout << "\nFinal Parameter Table:\n";
            for (const auto& entry : pntab) {
                std::cout << entry.first << "\n";
            }
            pntab.clear();
        } else if (flag == 1) {
            std::ostringstream mdtEntry;
            for (size_t i = 0; i < parts.size(); i++) {
                if (parts[i].find('&') != std::string::npos) {
                    parts[i].erase(std::remove(parts[i].begin(), parts[i].end(), '&'), parts[i].end());
                    mdtEntry << "(P," << pntab[parts[i]] << ") ";
                } else {
                    mdtEntry << parts[i] << " ";
                }
            }
            macroTable[Macroname].definition.push_back(mdtEntry.str());
            std::cout << "(MDT) " << mdtEntry.str() << "\n";
            mdtp++;
        } else {
            intermediateCode.push_back(line);
        }
    }
    br.close();

    std::cout << "\nIntermediate Code:\n";
    for (const auto& instr : intermediateCode) {
        std::cout << instr << "\n";
    }

    // Expanding Macros
    std::cout << "\nExpanded Code:\n";
    for (const auto& instr : intermediateCode) {
        std::istringstream iss(instr);
        std::vector<std::string> parts;
        std::string part;

        while (iss >> part) {
            parts.push_back(part);
        }

        if (macroTable.find(parts[0]) != macroTable.end()) {
            Macro macro = macroTable[parts[0]];
            std::unordered_map<int, std::string> argMap;

            // Mapping arguments
            for (size_t i = 1; i < parts.size(); i++) {
                argMap[i] = parts[i];
            }

            // Expanding macro
            for (const auto& line : macro.definition) {
                std::string expandedLine = line;
                for (const auto& entry : argMap) {
                    std::string placeholder = "(P," + std::to_string(entry.first) + ")";
                    size_t pos = expandedLine.find(placeholder);
                    while (pos != std::string::npos) {
                        expandedLine.replace(pos, placeholder.length(), entry.second);
                        pos = expandedLine.find(placeholder);
                    }
                }
                expandedCode.push_back(expandedLine);
            }
        } else {
            expandedCode.push_back(instr);
        }
    }

    for (const auto& instr : expandedCode) {
        std::cout << instr << "\n";
    }

    std::cout << "\nMacro Pass 1 Processing Done.\n";
    return 0;
}
