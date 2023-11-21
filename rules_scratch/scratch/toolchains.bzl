load("@scratch_npm//:repositories.bzl", "npm_repositories")

def scratch_register_toolchain():
    npm_repositories()
