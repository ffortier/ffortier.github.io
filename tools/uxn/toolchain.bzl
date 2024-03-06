def _impl(ctx):
    return [
        platform_common.ToolchainInfo(
            assembler = ctx.executable.assembler,
        ),
    ]

uxn_toolchain = rule(
    implementation = _impl,
    attrs = {
        "assembler": attr.label(executable = True, mandatory = True, cfg = "exec"),
    },
)
