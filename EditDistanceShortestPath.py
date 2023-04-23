import queue
from typing import List
from collections import deque


def read_line():
    try: return input()
    except EOFError as e: return ""


def read_input():
    tokens = []
    tok = read_line()
    while tok:
        if tok[0] == '>':
            tokens.append("")
        else:
            tokens[-1] += tok
        tok = read_line()

    return tokens


class Node:
    def __init__(self, x: int, y: int):
        self.x, self.y = x, y
        self.value: int = 0
        self.adj: List = []


def access_cell(x: int, y: int):
    return edit_distance_matrix[y][x - edit_distance_matrix[y][0].x]


tokens = read_input()

n: int = len(tokens[0])
m: int = len(tokens[1])

if n > m:
    tokens[0], tokens[1] = tokens[1], tokens[0]
    n, m = m, n

edit_distance_matrix = [deque([Node(i, i)]) for i in range(1, n + 1)]


# if __name__ == "__main__":
#     main()