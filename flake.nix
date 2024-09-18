{
  inputs = {
    nixpkgs = {
      url = "github:nixos/nixpkgs/nixpkgs-unstable";
    };
    flake-utils = {
      url = "github:numtide/flake-utils";
    };
  };
  outputs = { nixpkgs, flake-utils, ... }: flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs {
        inherit system;
      };
      klc = (with pkgs; stdenv.mkDerivation rec{
        pname = "klc";
        version = "0.0.1";
        src = ./.;

        buildPhase = "gcc main.c -W -Wall -Wextra -pedantic -lX11";
        installPhase = ''
          mkdir -p $out/bin
          find $TMP -type d -maxdepth 1 | grep source | xargs -I{} cp {}/a.out $out/bin/${pname}
        '';

        buildInputs = [
          xorg.libX11
          # xorg.libX11
          # xorg.libX11.dev
          # xorg.libXrandr
          # xorg.libXinerama
          # xorg.libXcursor
          # xorg.libXi
          # xorg.libXext

          # xorg.libxkbfile
          # xorg.xkbutils
          # xorg.xkbevd
          # xorg.xkbcomp
        ];
        # propagatedBuildInputs = [
        #   libGL
        # ];
      }
      );
    in
    rec {
      defaultApp = flake-utils.lib.mkApp {
        drv = defaultPackage;
      };
      defaultPackage = klc;
      devShell = pkgs.mkShell {
        packages = [ pkgs.gcc ];

        inputsFrom = [ klc ];
      };
    }
  );
}

