# This is a sample Python script.

# Press ⌃R to execute it or replace it with your code.
# Press Double ⇧ to search everywhere for classes, files, tool windows, actions, and settings.


def calculator():
    print("===== CALCULATOR =====\n")

    print("Enter your expression: ")
    input_string = input()
    input_string = input_string.replace(" ", "")

    stack_values = []

    value_str = ""
    for character in input_string:
        if character == "+":
            lhs = stack_values.pop()
            rhs = stack_values.pop()
            stack_values.append(lhs + rhs)
        elif character == "-":
            rhs = stack_values.pop()
            lhs = stack_values.pop()
            stack_values.append(lhs - rhs)
        elif character == "*":
            lhs = stack_values.pop()
            rhs = stack_values.pop()
            stack_values.append(lhs * rhs)
        elif character == "//":
            rhs = stack_values.pop()
            lhs = stack_values.pop()
            stack_values.append(lhs // rhs)
        elif character == "%":
            rhs = stack_values.pop()
            lhs = stack_values.pop()
            stack_values.append(lhs % rhs)
        else:
            value_str += character









# Press the green button in the gutter to run the script.
calculator()
