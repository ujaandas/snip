{
  description = "Build and develop the 'parity' editor.";
 
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

        qtPkgs = with pkgs; [
          qt6.qtbase
          qt6.qtbase.dev
          qt6.qtdeclarative
          qt6.qtdeclarative.dev
        ];

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
            export QML_DISABLE_DISK_CACHE=1
            cmake -S . -B .nix-dev/build -G Ninja
          '';
        };

        dev-test = pkgs.writeShellApplication {
          name = "dev-test";
          meta.description = "Run test suite.";
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
            export QML_DISABLE_DISK_CACHE=1
            cmake -S . -B .nix-dev/build
            cmake --build .nix-dev/build
            ctest --test-dir .nix-dev/build --output-on-failure
          '';
        };

        dev-clang-tidy = pkgs.writeShellApplication {
          name = "dev-clang-tidy";
          meta.description = "Run clang-tidy with Qt include roots.";
          runtimeInputs =
            with pkgs;
            [
              clang-tools
            ]
            ++ qtPkgs;
          text = ''
            set -euo pipefail
            clang-tidy \
              --extra-arg=-isystem \
              --extra-arg=${pkgs.qt6.qtbase.dev}/include \
              --extra-arg=-isystem \
              --extra-arg=${pkgs.qt6.qtdeclarative.dev}/include \
              "$@"
          '';
        };

        parity = pkgs.clangStdenv.mkDerivation {
          pname = "parity";
          version = "0.1.0";
          src = ./.;
          meta = {
            mainProgram = "parity";
            description = "A badonkers-fast, Nix-first editor for gigachads.";
          };

          nativeBuildInputs = with pkgs; [
            cmake
            ninja
            qt6.wrapQtAppsHook
          ];

          qtWrapperArgs = [
            "--set"
            "QML_DISABLE_DISK_CACHE"
            "1"
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
        packages.default = parity;

        checks = {
          default = parity;
        };

        apps = {
          default = (flake-utils.lib.mkApp { drv = parity; }) // {
            meta.description = "Run the default parity executable.";
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

          clang-tidy = {
            type = "app";
            program = "${dev-clang-tidy}/bin/dev-clang-tidy";
            meta.description = "Run clang-tidy with Qt include roots.";
          };

        };

        formatter = pkgs.nixfmt;

        devShells.default = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
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

          shellHook = ''
            export QML_DISABLE_DISK_CACHE=1
          '';
        };
      }
    );
}
