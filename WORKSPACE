load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive", "http_file")

http_archive(
    name = "rules_nodejs",
    sha256 = "162f4adfd719ba42b8a6f16030a20f434dc110c65dc608660ef7b3411c9873f9",
    strip_prefix = "rules_nodejs-6.0.2",
    url = "https://github.com/bazelbuild/rules_nodejs/releases/download/v6.0.2/rules_nodejs-v6.0.2.tar.gz",
)

http_archive(
    name = "aspect_rules_js",
    sha256 = "d9ceb89e97bb5ad53b278148e01a77a3e9100db272ce4ebdcd59889d26b9076e",
    strip_prefix = "rules_js-1.34.0",
    url = "https://github.com/aspect-build/rules_js/releases/download/v1.34.0/rules_js-v1.34.0.tar.gz",
)

http_archive(
    name = "rules_rust",
    sha256 = "36ab8f9facae745c9c9c1b33d225623d976e78f2cc3f729b7973d8c20934ab95",
    urls = ["https://github.com/bazelbuild/rules_rust/releases/download/0.31.0/rules_rust-v0.31.0.tar.gz"],
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
    sha256 = "8d2ae8941c8028eb826e4c83ad3a0acb73d3237861d9ce788a3a97d60ceac45b",
    urls = [
        "https://github.com/bazelbuild/rules_java/releases/download/7.3.0/rules_java-7.3.0.tar.gz",
    ],
)

http_archive(
    name = "rules_python",
    sha256 = "9acc0944c94adb23fba1c9988b48768b1bacc6583b52a2586895c5b7491e2e31",
    strip_prefix = "rules_python-0.27.0",
    url = "https://github.com/bazelbuild/rules_python/releases/download/0.27.0/rules_python-0.27.0.tar.gz",
)

http_file(
    name = "v8_wasm_opcodes",
    downloaded_file_path = "wasm-opcodes.h",
    sha256 = "b8c1de55ad87384a89b2d9a623f74aa8db6b3eda899e8ef6f8436a9b040f200a",
    url = "https://raw.githubusercontent.com/v8/v8/2cff5f45aff1e9991e1239168831827e2f76ed7e/src/wasm/wasm-opcodes.h",
)

# Javascript

load("@aspect_rules_js//js:repositories.bzl", "rules_js_dependencies")

rules_js_dependencies()

load("@rules_nodejs//nodejs:repositories.bzl", "DEFAULT_NODE_VERSION", "nodejs_register_toolchains")

nodejs_register_toolchains(
    name = "nodejs",
    node_version = DEFAULT_NODE_VERSION,
)

load("@aspect_rules_js//npm:repositories.bzl", "npm_translate_lock")

npm_translate_lock(
    name = "npm",
    pnpm_lock = "//:pnpm-lock.yaml",
    verify_node_modules_ignored = "//:.bazelignore",
)

load("@npm//:repositories.bzl", "npm_repositories")

npm_repositories()

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
