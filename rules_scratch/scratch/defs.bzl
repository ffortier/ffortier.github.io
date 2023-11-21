load(":private/providers.bzl", _ScratchExtensionInfo = "ScratchExtensionInfo", _ScratchInfo = "ScratchInfo")
load(":private/scratch_binary.bzl", _scratch_binary = "scratch_binary")
load(":private/scratch_extension.bzl", _scratch_extension = "scratch_extension")
load(":private/scratch_library.bzl", _scratch_library = "scratch_library")

scratch_binary = _scratch_binary
scratch_library = _scratch_library
scratch_extension = _scratch_extension
ScratchInfo = _ScratchInfo
ScratchExtensionInfo = _ScratchExtensionInfo
