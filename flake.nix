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

        sourcePaths = "snip snip-core snip-runtime snip-editor snip-ui";

        fmt = pkgs.writeShellApplication {
          name = "snip-fmt";
          meta.description = "Format C++ sources with clang-format";
          runtimeInputs = with pkgs; [
            clang-tools
            findutils
          ];
          text = ''
            set -euo pipefail

            mapfile -t files < <(find ${sourcePaths} -type f \( -name '*.cpp' -o -name '*.hpp' \) | sort)

            if [ "''${#files[@]}" -eq 0 ]; then
              echo "No C++ source files found."
              exit 0
            fi

            clang-format -i "''${files[@]}"
            echo "Formatted ''${#files[@]} file(s)."
          '';
        };

        lint = pkgs.writeShellApplication {
          name = "snip-lint";
          meta.description = "Run cppcheck linting over project sources";
          runtimeInputs = with pkgs; [
            cppcheck
          ];
          text = ''
            set -euo pipefail

            cppcheck \
              --std=c++23 \
              --language=c++ \
              --enable=warning,style,performance,portability \
              --check-level=exhaustive \
              --inline-suppr \
              --error-exitcode=1 \
              --quiet \
              ${sourcePaths}

            echo "cppcheck passed."
          '';
        };

        # tidy = pkgs.writeShellApplication {
        #   name = "snip-tidy";
        #   meta.description = "Run clang-tidy using generated compile_commands.json";
        #   runtimeInputs = with pkgs; [
        #     clang-tools
        #     cmake
        #     findutils
        #     ninja
        #   ];
        #   text = ''
        #     set -euo pipefail

        #     build_dir="$(mktemp -d)"
        #     trap 'rm -rf "$build_dir"' EXIT

        #     cmake -S . -B "$build_dir" -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

        #     mapfile -t cpp_files < <(find ${sourcePaths} -type f -name '*.cpp' | sort)

        #     if [ "''${#cpp_files[@]}" -eq 0 ]; then
        #       echo "No C++ source files found."
        #       exit 0
        #     fi

        #     clang-tidy -p "$build_dir" "''${cpp_files[@]}"
        #     echo "clang-tidy completed for ''${#cpp_files[@]} translation unit(s)."
        #   '';
        # };

        qa = pkgs.writeShellApplication {
          name = "snip-qa";
          meta.description = "Run formatter, cppcheck lint, and clang-tidy";
          runtimeInputs = [
            fmt
            lint
            # tidy
          ];
          text = ''
            set -euo pipefail

            snip-fmt
            snip-lint
            # snip-tidy
          '';
        };
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
        };

        # apps = {
        #   fmt = flake-utils.lib.mkApp { drv = fmt; };
        #   fmt-check = flake-utils.lib.mkApp { drv = fmtCheck; };
        #   lint = flake-utils.lib.mkApp { drv = lint; };
        #   tidy = flake-utils.lib.mkApp { drv = tidy; };
        #   qa = flake-utils.lib.mkApp { drv = qa; };
        # };

        checks = {
          format =
            pkgs.runCommand "snip-format"
              {
                src = ./.;
                nativeBuildInputs = [ fmt ];
              }
              ''
                cp -r "$src" source
                chmod -R +w source
                cd source
                snip-fmt
                touch "$out"
              '';

          lint =
            pkgs.runCommand "snip-lint"
              {
                src = ./.;
                nativeBuildInputs = [ lint ];
              }
              ''
                cp -r "$src" source
                chmod -R +w source
                cd source
                snip-lint
                touch "$out"
              '';

          # tidy =
          #   pkgs.runCommand "snip-tidy"
          #     {
          #       src = ./.;
          #       nativeBuildInputs = [ tidy ];
          #     }
          #     ''
          #       cp -r "$src" source
          #       chmod -R +w source
          #       cd source
          #       snip-tidy
          #       touch "$out"
          #     '';
        };

        formatter = pkgs.nixfmt;

        devShells.default = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
          buildInputs = with pkgs; [
            clang-tools
            cmake
            cppcheck
            prek
            ninja
          ];
        };
      }
    );
}
