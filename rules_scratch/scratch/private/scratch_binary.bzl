load(":private/providers.bzl", "ScratchInfo")

def _impl(ctx):
    sb3_file = ctx.actions.declare_file(ctx.label.name + ".sb3")
    exec_file = ctx.actions.declare_file(ctx.label.name)

    args = ctx.actions.args()

    args.add("-o").add(sb3_file)

    if ctx.file.project_json:
        args.add("-p").add(ctx.file.project_json)

    args.add_all(ctx.files.deps, before_each = "-d")
    args.add_all(ctx.files.assets, before_each = "-a")

    ctx.actions.run(
        executable = ctx.executable._sb3c,
        inputs = ctx.files.project_json + ctx.files.assets + ctx.files.deps,
        outputs = [sb3_file],
        arguments = [args],
    )

    ctx.actions.write(
        output = exec_file,
        content = "ls " + sb3_file.short_path,
        is_executable = True,
    )

    return [
        DefaultInfo(
            files = depset([sb3_file]),
            runfiles = ctx.runfiles(files = [exec_file, sb3_file]),
            executable = exec_file,
        ),
    ]

scratch_binary = rule(
    implementation = _impl,
    executable = True,
    attrs = {
        "srcs": attr.label_list(
            doc = "Stage and Sprite target definitions as specified here: https://github.com/scratchfoundation/scratch-parser/blob/master/lib/sb3_schema.json#/properties/targets",
            allow_files = [".json"],
        ),
        "assets": attr.label_list(
            doc = "External assets like images and sound",
            allow_files = True,
        ),
        "deps": attr.label_list(
            doc = "Other scratch libraries to merge with this definition",
            providers = [ScratchInfo],
        ),
        "project_json": attr.label(
            doc = "Other scratch libraries to merge with this definition",
            allow_single_file = [".json"],
        ),
        "_sb3c": attr.label(
            executable = True,
            default = "//tools/sb3c",
            cfg = "exec",
        ),
    },
)
