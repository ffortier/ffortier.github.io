# def _impl(ctx):
#     toolchain = ctx.toolchains["@bazel_tools//tools/cpp:toolchain_type"]

#     print(toolchain)

# wasm_binary = rule(
#     implementation = _impl,
#     toolchains = ["@bazel_tools//tools/cpp:toolchain_type"],
#     attrs = {
#         "srcs": attr.label_list(
#             allow_files = [".c", ".h"],
#             allow_empty = False,
#             mandatory = True,
#         ),
#         "out": attr.output(),
#     },
# )
load("@rules_cc//cc:defs.bzl", "cc_binary")

def wasm_binary(name, srcs):
    cc_binary(
        copts = ["-nostdlib"],
        linkopts = ["-Wl,--export-all", "-Wl,--no-entry", "--target=wasm32-unknown-unknown"],
        name = name,
        srcs = srcs,
    )
