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

        qtPkgs = with pkgs; [ qt6.qtbase ];

        dev-configure = pkgs.writeShellApplication {
          name = "dev-configure";
          meta.description = "Configure clangd environment.";
          runtimeInputs =
            with pkgs;
            [
              clang
              cmake
              ninja
              gtest
            ]
            ++ qtPkgs;
          text = ''
            set -euo pipefail
            cmake -S . -B .nix-dev/build -G Ninja
          '';
        };

        dev-test = pkgs.writeShellApplication {
          name = "dev-test";
          meta.description = "Run test suite.";
          runtimeInputs = with pkgs; [
            clang
            cmake
            ninja
            gtest
            qt
          ];
          text = ''
            set -euo pipefail
            cmake -S . -B .nix-dev/build
            cmake --build .nix-dev/build
            ctest --test-dir .nix-dev/build --output-on-failure
          '';
        };

        snip = pkgs.clangStdenv.mkDerivation {
          pname = "snip";
          version = "0.1.0";
          src = ./.;
          meta = {
            mainProgram = "snip";
            description = "A badonkers-fast, Nix-first editor for gigachads.";
          };

          nativeBuildInputs = with pkgs; [
            cmake
            ninja
            qt6.wrapQtAppsHook
          ];

          buildInputs =
            with pkgs;
            [
              gtest
            ]
            ++ qtPkgs;

          doCheck = true;
          checkPhase = ''
            ctest --output-on-failure
          '';
        };

      in
      {
        packages.default = snip;

        checks = {
          default = snip;
        };

        apps = {
          default = (flake-utils.lib.mkApp { drv = snip; }) // {
            meta.description = "Run the default snip executable.";
          };

          configure = {
            type = "app";
            program = "${dev-configure}/bin/dev-configure";
            meta.description = "Configure the local CMake build directory.";
          };

          test = {
            type = "app";
            program = "${dev-test}/bin/dev-test";
            meta.description = "Run test suites.";
          };
        };

        formatter = pkgs.nixfmt;

        devShells.default = pkgs.mkShell {
          buildInputs =
            with pkgs;
            [
              clang-tools
              cmake
              cppcheck
              prek
              ninja
              gtest
            ]
            ++ qtPkgs;
        };
      }
    );
}
