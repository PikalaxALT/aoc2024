import abc
import pathlib

__all__ = ("Impl",)

examples_dir = pathlib.Path(__file__).parent.parent / "data/examples"
testcase_dir = pathlib.Path(__file__).parent.parent / "data/testcases"


class Impl:
    day: int

    @abc.abstractmethod
    def __init__(self, data: str): ...

    @abc.abstractmethod
    def part1(): ...

    @abc.abstractmethod
    def part2(): ...

    def __init_subclass__(cls, day: int) -> None:
        cls.day = day

    @classmethod
    def run(cls):
        try:
            with open(examples_dir / f"{cls.day}.txt") as fp:
                example = fp.read()
        except FileNotFoundError:
            example = None

        try:
            with open(testcase_dir / f"{cls.day}.txt") as fp:
                testcase = fp.read()
        except FileNotFoundError:
            testcase = None

        example_instance = example and cls(example)
        testcase_instance = testcase and cls(testcase)

        if example:
            print("")
            print("=== PART 1 EXAMPLE ===")
            print("")
            example_instance.part1()

        if testcase:
            print("")
            print("=== PART 1 TESTCASE ===")
            print("")
            testcase_instance.part1()

        if example:
            print("")
            print("=== PART 2 EXAMPLE ===")
            print("")
            example_instance.part2()

        if testcase:
            print("")
            print("=== PART 2 TESTCASE ===")
            print("")
            testcase_instance.part2()
