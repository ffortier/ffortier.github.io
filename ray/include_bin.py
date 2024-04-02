from argparse import ArgumentParser, BooleanOptionalAction

parser = ArgumentParser()

parser.add_argument("--input", "-i", required=True)
parser.add_argument("--output", "-o", required=True)
parser.add_argument("--constant", "-c", required=True)
parser.add_argument(
    "--null_terminator", "-n", default=True, action=BooleanOptionalAction
)

args = parser.parse_args()

with open(args.output, "w") as output:
    print("#include <stddef.h>", file=output)
    print("", file=output)
    print(f"const char {args.constant}_DATA[] = {{", file=output)

    with open(args.input, "r") as input:
        line = input.readline().rstrip("\r\n")

        width = len(line) if line else 0
        height = 0

        while line:
            if len(line) > 0:
                assert len(line) == width
                height += 1
                chars = [f"'{ch}'" for ch in line]
                print(*chars, sep=", ", end=",\n", file=output)

            line = input.readline().rstrip("\r\n")

    print("};", file=output)
    print("", file=output)
    print(f"const size_t {args.constant}_WIDTH = {width};", file=output)
    print(f"const size_t {args.constant}_HEIGHT = {height};", file=output)
