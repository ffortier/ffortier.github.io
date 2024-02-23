from argparse import ArgumentParser

parser = ArgumentParser()

parser.add_argument("--boot_loader", "-b", required=True)
parser.add_argument("--kernel", "-k", required=True)
parser.add_argument("--out", "-o", required=True)
parser.add_argument("--sector_size", type=int, default=512)
parser.add_argument("--kernel_total_sectors", type=int, required=True)

args = parser.parse_args()

with open(args.out, "wb") as out:
    with open(args.boot_loader, "rb") as boot_loader:
        out.write(boot_loader.read())

    assert out.tell() == args.sector_size

    with open(args.kernel, "rb") as kernel:
        out.write(kernel.read())

    padding = args.sector_size - (out.tell() % args.sector_size)
    bundle_size = (args.kernel_total_sectors + 1) * args.sector_size

    assert out.tell() <= bundle_size

    out.write(b"\0" * (bundle_size - out.tell()))
