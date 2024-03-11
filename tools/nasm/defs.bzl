def _impl(ctx):
    # out = ctx.actions.declare_file(ctx.attr.out)

    args = ctx.actions.args()

    args.add("-f").add(ctx.attr.format)
    args.add("-o").add(ctx.outputs.out)
    args.add_all(ctx.files.srcs)
    args.add_all(ctx.attr.defines, format_each = "-d%s")

    #nasm = ctx.attr._nasm[DefaultInfo].files.to_list()[0]
    nasm = ctx.executable._nasm

    ctx.actions.run(
        executable = nasm,
        arguments = [args],
        inputs = ctx.files.srcs + ctx.attr._nasm[DefaultInfo].default_runfiles.files.to_list(),
        outputs = [ctx.outputs.out],
    )

    return [
        DefaultInfo(
            files = depset([ctx.outputs.out]),
        ),
    ]

nasm_library = rule(
    implementation = _impl,
    attrs = {
        "format": attr.string(default = "elf", values = ["elf", "bin", "elf64"]),
        "srcs": attr.label_list(allow_files = [".s", ".asm"]),
        "out": attr.output(mandatory = True),
        "defines": attr.string_list(),
        "_nasm": attr.label(default = "@nasm", cfg = "exec", executable = True),
    },
)
