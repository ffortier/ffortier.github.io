{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  name = "dev";
  buildInputs = with pkgs; [
    git
    tree
    bazelisk
    buildifier
    buildozer
    nodejs_18
    bazel-watcher
    nodePackages.pnpm
    jdk21
    qemu
    gnuplot
  ];
  shellHook = 
  ''
bazel() {
  bazelisk $@
}
export -f bazel

NO_NIX_PATH=$(awk '!/^\/nix/ {x=(NR==1?x:x":") $0} END {print x}' RS=':' <<< "$PATH")
export BAZEL_SH=$(env PATH="$NO_NIX_PATH" which bash)
unset NO_NIX_PATH
  '';
}
