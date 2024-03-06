load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

_toolchain = tag_class(
    attrs = {
        "urls": attr.string_list(),
        "sha256": attr.string(),
        "strip_prefix": attr.string(default = ""),
    },
)

def _impl(mctx):
    mod = mctx.modules[0]
    toolchain = mod.tags.toolchain[0]

    http_archive(
        name = "uxn_sources",
        urls = toolchain.urls,
        strip_prefix = toolchain.strip_prefix,
        sha256 = toolchain.sha256,
        build_file = ":BUILD.uxn.bazel",
    )

uxn = module_extension(
    implementation = _impl,
    tag_classes = {
        "toolchain": _toolchain,
    },
)
