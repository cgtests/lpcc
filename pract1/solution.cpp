#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <map>
#include <algorithm>

class Mnemonic
{
public:
  std::string opCode{};
  std::string cl{};
  int machineCode{};
  Mnemonic(std::string oc, std::string cl, int mc)
      : opCode(oc), cl(cl), machineCode(mc)
  {
  }
};

class Symbol
{
public:
  std::string sym{};
  int ind{};
  int add{};

  Symbol(std::string sym, int ind, int add)
      : sym(sym), ind(ind), add(add)
  {
  }
};

class Literal
{
public:
  std::string lit{};
  int ind{};
  int add{};

  Literal(std::string lit, int ind, int add)
      : lit(lit), ind(ind), add(add)
  {
  }
};

void makeInstrArray(std::string str, std::vector<std::string> &getBlank)
{
  std::stringstream ss(str);
  std::string instruction{};
  while (ss >> instruction)
    getBlank.push_back(instruction);
}

bool isMnemonic(const std::string &instruction, Mnemonic mnemonicTable[], int tableSize)
{
  for (int i = 0; i < tableSize; i++)
  {
    if (instruction == mnemonicTable[i].opCode)
      return true;
  }
  return false;
}

bool isLiteral(const std::string &instruction)
{
  return (instruction.substr(0, 2) == "='");
}

bool isSymbol(const std::string &instruction, Mnemonic mnemonicTable[], int tableSize)
{
  if (isMnemonic(instruction, mnemonicTable, tableSize))
    return false; 
  for (char ch : instruction)
  {
    if (std::isdigit(ch))
      return false;
  }
  return true;
}

bool symbolExists(const std::string &sym, const std::vector<Symbol> &symbolTable)
{

  for (const auto &symbol : symbolTable)
  {
    if (symbol.sym == sym)
      return true;
  }

  return false;
}

// second pass-

void assignAddresses(std::vector<Symbol> &symbolTable, std::vector<Literal> &literalTable, int &LC)
{

  for (auto &lit : literalTable)
  {
    if (lit.add == -1)
    {
      lit.add = LC++;
    }
  }
  for (auto &sym : symbolTable)
  {
    if (sym.add == -1)
    {
      sym.add = LC++;
    }
  }
}

void processInstruction(std::vector<std::string> &instrArray, int &LC, std::vector<Symbol> &symbolTable,
                        std::vector<Literal> &literalTable, Mnemonic mnemonicTable[], int tableSize)
{
  static int lit_indx = 0, sym_indx = 0;


  for (size_t i = 0; i < instrArray.size(); i++)
  {
    std::string instr = instrArray[i];


    if (instr == "START")
    {
      if (i + 1 < instrArray.size())
      {
        LC = std::stoi(instrArray[i + 1]); 
        i++;                          
        continue;
      }
    }

    if (instr == "LTORG" || instr == "END")
    {
      assignAddresses(symbolTable, literalTable, LC);
      continue;
    }

    if (isMnemonic(instr, mnemonicTable, tableSize))
    {
      LC++;
    }
    else if (isLiteral(instr))
    {
      literalTable.emplace_back(instr, lit_indx++, -1);
    }
    else if (isSymbol(instr, mnemonicTable, tableSize))
    {
      if (!symbolExists(instr, symbolTable))
      {
        symbolTable.emplace_back(instr, sym_indx++, -1);
      }
    }
  }
}

void generateIntermediateFile(std::vector<std::string> &instrArray, std::ofstream &outFile, int LC)
{
  outFile << LC << " ";
  for (const auto &instr : instrArray)
  {
    outFile << instr << " ";
  }
  outFile << "\n";
}

std::map<std::string, std::pair<std::string, std::string>> MOT = {
    {"START", {"AD", "01"}},
    {"END", {"AD", "02"}},
    {"ORIGIN", {"AD", "03"}},
    {"EQU", {"AD", "04"}},
    {"LTORG", {"AD", "05"}},
    {"LOAD", {"IS", "11"}},
    {"ADD", {"IS", "01"}},
    {"MULT", {"IS", "03"}},
    {"DS", {"DL", "01"}},
    {"DC", {"DL", "02"}}};

int evaluateExpression(std::string expr, const std::vector<Symbol> &symbolTable)
{
  std::string symbolName = "";
  int offset = 0;

  size_t pos = expr.find("+");
  if (pos != std::string::npos)
  {
    symbolName = expr.substr(0, pos);
    offset = stoi(expr.substr(pos + 1));
  }
  else
  {
    pos = expr.find("-");
    if (pos != std::string::npos)
    {
      symbolName = expr.substr(0, pos);
      offset = -stoi(expr.substr(pos + 1));
    }
    else
    {
      symbolName = expr;
    }
  }

  int baseAddr = 0;
  for (auto &sym : symbolTable)
  {
    if (sym.sym == symbolName)
    {
      baseAddr = sym.add;
      break;
    }
  }

  return baseAddr + offset;
}

void process_line(std::string line, std::ofstream &ic_file, int &LC, std::vector<Symbol> &extSymbolTable, std::vector<Literal> &extLiteralTable)
{
  std::stringstream ss(line);
  std::vector<std::string> tokens;
  std::string token;

  while (ss >> token)
  {
    tokens.push_back(token);
  }
  if (tokens.empty())
    return;

  int index = 0;
  std::string label = "";

  if (MOT.find(tokens[0]) == MOT.end() && tokens[0].substr(0, 2) != "='")
  {
    label = tokens[0];

    if (!symbolExists(label, extSymbolTable))
    {
      extSymbolTable.push_back(Symbol(label, extSymbolTable.size(), LC));
    }
    else
    {

      for (auto &sym : extSymbolTable)
      {
        if (sym.sym == label)
        {
          sym.add = LC;
          break;
        }
      }
    }
    index = 1;
  }
  if (index >= tokens.size())
    return;

  std::string mnemonic = tokens[index];
  if (MOT.find(mnemonic) != MOT.end())
  {
    std::string type = MOT[mnemonic].first;
    std::string code = MOT[mnemonic].second;
    if (type == "AD")
    {
      if (mnemonic == "START")
      {
        LC = stoi(tokens[index + 1]);
        ic_file << "LC = " << LC << "\t(AD, " << code << ")\t(C, " << tokens[index + 1] << ")\n";
      }
      else if (mnemonic == "END" || mnemonic == "LTORG")
      {
        ic_file << "\t(AD, " << code << ")\n";
        assignAddresses(extSymbolTable, extLiteralTable, LC);
      }
      else if (mnemonic == "ORIGIN")
      {
        LC = evaluateExpression(tokens[index + 1], extSymbolTable);
        ic_file << "\t(AD, " << code << ")\n";
      }
      else if (mnemonic == "EQU")
      {
        std::string target = tokens[index + 1];
        int targetIdx = -1;
        for (int i = 0; i < extSymbolTable.size(); i++)
        {
          if (extSymbolTable[i].sym == target)
          {
            targetIdx = i;
            break;
          }
        }
        int labelIdx = -1;
        for (int i = 0; i < extSymbolTable.size(); i++)
        {
          if (extSymbolTable[i].sym == label)
          {
            labelIdx = i;
            break;
          }
        }
        if (labelIdx != -1 && targetIdx != -1)
        {
          extSymbolTable[labelIdx].add = extSymbolTable[targetIdx].add;
        }
        ic_file << "\t(AD, " << code << ")\t(S, " << std::setw(2) << std::setfill('0') << targetIdx + 1 << ")\n";
      }
    }
    else if (type == "DL")
    {
      ic_file << "LC = " << LC << "\t(DL, " << code << ")\t(C, " << std::setw(2) << std::setfill('0') << tokens[index + 1] << ")\n";
      if (mnemonic == "DS")
      {
        LC += stoi(tokens[index + 1]);
      }
      else
      {
        LC++;
      }
    }
    else if (type == "IS")
    {
      ic_file << "LC = " << LC << "\t(IS, " << code << ")\t";
      if (index + 1 < tokens.size())
      {
        std::string operand = tokens[index + 1];
        if (operand[0] == '=')
        {

          auto it = find_if(extLiteralTable.begin(), extLiteralTable.end(), [&](const Literal &lit)
                            { return lit.lit == operand; });
          if (it == extLiteralTable.end())
          {
            extLiteralTable.push_back(Literal(operand, extLiteralTable.size(), -1));
          }
          ic_file << "(L, " << std::setw(2) << std::setfill('0') << extLiteralTable.size() << ")";
        }
        else
        {

          int symIdx = -1;
          for (int i = 0; i < extSymbolTable.size(); i++)
          {
            if (extSymbolTable[i].sym == operand)
            {
              symIdx = i;
              break;
            }
          }
          if (symIdx == -1)
          {
            extSymbolTable.push_back(Symbol(operand, extSymbolTable.size(), -1));
            symIdx = extSymbolTable.size() - 1;
          }
          ic_file << "(S, " << std::setw(2) << std::setfill('0') << symIdx + 1 << ")";
        }
      }
      ic_file << "\n";
      LC++;
    }
    return;
  }
  else if (mnemonic[0] == '=')
  {
    ic_file << "LC = " << LC << "\t(L, " << std::setw(2) << std::setfill('0') << extLiteralTable.size() << ")\n";
    LC++;
  }
}

int main()
{

  // std::string opcodes[] = {"START", "DC", "LOAD", "ADD", "AD", "ORIGIN", "MULT", "LTORG", "DS", "EQU", "END"};
  Mnemonic mnemonicTable[] = {
      Mnemonic("STOP", "IS", 0),
      Mnemonic("ADD", "IS", 1),
      Mnemonic("SUB", "IS", 2),
      Mnemonic("MULT", "IS", 3),
      Mnemonic("MOVER", "IS", 4),
      Mnemonic("MOVEM", "IS", 5),
      Mnemonic("COMP", "IS", 6),
      Mnemonic("BC", "IS", 7),
      Mnemonic("DIV", "IS", 8),
      Mnemonic("READ", "IS", 9),
      Mnemonic("PRINT", "IS", 10),
      Mnemonic("LOAD", "IS", 11),
      Mnemonic("START", "AD", 1),
      Mnemonic("END", "AD", 2),
      Mnemonic("ORIGIN", "AD", 3),
      Mnemonic("EQU", "AD", 4),
      Mnemonic("LTORG", "AD", 5),
      Mnemonic("DS", "DL", 1),
      Mnemonic("DC", "DL", 2),
      Mnemonic("AREG", "RG", 1),
      Mnemonic("BREG", "RG", 2),
      Mnemonic("CREG", "RG", 3),
      Mnemonic("DREG", "RG", 4),
      Mnemonic("EQ", "CC", 1),
      Mnemonic("LT", "CC", 2),
      Mnemonic("GT", "CC", 3),
      Mnemonic("LE", "CC", 4),
      Mnemonic("GE", "CC", 5),
      Mnemonic("ANY", "CC", 6)};

  int tableSize = sizeof(mnemonicTable) / sizeof(mnemonicTable[0]);

  std::vector<Symbol> symbolTable;
  std::vector<Literal> literalTable;

  std::ifstream inFile("SAMPLE_input.txt");

  std::string str{};
  std::string file_content{};
  int lit_indx{0};
  int sym_indx{0};
  while (std::getline(inFile, str))
  {
    // str line

    std::vector<std::string> instrArray;

    makeInstrArray(str, instrArray);
    // std::cout << str << '\n';
    std::cout << "Instruction Array: ";

    for (const auto &instr : instrArray)
    {
      std::cout << instr << ' ';

      if (isMnemonic(instr, mnemonicTable, tableSize))
      {
        continue;
      }
      else if (isLiteral(instr))
      {
        literalTable.emplace_back(instr, lit_indx, -1);
        lit_indx++;
      }
      else if (isSymbol(instr, mnemonicTable, tableSize))
      {
        if (!symbolExists(instr, symbolTable))
        {
          symbolTable.emplace_back(instr, sym_indx, -1);
          sym_indx++;
        }
      }
    }
    std::cout << '\n';
  }
  inFile.close();

  std::cout << "\n--- Original Tables ---" << '\n';
  std::cout << "\nMnemonic Table:" << '\n';
  for (const auto &mnemonic : mnemonicTable)
  {
    std::cout << mnemonic.opCode << " " << mnemonic.cl << " " << mnemonic.machineCode << '\n';
  }
  std::cout << "\nSymbol Table:" << '\n';
  for (const auto &symbol : symbolTable)
  {
    std::cout << symbol.sym << " " << symbol.ind << " " << symbol.add << '\n';
  }
  std::cout << "\nLiteral Table:" << '\n';
  for (const auto &literal : literalTable)
  {
    std::cout << literal.lit << " " << literal.ind << " " << literal.add << '\n';
  }

  std::ifstream inFile2("SAMPLE_input.txt");
  std::ofstream outFile("intermediate.txt");
  outFile << "INTERMEDIATE CODE\n";

  int LC = 0;
  std::vector<Symbol> extSymbolTable = symbolTable;
  std::vector<Literal> extLiteralTable = literalTable;

  while (std::getline(inFile2, str))
  {
    std::vector<std::string> instrArray;
    makeInstrArray(str, instrArray);

    processInstruction(instrArray, LC, extSymbolTable, extLiteralTable, mnemonicTable, tableSize);
    generateIntermediateFile(instrArray, outFile, LC);
  }
  assignAddresses(extSymbolTable, extLiteralTable, LC);
  inFile2.close();
  outFile.close();

  std::cout << "\n--- Tables after Address Assignment ---" << '\n';
  std::cout << "\nSymbol Table (ADDRESS ASSIGNED):" << '\n';
  for (const auto &symbol : extSymbolTable)
  {
    std::cout << symbol.sym << " " << symbol.ind << " " << symbol.add << '\n';
  }
  std::cout << "\nLiteral Table (ADDRESS ASSIGNED):" << '\n';
  for (const auto &literal : extLiteralTable)
  {
    std::cout << literal.lit << " " << literal.ind << " " << literal.add << '\n';
  }

  std::vector<std::string> assemblyLines;
  std::ifstream asmFile("temp.asm");
  if (!asmFile.is_open())
  {
    std::cout << "Error: Could not open temp.asm for extended pass.\n";
    return 1;
  }
  while (std::getline(asmFile, str))
  {
    if (!str.empty())
      assemblyLines.push_back(str);
  }
  asmFile.close();

  std::ofstream ic_file("sample_IC.txt");
  ic_file << "INTERMEDIATE CODE\n";
  LC = 0;
  for (const std::string &line : assemblyLines)
  {
    process_line(line, ic_file, LC, extSymbolTable, extLiteralTable);
  }
  if (LC > 0)
  {
    ic_file << "LC = " << LC << "\n";
  }
  ic_file.close();

  std::cout << "\nIntermediate code generated in intermediate.txt and sample_IC.txt" << '\n';
  return 0;
}