import sys

class Symbol:
    def __init__(self, name, address):
        self.name = name
        self.address = address

def main():
    try:
        infile = open("input.asm", "r")
    except IOError:
        print("Error opening input file.", file=sys.stderr)
        return 1

    opcodeTable = {
        "READ": "09",
        "MOVER": "04",
        "SUB": "05",
        "STOP": "00",
        "START": "01",
        "END": "02",
        "DS": "01"
    }

    classTable = {
        "READ": "IS",
        "MOVER": "IS",
        "SUB": "IS",
        "STOP": "IS",
        "START": "AD",
        "END": "AD",
        "DS": "DL"
    }

    registerTable = {
        "AREG": 1,
        "A":1,
        "BREG": 2,
        "B": 2,
        "CREG": 3,
        "C": 3

    }

    symbolTable = []
    symIndex = {}  # name to index
    intermediateCode = []

    LC = 0

    for line in infile:
        tokens = line.split()
        if not tokens:
            continue

        # START
        if tokens[0] == "START":
            LC = int(tokens[1])
            intermediateCode.append(f"(AD,01) (C,{LC})")

        # END
        elif tokens[0] == "END":
            intermediateCode.append("(AD,02)")

        # Declarative Statement (e.g., A DS 1)
        elif len(tokens) == 3 and tokens[1] == "DS":
            symIndex[tokens[0]] = len(symbolTable)
            symbolTable.append(Symbol(tokens[0], LC))
            intermediateCode.append(f"(DL,01) (C,{tokens[2]})")
            LC += 1

        # Instructions
        else:
            opcode = ""
            reg = ""
            operand = ""
            i = 0

            # Check if it has label
            if tokens[0] not in opcodeTable:
                # It's a label
                i = 1

            opcode = tokens[i]

            if len(tokens) > i + 1:
                reg = tokens[i + 1]

            if len(tokens) > i + 2:
                operand = tokens[i + 2]

            ic = f"({classTable[opcode]},{opcodeTable[opcode]}) "

            if reg:
                # Remove comma if exists
                if reg.endswith(','):
                    reg = reg[:-1]

                ic += f"({registerTable[reg]}) "

            if operand:
                if operand not in symIndex:
                    # Add symbol if not found
                    symIndex[operand] = len(symbolTable)
                    symbolTable.append(Symbol(operand, -1))
                ic += f"(S,{symIndex[operand]})"

            intermediateCode.append(ic)
            LC += 1

    infile.close()

    # Output Intermediate Code
    print("Intermediate Code:")
    for line in intermediateCode:
        print(line)

    # Output Symbol Table
    print("\nSymbol Table:")
    print(f"{'Index':<10}{'Symbol':<10}{'Address':<10}")
    for i, symbol in enumerate(symbolTable):
        print(f"{i:<10}{symbol.name:<10}{symbol.address:<10}")

if __name__ == "__main__":
    main()

