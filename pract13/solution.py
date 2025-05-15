import sys
from typing import List, Dict
from dataclasses import dataclass, field

@dataclass
class Macro:
    name: str
    mdt_index: int
    parameters: List[str] = field(default_factory=list)

MDT: List[str] = []
MNT: Dict[str, Macro] = {}

def process_pass1(filepath: str):
    global MDT, MNT
    in_macro = False
    current_macro = None

    try:
        with open(filepath, 'r') as infile:
            for line in infile:
                line = line.strip()
                if not line:
                    continue
                tokens = line.split()
                if not tokens:
                    continue
                word = tokens[0]

                if word == "MACRO":
                    in_macro = True
                    header = infile.readline().strip()
                    if not header:
                        continue
                    parts = header.split()
                    macro_name = parts[0]
                    params = []

                    if len(parts) > 1:
                        params = [p.strip() for p in ' '.join(parts[1:]).split(',')]

                    current_macro = Macro(name=macro_name, mdt_index=len(MDT), parameters=params)
                    MNT[macro_name] = current_macro

                elif word == "MEND":
                    MDT.append("MEND")
                    in_macro = False
                elif in_macro:
                    MDT.append(line)
    except IOError:
        print("Error reading input file.", file=sys.stderr)
        sys.exit(1)

def expand_macro(macro_name: str, actual_args: List[str]) -> List[str]:
    expanded = []
    if macro_name not in MNT:
        return [macro_name]  # Unknown macro, return as-is

    macro = MNT[macro_name]
    ALA = {param: actual_args[i] for i, param in enumerate(macro.parameters) if i < len(actual_args)}

    i = macro.mdt_index
    while MDT[i] != "MEND":
        line = MDT[i]
        tokens = line.split()
        result = ""
        for token in tokens:
            result += ALA.get(token, token) + " "
        expanded.append(result.strip())
        i += 1

    return expanded

def process_pass2(filepath: str):
    output = []
    in_macro = False

    try:
        with open(filepath, 'r') as infile:
            for line in infile:
                line = line.strip()
                if not line:
                    continue
                tokens = line.split()
                if not tokens:
                    continue

                word = tokens[0]

                if word == "MACRO":
                    in_macro = True
                elif word == "MEND":
                    in_macro = False
                elif in_macro:
                    continue  # Skip macro body
                else:
                    macro_name = tokens[0]
                    if macro_name in MNT:
                        args = []
                        if len(tokens) > 1:
                            args_line = ' '.join(tokens[1:])
                            args = [arg.strip() for arg in args_line.split(',')]
                        expanded = expand_macro(macro_name, args)
                        output.extend(expanded)
                    else:
                        output.append(line)
    except IOError:
        print("Error reading input file during Pass 2.", file=sys.stderr)
        sys.exit(1)

    # Output Intermediate Code
    print("\n--- Intermediate Code ---")
    for line in output:
        print(line)

    # Output MNT
    print("\n--- Macro Name Table (MNT) ---")
    print(f"{'Name':<10}{'MDT Index':<12}{'Parameters'}")
    for macro in MNT.values():
        params = ' '.join(macro.parameters)
        print(f"{macro.name:<10}{macro.mdt_index:<12}{params}")

    # Output MDT
    print("\n--- Macro Definition Table (MDT) ---")
    for i, line in enumerate(MDT):
        print(f"{i} : {line}")

def main():
    filename = "input.asm"
    process_pass1(filename)
    process_pass2(filename)

if __name__ == "__main__":
    main()
