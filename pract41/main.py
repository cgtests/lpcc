import re

# Define operator precedence
precedence = {
    '^': 3,
    '*': 2, '/': 2,
    '+': 1, '-': 1
}

# Check if character is an operator
def is_operator(c):
    return c in precedence

# Convert infix to postfix using shunting yard algorithm
def infix_to_postfix(expression):
    stack = []
    output = []
    tokens = re.findall(r"[a-zA-Z_]\w*|\d+|[+*/^()-]", expression)

    for token in tokens:
        if token.isalnum():
            output.append(token)
        elif token == '(':
            stack.append(token)
        elif token == ')':
            while stack and stack[-1] != '(':
                output.append(stack.pop())
            stack.pop()  # pop '('
        elif is_operator(token):
            while (stack and stack[-1] != '(' and
                   precedence.get(token, 0) <= precedence.get(stack[-1], 0)):
                output.append(stack.pop())
            stack.append(token)
    while stack:
        output.append(stack.pop())

    return output

# Generate Three Address Code from postfix
def generate_TAC(postfix, result_var):
    stack = []
    temp_count = 1
    code = []

    for token in postfix:
        if token.isalnum():
            stack.append(token)
        elif is_operator(token):
            op2 = stack.pop()
            op1 = stack.pop()
            temp_var = f't{temp_count}'
            temp_count += 1
            code.append(f"{temp_var} = {op1} {token} {op2}")
            stack.append(temp_var)

    final_result = stack.pop()
    code.append(f"{result_var} = {final_result}")
    return code

# Main function
def main():
    expr = input("Enter an expression (e.g., a = b*b + c*c): ").strip()

    if '=' not in expr:
        print("Invalid expression format. Use: variable = expression")
        return

    lhs, rhs = map(str.strip, expr.split('='))
    postfix = infix_to_postfix(rhs)
    tac = generate_TAC(postfix, lhs)

    print("\nThree Address Code:")
    for line in tac:
        print(line)

if __name__ == "__main__":
    main()
