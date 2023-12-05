load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive", "http_file")

http_archive(
    name = "rules_rust",
    sha256 = "75177226380b771be36d7efc538da842c433f14cd6c36d7660976efb53defe86",
    urls = ["https://github.com/bazelbuild/rules_rust/releases/download/0.34.1/rules_rust-v0.34.1.tar.gz"],
)

http_archive(
    name = "rules_jvm_external",
    sha256 = "d31e369b854322ca5098ea12c69d7175ded971435e55c18dd9dd5f29cc5249ac",
    strip_prefix = "rules_jvm_external-5.3",
    url = "https://github.com/bazelbuild/rules_jvm_external/releases/download/5.3/rules_jvm_external-5.3.tar.gz",
)

http_archive(
    name = "contrib_rules_jvm",
    sha256 = "4d62589dc6a55e74bbe33930b826d593367fc777449a410604b2ad7c6c625ef7",
    strip_prefix = "rules_jvm-0.19.0",
    url = "https://github.com/bazel-contrib/rules_jvm/releases/download/v0.19.0/rules_jvm-v0.19.0.tar.gz",
)

http_archive(
    name = "rules_java",
    sha256 = "4018e97c93f97680f1650ffd2a7530245b864ac543fd24fae8c02ba447cb2864",
    urls = [
        "https://github.com/bazelbuild/rules_java/releases/download/7.3.1/rules_java-7.3.1.tar.gz",
    ],
)

http_archive(
    name = "rules_python",
    sha256 = "e85ae30de33625a63eca7fc40a94fea845e641888e52f32b6beea91e8b1b2793",
    strip_prefix = "rules_python-0.27.1",
    url = "https://github.com/bazelbuild/rules_python/releases/download/0.27.1/rules_python-0.27.1.tar.gz",
)

http_file(
    name = "v8_wasm_opcodes",
    downloaded_file_path = "wasm-opcodes.h",
    sha256 = "b8c1de55ad87384a89b2d9a623f74aa8db6b3eda899e8ef6f8436a9b040f200a",
    url = "https://raw.githubusercontent.com/v8/v8/2cff5f45aff1e9991e1239168831827e2f76ed7e/src/wasm/wasm-opcodes.h",
)

http_archive(
    name = "io_tweag_rules_nixpkgs",
    sha256 = "980edfceef2e59e1122d9be6c52413bc298435f0a3d452532b8a48d7562ffd67",
    strip_prefix = "rules_nixpkgs-0.10.0",
    urls = ["https://github.com/tweag/rules_nixpkgs/releases/download/v0.10.0/rules_nixpkgs-0.10.0.tar.gz"],
)

# Rust

load("@rules_rust//rust:repositories.bzl", "rules_rust_dependencies", "rust_register_toolchains")

rules_rust_dependencies()

rust_register_toolchains(
    edition = "2021",
)

load("@rules_rust//crate_universe:repositories.bzl", "crate_universe_dependencies")

crate_universe_dependencies()

load("@rules_rust//crate_universe:defs.bzl", "crates_repository")

crates_repository(
    name = "crate_index_trex",
    cargo_lockfile = "//trex:Cargo.Bazel.lock",
    lockfile = "//trex:cargo-bazel-lock.json",
    manifests = [
        "//trex:Cargo.toml",
        "//trex/cli:Cargo.toml",
        "//trex/parser:Cargo.toml",
    ],
)

load(
    "@crate_index_trex//:defs.bzl",
    trex_crate_repositories = "crate_repositories",
)

trex_crate_repositories()

# Python
load("@rules_python//python:repositories.bzl", "py_repositories", "python_register_toolchains")

py_repositories()

python_register_toolchains(
    name = "python311",
    python_version = "3.11",
)

# Java

load("@rules_java//java:repositories.bzl", "rules_java_dependencies", "rules_java_toolchains")

rules_java_dependencies()

rules_java_toolchains()

load("@rules_jvm_external//:repositories.bzl", "rules_jvm_external_deps")

rules_jvm_external_deps()

load("@rules_jvm_external//:setup.bzl", "rules_jvm_external_setup")

rules_jvm_external_setup()

load("@rules_jvm_external//:defs.bzl", "maven_install")

maven_install(
    artifacts = [
        "org.ow2.asm:asm:9.6",
        "org.ow2.asm:asm-util:9.6",
        "io.soabase.record-builder:record-builder-processor:37",
        "io.soabase.record-builder:record-builder-core:37",
        "org.junit.jupiter:junit-jupiter-api:5.10.1",
        "org.junit.jupiter:junit-jupiter-engine:5.10.1",
        "org.junit.platform:junit-platform-launcher:1.10.1",
        "org.junit.platform:junit-platform-reporting:1.10.1",
        "org.assertj:assertj-core:3.24.2",
    ],
    fetch_javadoc = True,
    fetch_sources = True,
    maven_install_json = "//third_party/java:maven_install.json",
    repositories = [
        "https://repo1.maven.org/maven2",
    ],
)

load("@maven//:defs.bzl", "pinned_maven_install")

pinned_maven_install()

# contrib_rules_jvm
load("@contrib_rules_jvm//:repositories.bzl", "contrib_rules_jvm_deps")

contrib_rules_jvm_deps()

load("@contrib_rules_jvm//:setup.bzl", "contrib_rules_jvm_setup")

contrib_rules_jvm_setup()

# io_tweag_rules_nixpkgs

load("@io_tweag_rules_nixpkgs//nixpkgs:repositories.bzl", "rules_nixpkgs_dependencies")

rules_nixpkgs_dependencies()

load("@io_tweag_rules_nixpkgs//nixpkgs:nixpkgs.bzl", "nixpkgs_git_repository", "nixpkgs_package")

nixpkgs_git_repository(
    name = "nixpkgs",
    revision = "23.11",
    sha256 = "bc9a0a74e8d7fb0e11434dd3abaa0cb0572ccd3a65b5a192eea41832b286e8a0",
)

nixpkgs_package(
    name = "gnuplot",
    build_file_content = """
load("@bazel_skylib//rules:copy_file.bzl", "copy_file")

copy_file(
    name = "gnuplot",
    src = "bin/gnuplot",
    out = "gnuplot.exe",
    is_executable = True,
    visibility = ["//visibility:public"],
)
    """,
    repository = "@nixpkgs//:default.nix",
)
