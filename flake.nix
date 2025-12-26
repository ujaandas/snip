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
        packages.default = pkgs.stdenv.mkDerivation {
          pname = "snip";
          version = "0.1.0";
          src = ./.;

          nativeBuildInputs = with pkgs; [
            gnumake
            "g++"
          ];

          installPhase = ''
            mkdir -p $out/bin
            cp snip $out/bin/
          '';
        };

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
