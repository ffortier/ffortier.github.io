load("@aspect_rules_js//npm:repositories.bzl", "npm_translate_lock")

def scratch_repositories():
    npm_translate_lock(
        name = "scratch_npm",
        pnpm_lock = "//:pnpm-lock.yaml",
        verify_node_modules_ignored = "//:.bazelignore",
    )
