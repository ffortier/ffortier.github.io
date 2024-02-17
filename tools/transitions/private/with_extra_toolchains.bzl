def _transition_impl(_, attr):
    labels = [str(label).removeprefix("@@") for label in attr.extra_toolchains]

    return {"//command_line_option:extra_toolchains": ",".join(labels)}

extra_toolchains_transition = transition(
    implementation = _transition_impl,
    inputs = [],
    outputs = ["//command_line_option:extra_toolchains"],
)

def _rule_impl(ctx):
    return [
        DefaultInfo(
            files = depset([ctx.executable.binary]),
        ),
    ]

with_extra_toolchains = rule(
    implementation = _rule_impl,
    attrs = {
        "binary": attr.label(
            executable = True,
            cfg = extra_toolchains_transition,
            mandatory = True,
        ),
        "extra_toolchains": attr.label_list(
            mandatory = True,
            allow_empty = False,
        ),
    },
)
