import sys

class Literal:
    def __init__(self, lit, ind, add):
        self.lit = lit
        self.ind = ind
        self.add = add

def is_literal(token):
    return token.startswith("='")

def literal_exists(lit, table):
    return any(l.lit == lit for l in table)

def assign_addresses(literal_table, LC):
    for lit in literal_table:
        if lit.add == -1:
            lit.add = LC[0]
            LC[0] += 1

def make_tokens(line):
    return line.split()

def is_pseudo(tok):
    return tok in ["START", "END", "LTORG"]

def is_storage_directive(tok):
    return tok in ["DS", "DC"]

def is_instruction(tok):
    ops = [
        "ADD", "SUB", "MULT", "MOVER", "MOVEM", "COMP", "BC", "DIV", "READ", "PRINT", "LOAD", "STOP"
    ]
    return tok in ops

def main():
    try:
        with open("SAMPLE_input.txt", "r") as inFile:
            literal_table = []
            LC = [0]
            lit_index = 0
            start_found = False

            for line in inFile:
                tokens = make_tokens(line.strip())
                if not tokens:
                    continue

                # START Directive
                if tokens[0] == "START" and len(tokens) > 1 and not start_found:
                    LC[0] = int(tokens[1])
                    start_found = True
                    continue

                # Assign literals at LTORG or END
                if tokens[0] in ["LTORG", "END"]:
                    assign_addresses(literal_table, LC)
                    continue

                # Label present
                tok_index = 0 if (is_instruction(tokens[0]) or is_pseudo(tokens[0]) or is_storage_directive(tokens[0])) else 1

                # Instructions or Storage directive
                if tok_index < len(tokens):
                    op = tokens[tok_index]
                    if is_instruction(op) or is_storage_directive(op):
                        LC[0] += 1

                # Add any literal
                for tok in tokens:
                    if is_literal(tok) and not literal_exists(tok, literal_table):
                        literal_table.append(Literal(tok, lit_index, -1))
                        lit_index += 1

            print("LITERAL TABLE (with addresses):")
            print("Index\tLiteral\tAddress")
            for lit in literal_table:
                print(f"{lit.ind}\t{lit.lit}\t{lit.add}")

    except FileNotFoundError:
        print("File open error.", file=sys.stderr)
        return 1

if __name__ == "__main__":
    main()

