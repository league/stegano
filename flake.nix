{
  inputs.nixpkgs.url = "nixpkgs/release-22.05";

  outputs = { nixpkgs, ... }:
    let
      inherit (nixpkgs) lib;
      each = lib.genAttrs [ "x86_64-linux" "x86_64-darwin" "aarch64-darwin" ];
    in {
      packages = each (system: {
        default = nixpkgs.legacyPackages.${system}.callPackage
          ({ stdenv, graphicsmagick, libwebp }:
            stdenv.mkDerivation {
              name = "stegano";
              src = ./.;
              buildInputs = [ graphicsmagick libwebp ];
              makeFlags = [ "PREFIX=$(out)" ];
            }) { };
      });
    };
}
