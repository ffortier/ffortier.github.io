ScratchInfo = provider(
    doc = "Contains information about a scratch artifact",
    fields = {
        "sb3a": "Static library",
    },
)

ScratchExtensionInfo = provider(
    "A Scratch extension",
    fields = {
        "definition": "JSON file containing the extension definition",
    },
)
