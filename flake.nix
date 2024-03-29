{
  description = "The Replit RTLD Loader allows dynamically loaded shared libraries (.so) to work seamlessly in Repls";
  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-23.11";

  outputs = { self, nixpkgs, ... }:
    let pkgs = import nixpkgs {
      system = "x86_64-linux";
    };
    package = pkgs.stdenv.mkDerivation {
      pname = "replit_rtld_loader";
      version = "1";
      src = ./.;
      buildInputs = [pkgs.python310];
      installPhase = ''
        mkdir $out
        mv rtld_loader.so $out/
      '';
    };
    in
    {
      packages.x86_64-linux.default = package;
      devShells.x86_64-linux.default = pkgs.mkShell {
        packages = with pkgs; [
          python310
          gnumake
        ];
      };
      overlays.default = final: prev: {
        replit-rtld-loader = package;
      };
    };
}
