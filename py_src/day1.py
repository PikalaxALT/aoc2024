#!/usr/bin/env python

from lib import Impl


class Day1(Impl, day=1):
    def __init__(self, data: str):
        self.left = []
        self.right = []
        for line in data.split("\n"):
            if not line:
                break
            a, b = line.split()
            self.left.append(int(a))
            self.right.append(int(b))

    def part1(self):
        self.left.sort()
        self.right.sort()
        print(sum(abs(a - b) for a, b in zip(self.left, self.right)))

    def part2(self):
        import collections

        counts = collections.Counter(self.right)
        print(sum(x * counts[x] for x in self.left))


if __name__ == "__main__":
    Day1.run()
