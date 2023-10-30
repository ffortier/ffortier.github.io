load("@rules_jvm_external//:defs.bzl", "maven_install")
load("@rules_jvm_external//:specs.bzl", "maven")

def test_artifact(maven_coordinates):
    group, artifact, version = maven_coordinates.split(":")

    return maven.artifact(
        group = group,
        artifact = artifact,
        version = version,
        testonly = True,
    )

def establish_java_dependencies():
    maven_install(
        artifacts = [
            test_artifact("org.junit.jupiter:junit-jupiter-api:5.10.0"),
            test_artifact("org.junit.jupiter:junit-jupiter-engine:5.10.0"),
            test_artifact("org.junit.platform:junit-platform-launcher:1.10.0"),
            test_artifact("org.junit.platform:junit-platform-reporting:1.10.0"),
            test_artifact("org.assertj:assertj-core:3.24.2"),
        ],
        repositories = [
            "https://repo1.maven.org/maven2",
        ],
        fetch_sources = True,
        fetch_javadoc = True,
        maven_install_json = "//third_party/java:maven_install.json",
    )
