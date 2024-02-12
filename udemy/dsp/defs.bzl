def _impl(ctx):
    img_file = ctx.actions.declare_file(ctx.label.name + "." + ctx.attr.format)

    script_file = None

    if ctx.attr.script_content:
        script_file = ctx.actions.declare_file(ctx.label.name + ".gdi")

        ctx.actions.write(
            output = script_file,
            content = "\n".join([
                "set terminal " + ctx.attr.format,
            ] + [
                ctx.expand_location(line, ctx.attr.data)
                for line in ctx.attr.script_content
            ]),
        )

    if not script_file:
        fail("Expected script")

    ctx.actions.run_shell(
        outputs = [img_file],
        inputs = [script_file] + ctx.files.data,
        tools = [ctx.executable._tool],
        command = "%s -c %s > %s" % (ctx.executable._tool.path, script_file.path, img_file.path),
    )

    return [
        DefaultInfo(files = depset([img_file])),
    ]

def _open_impl(ctx):
    open_file = ctx.actions.declare_file(ctx.label.name + ".sh")

    ctx.actions.write(
        output = open_file,
        is_executable = True,
        content = "open " + ctx.file.img.short_path,
    )

    return [
        DefaultInfo(
            runfiles = ctx.runfiles(files = [ctx.file.img, open_file]),
            executable = open_file,
        ),
    ]

_gnuplot_img = rule(
    implementation = _impl,
    attrs = {
        "format": attr.string(
            default = "png",
            values = ["png", "svg"],
        ),
        "script_content": attr.string_list(
        ),
        "data": attr.label_list(
            allow_files = True,
        ),
        "_tool": attr.label(
            executable = True,
            cfg = "exec",
            default = "@gnuplot//:bin/gnuplot",
        ),
    },
)

_gnuplot_img_open = rule(
    implementation = _open_impl,
    executable = True,
    attrs = {
        "img": attr.label(
            mandatory = True,
            allow_single_file = True,
        ),
    },
)

def gnuplot_img(name, **kwargs):
    _gnuplot_img(
        name = name,
        **kwargs
    )

    _gnuplot_img_open(
        name = name + ".open",
        img = ":" + name,
        tags = ["manual"],
    )
