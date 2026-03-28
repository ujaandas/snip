{
  description = "Build and develop the 'snip' editor.";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
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
            cmake
            ninja
          ];

          installPhase = ''
            runHook preInstall

            cmake --install . --prefix "$out"

            mkdir -p $out
            cp compile_commands.json $out/compile_commands.json

            runHook postInstall
          '';
        };

        devShell = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
          buildInputs = with pkgs; [
            clang-tools
            cmake
            prek
            ninja
          ];
        };
      }
    );
}
