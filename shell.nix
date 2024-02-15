let
  # pin revisions and checksums
  nixpkgs_rev = "01885a071465e223f8f68971f864b15829988504"; # nixos-22.11 on 2024-02-15
  nixpkgs_sah256 = "sha256:0nkkakd5w6xmyv61sbh9wdmv15m451n046zgnymcvv5wcnpdzcy8";

  nixpkgs-esp-dev_rev = "2a747222404c2b3e858813da8e1f16acd903a4d6"; # master on 2024-02-15
  nixpkgs-esp-dev_sha256 = "sha256:1x7s3wqxiby1dp2qm8skdcvpcsi5i4sfvjkknmiy80399z1iwvha";

  esp-idf_rev = "482a8fb2d78e3b58eb21b26da8a5bedf90623213"; # v5.1.2 on 2024-02-15
  esp-idf_sha256 = "sha256-uEf3/3NPH+E39VgQ02AbxTG7nmG5bQlhwk/WcTeAUfg=";

  # define package sources
  nixpkgs_src = (builtins.fetchTarball {
    url = "https://github.com/NixOS/nixpkgs/archive/${nixpkgs_rev}.tar.gz";
    sha256 = nixpkgs_sah256;
  });

  nixpkgs-esp-dev_src = (builtins.fetchTarball {
    url = "https://github.com/mirrexagon/nixpkgs-esp-dev/archive/${nixpkgs-esp-dev_rev}.tar.gz";
    sha256 = nixpkgs-esp-dev_sha256;
  });

  # get package repo
  pkgs = import nixpkgs_src {
    overlays = [
      (import "${nixpkgs-esp-dev_src}/overlay.nix")
    ];
  };
in

with pkgs;

mkShell {
  name = "esp-dev-shell";

  buildInputs = [
    (esp-idf-esp32s2.override {
      rev = esp-idf_rev;
      sha256 = esp-idf_sha256;
    })
  ];
}

