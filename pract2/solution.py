import sys

class Symbol:
    def __init__(self, s, i, a):
        self.sym = s
        self.index = i
        self.address = a

def is_mnemonic(token):
    mnemonics = [
        "START", "END", "ORIGIN", "EQU", "LTORG",
        "LOAD", "ADD", "MULT", "DS", "DC", "SUB",
        "MOVER", "MOVEM", "COMP", "BC", "DIV",
        "READ", "PRINT", "STOP"
    ]
    return token in mnemonics

def is_symbol(token):
    if is_mnemonic(token):
        return False
    return all(not ch.isdigit() for ch in token)

def symbol_exists(name, table):
    return any(sym.sym == name for sym in table)

def assign_addresses(symbol_table, LC):
    for sym in symbol_table:
        if sym.address == -1:
            sym.address = LC[0]
            LC[0] += 1

def process_line(line, symbol_table, LC, index):
    tokens = line.split()
    
    for i in range(len(tokens)):
        if tokens[i] == "START" and i + 1 < len(tokens):
            LC[0] = int(tokens[i + 1])
            break
        elif is_symbol(tokens[i]) and not symbol_exists(tokens[i], symbol_table):
            symbol_table.append(Symbol(tokens[i], index[0], -1))
            index[0] += 1

def main():
    try:
        with open("SAMPLE_input.txt", "r") as input_file:
            symbol_table = []
            line = ""
            LC = [0]
            index = [0]

            for line in input_file:
                line = line.strip()
                if line:
                    process_line(line, symbol_table, LC, index)

            assign_addresses(symbol_table, LC)

            print("SYMBOL TABLE")
            print("Index\tSymbol\tAddress")
            for sym in symbol_table:
                print(f"{sym.index}\t{sym.sym}\t{sym.address}")

    except FileNotFoundError:
        print("Failed to open input file.", file=sys.stderr)
        return 1

if __name__ == "__main__":
    main()

