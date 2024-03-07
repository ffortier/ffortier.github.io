load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive", "http_file")

http_file(
    name = "v8_wasm_opcodes",
    downloaded_file_path = "wasm-opcodes.h",
    sha256 = "b8c1de55ad87384a89b2d9a623f74aa8db6b3eda899e8ef6f8436a9b040f200a",
    url = "https://raw.githubusercontent.com/v8/v8/2cff5f45aff1e9991e1239168831827e2f76ed7e/src/wasm/wasm-opcodes.h",
)

# Rust
http_archive(
    name = "rules_rust",
    sha256 = "6501960c3e4da32495d1e1007ded0769a534cb195c30dea36aa54f9d8a3f0361",
    urls = ["https://github.com/bazelbuild/rules_rust/releases/download/0.38.0/rules_rust-v0.38.0.tar.gz"],
)

load("@rules_rust//rust:repositories.bzl", "rules_rust_dependencies", "rust_register_toolchains")

rules_rust_dependencies()

rust_register_toolchains(
    edition = "2021",
    extra_target_triples = [
        "x86_64-unknown-none",
        "i686-unknown-linux-gnu",
        "wasm32-unknown-unknown",
    ],
    versions = [
        "1.75.0",
    ],
)

load("@rules_rust//tools/rust_analyzer:deps.bzl", "rust_analyzer_dependencies")

rust_analyzer_dependencies()

load("@rules_rust//crate_universe:repositories.bzl", "crate_universe_dependencies")

crate_universe_dependencies()

load("@rules_rust//crate_universe:defs.bzl", "crates_repository")

crates_repository(
    name = "crate_index",
    cargo_lockfile = "//:Cargo.Bazel.lock",
    lockfile = "//:cargo-bazel-lock.json",
    manifests = [
        "//:Cargo.toml",
        "//trex/cli:Cargo.toml",
        "//trex/parser:Cargo.toml",
        "//transpiler:Cargo.toml",
        "//rpm:Cargo.toml",
    ],
)

load("@crate_index//:defs.bzl", "crate_repositories")

crate_repositories()

# LLVM

http_archive(
    name = "toolchains_llvm",
    canonical_id = "0.10.3",
    patch_args = ["-p1"],
    patches = ["//third_party/llvm_toolchain:llvm_toolchain.patch"],
    sha256 = "b7cd301ef7b0ece28d20d3e778697a5e3b81828393150bed04838c0c52963a01",
    strip_prefix = "toolchains_llvm-0.10.3",
    urls = ["https://github.com/grailbio/bazel-toolchain/releases/download/0.10.3/toolchains_llvm-0.10.3.tar.gz"],
)

load("@toolchains_llvm//toolchain:deps.bzl", "bazel_toolchain_dependencies")

bazel_toolchain_dependencies()

load("@toolchains_llvm//toolchain:rules.bzl", "llvm_toolchain")

llvm_toolchain(
    name = "llvm_toolchain",
    llvm_version = "16.0.0",
    sysroot = {
        "linux-x86_64": "@sysroots_bullseye_amd64//:sysroot",
        "linux-x86_32": "@sysroots_bullseye_i386//:sysroot",
        "linux-aarch64": "@sysroots_bullseye_arm64//:sysroot",
        "darwin-aarch64": "@sysroot_darwin_universal//:sysroot",
        "darwin-x86_64": "@sysroot_darwin_universal//:sysroot",
    },
)

load("@llvm_toolchain//:toolchains.bzl", "llvm_register_toolchains")

llvm_register_toolchains()

http_archive(
    name = "uxn5",
    build_file = "//third_party/uxn5:BUILD.uxn5.bazel",
    sha256 = "ff2311a25cad0080f85e824a26233f6579bd609a39258efd4ac99a2a2bf1c156",
    strip_prefix = "uxn5-e6b2cca270d0284cfca2fb4784f97f5fbec90877",
    urls = ["https://git.sr.ht/~rabbits/uxn5/archive/e6b2cca270d0284cfca2fb4784f97f5fbec90877.tar.gz"],
)
