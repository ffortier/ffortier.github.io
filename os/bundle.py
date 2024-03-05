from argparse import ArgumentParser
from pathlib import Path, PosixPath
from fs import open_fs
from fs.base import FS
from sys import stderr


def copy_tree(source: Path, dest: FS, path: PosixPath):
    for file in source.iterdir():
        fs_path = path.joinpath(file.name)
        if file.is_dir():
            assert dest.makedir(str(fs_path))
            copy_tree(file, dest, fs_path)
        else:
            assert file.is_file()
            with open(file, "rb") as input:
                fs.upload(str(fs_path), input)


if __name__ == "__main__":
    parser = ArgumentParser()

    parser.add_argument("--boot_loader", "-b", required=True)
    parser.add_argument("--kernel", "-k", required=True)
    parser.add_argument("--out", "-o", required=True)
    parser.add_argument("--fs_root", required=True)
    parser.add_argument("--sector_size", type=int, default=512)
    parser.add_argument("--kernel_total_sectors", type=int, required=True)
    parser.add_argument("--disk_size", type=int, required=True)

    args = parser.parse_args()

    with open(args.out, "wb") as out:
        with open(args.boot_loader, "rb") as boot_loader:
            out.write(boot_loader.read())

        assert out.tell() == args.sector_size

        with open(args.kernel, "rb") as kernel:
            out.write(kernel.read())

        padding = args.sector_size - (out.tell() % args.sector_size)
        kernel_size = (args.kernel_total_sectors + 1) * args.sector_size

        assert kernel_size <= args.disk_size
        assert out.tell() <= kernel_size

        out.write(b"\0" * (args.disk_size - out.tell()))

    with open_fs(f"fat://{Path(args.out).absolute()}") as fs:
        fs_root = Path(args.fs_root)

        copy_tree(fs_root, fs, PosixPath("/"))
        fs.tree()
