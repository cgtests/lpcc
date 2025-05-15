import os

# Check if a token is a literal
def is_literal(token):
    return len(token) > 2 and token.startswith("='")

def main():
    if not os.path.isfile("input.asm"):
        print("Error: Could not open file.")
        return 1

    literal_table = []
    literal_set = set()
    pool_table = []
    literal_index = 0

    with open("input.asm", "r") as infile:
        for line in infile:
            tokens = line.split()
            for token in tokens:
                if is_literal(token):
                    if token not in literal_set:
                        literal_set.add(token)
                        literal_table.append(token)
                elif token in ["LTORG", "END"]:
                    if literal_index < len(literal_table):
                        pool_table.append(literal_index)
                        literal_index = len(literal_table)  # update literal pool index

    # Output Pool Table
    print("POOL TABLE")
    for i in range(len(pool_table)):
        print(f"#{i + 1} -> {pool_table[i]}")

if __name__ == "__main__":
    main()

