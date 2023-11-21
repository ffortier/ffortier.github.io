load(":private/providers.bzl", "ScratchExtensionInfo")

def _impl(ctx):
    return [
        ScratchExtensionInfo(definition = ctx.file.definition),
    ]

scratch_extension = rule(
    implementation = _impl,
    attrs = {
        "definition": attr.label(
            allow_single_file = [".json"],
            mandatory = True,
        ),
    },
)
