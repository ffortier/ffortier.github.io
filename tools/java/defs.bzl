load("@contrib_rules_jvm//java:defs.bzl", "JUNIT5_DEPS", "java_junit5_test")
load("@rules_java//java:defs.bzl", "java_binary", "java_library")
load("@rules_jvm_external//:defs.bzl", "artifact")

def java_project(name, deps = [], test_deps = [], test_data = [], test_env = [], runtime_deps = [], main_class = None, additional_srcs = [], test_size = "small", **kwargs):
    """
    Java project with standard directory structures

    Sources are found under src/main/java and test sources are found under src/test/java

    Args:
        name: Name of the java_library target
        deps: Dependencies for java_library and java_juint5_test targets
        main_class: if defined the main target becomes a java_binary
        test_deps: Additional test dependencies
        test_size: Test size (default small)
        test_data: Additional test data
        test_env: Test environment variables
        runtime_deps: Additional runtime deps for the java binary if main_class is defined
        additional_srcs: Additional srcs for the java_library
        **kwargs: args forwarded to the java_library (or java_binary if main_class is defined)
    """
    main_target = ":" + name

    if main_class:
        java_library(
            name = name + "_main",
            srcs = native.glob(["src/main/java/**/*.java"]) + additional_srcs,
            deps = deps + [
                artifact("io.soabase.record-builder:record-builder-core"),
            ],
            plugins = [
                "//tools/java:record_builder",
            ],
        )

        java_binary(
            name = name + "bin",
            runtime_deps = [":" + name + "_main"] + runtime_deps,
            main_class = main_class,
            **kwargs
        )

        main_target = ":" + name + "_main"
    else:
        java_library(
            name = name,
            srcs = native.glob(["src/main/java/**/*.java"]) + additional_srcs,
            deps = deps + [
                artifact("io.soabase.record-builder:record-builder-core"),
            ],
            plugins = [
                "//tools/java:record_builder",
            ],
            **kwargs
        )

    for test_file in native.glob(["src/test/java/**/*Test.java"]):
        java_junit5_test(
            name = test_file
                .removeprefix("src/test/java/")
                .removesuffix(".java")
                .replace("/", "."),
            size = test_size,
            srcs = [test_file],
            deps = [main_target] + deps + test_deps + [
                artifact("org.junit.jupiter:junit-jupiter-api"),
                artifact("org.assertj:assertj-core"),
            ],
            runtime_deps = JUNIT5_DEPS,
            data = test_data,
            env = test_env,
        )
