{
  description = "Build and develop the 'snip' editor.";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShell = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
          buildInputs = with pkgs; [
            gdb
            clang-tools
            cmake
            cppcheck
            gtest
          ];
        };
      }
    );
}
