def uxn5_format_js(name, rom, out):
    native.genrule(
        name = name,
        srcs = [rom],
        outs = [out],
        cmd = "$(execpath @uxn5//:format_js) $< > $@",
        tools = ["@uxn5//:format_js"],
    )
