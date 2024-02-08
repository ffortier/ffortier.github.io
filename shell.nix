{ pkgs ? import <nixpkgs> {} }:
let neovim-configured = 
  pkgs.neovim.override {
    configure = {
      customRC = ''
      :set number
      :set relativenumber
      :set expandtab
      :set autoindent
      :set tabstop=4
      :set shiftwidth=4
      :set clipboard=unnamedplus
      '';
      packages.myVimPackage = with pkgs.vimPlugins; {
        start = [
          airline
          nerdtree
          nvim-web-devicons
          nvim-treesitter-parsers.glsl
          nvim-cmp
        ];
      };
    };
  };
in
let tmux-configured =
  pkgs.tmux.override {
    extraConfig = ''
set -g mouse on
    '';
  };
in
pkgs.mkShell {
  name = "dev";
  buildInputs = with pkgs; [
    git
    tmux
    tree
    bazelisk
    buildifier
    buildozer
    neovim-configured
    nodejs_18
    bazel-watcher
    nodePackages.pnpm
    jdk21
  ];
  shellHook = 
  ''
    bazel() {
      bazelisk $@
    }
    export -f bazel
  '';
}
