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
        "x86_64-unknown-linux-gnu",
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
    canonical_id = "1.0.0",
    patch_args = ["-p1"],
    patches = ["//third_party/llvm_toolchain:llvm_toolchain.patch"],
    sha256 = "e91c4361f99011a54814e1afbe5c436e0d329871146a3cd58c23a2b4afb50737",
    strip_prefix = "toolchains_llvm-1.0.0",
    url = "https://github.com/bazel-contrib/toolchains_llvm/releases/download/1.0.0/toolchains_llvm-1.0.0.tar.gz",
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

llvm_toolchain(
    name = "llvm_toolchain_rbe",
    distribution = "ubuntu",
    exec_arch = "x86_64",
    exec_os = "linux",
    llvm_version = "16.0.0",
    sysroot = {
        "linux-x86_64": "@sysroots_bullseye_amd64//:sysroot",
        "linux-x86_32": "@sysroots_bullseye_i386//:sysroot",
        "linux-aarch64": "@sysroots_bullseye_arm64//:sysroot",
        "darwin-aarch64": "@sysroot_darwin_universal//:sysroot",
        "darwin-x86_64": "@sysroot_darwin_universal//:sysroot",
    },
)

http_archive(
    name = "uxn5",
    build_file = "//third_party/uxn5:BUILD.uxn5.bazel",
    sha256 = "ff2311a25cad0080f85e824a26233f6579bd609a39258efd4ac99a2a2bf1c156",
    strip_prefix = "uxn5-e6b2cca270d0284cfca2fb4784f97f5fbec90877",
    urls = ["https://git.sr.ht/~rabbits/uxn5/archive/e6b2cca270d0284cfca2fb4784f97f5fbec90877.tar.gz"],
)

http_archive(
    name = "raylib",
    build_file = "//third_party/raylib:BUILD.raylib.bazel",
    patch_args = ["-p1"],
    patches = ["//third_party/raylib:raylib_glfw.patch"],
    sha256 = "98f049b9ea2a9c40a14e4e543eeea1a7ec3090ebdcd329c4ca2cf98bc9793482",
    strip_prefix = "raylib-5.0",
    urls = ["https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz"],
)
