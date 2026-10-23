{
  description = "sen build";
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-22.11";
    nixpkgs-tools.url = "github:NixOS/nixpkgs/nixos-unstable";
  };
  outputs =
    {
      self,
      nixpkgs,
      nixpkgs-tools,
    }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
      tools = import nixpkgs-tools { inherit system; };

      senVersion = "0.7.0";

      buildInputs = [
        pkgs.libGL
        pkgs.libGLU
        pkgs.xorg.libX11
        pkgs.xorg.libXext
        pkgs.xorg.libXrandr
        pkgs.xorg.libXinerama
        pkgs.xorg.libXcursor
        pkgs.xorg.libXi
        pkgs.xorg.libXxf86vm
        pkgs.wayland
        pkgs.libxkbcommon
        pkgs.libiconv
      ];

      nativeBuildInputs = [
        pkgs.gcc12
        pkgs.binutils
        pkgs.cmake
        pkgs.pkg-config
        pkgs.python3
        pkgs.perl
        pkgs.git
        pkgs.patchelf
        tools.conan
      ];

      # Clean the staged tree instead, after `cmake --install` and before the
      # archive is built, via CPack's pre-build hook. This also rewrites the ELF
      # interpreter to the system loader
      cpackFixup = pkgs.writeShellScript "sen-cpack-fixup" ''
        set -eu
        export PATH="${pkgs.patchelf}/bin:${pkgs.findutils}/bin:${pkgs.coreutils}/bin:$PATH"

        for dir in "$@"; do
          [ -n "$dir" ] && [ -d "$dir" ] || continue
          while IFS= read -r -d "" f; do
            # Not an ELF we can touch -> skip.
            patchelf --print-rpath "$f" >/dev/null 2>&1 || continue

            if patchelf --print-interpreter "$f" >/dev/null 2>&1; then
              patchelf --set-interpreter /lib64/ld-linux-x86-64.so.2 "$f" || true
            fi

            old="$(patchelf --print-rpath "$f" 2>/dev/null || true)"
            new=""
            while IFS= read -r part; do
              case "$part" in
                /nix/store/*|"") ;;
                *) new="''${new:+$new:}$part" ;;
              esac
            done < <(printf '%s' "$old" | tr ':' '\n')

            [ "$old" = "$new" ] && continue
            if [ -n "$new" ]; then
              patchelf --set-rpath "$new" "$f" || true
            else
              patchelf --remove-rpath "$f" || true
            fi
          done < <(find "$dir" -type f \( -executable -o -name '*.so*' \) -print0)
        done
      '';

      cpackFixupScript = pkgs.writeText "sen-cpack-fixup.cmake" ''
        execute_process(
          COMMAND ${cpackFixup}
                  "''${CPACK_TEMPORARY_DIRECTORY}"
                  "''${CPACK_TEMPORARY_INSTALL_DIRECTORY}"
          RESULT_VARIABLE _sen_fixup_result
        )
        if(NOT _sen_fixup_result EQUAL 0)
          message(FATAL_ERROR "sen cpack fixup failed: ''${_sen_fixup_result}")
        endif()
      '';

      pkgConfigPath = pkgs.lib.makeSearchPathOutput "dev" "lib/pkgconfig" [
        pkgs.libGL
        pkgs.libGLU
        pkgs.xorg.libX11
        pkgs.xorg.libXext
        pkgs.xorg.libXrandr
        pkgs.xorg.libXinerama
        pkgs.xorg.libXcursor
        pkgs.xorg.libXi
        pkgs.xorg.libXxf86vm
        pkgs.wayland
        pkgs.libxkbcommon
      ];
    in
    {
      packages.${system} = rec {
        sen = pkgs.stdenv.mkDerivation {
          pname = "sen";
          version = senVersion;
          src = ./.;

          inherit buildInputs nativeBuildInputs;

          # Conan downloads dependencies from network, which the Nix sandbox
          # blocks by default. __noChroot disables the sandbox.
          __noChroot = true;

          CC = "${pkgs.gcc12}/bin/gcc";
          CXX = "${pkgs.gcc12}/bin/g++";

          PKG_CONFIG_PATH = pkgConfigPath;
          GIT_LFS_SKIP_SMUDGE = "1";
          SEN_VERSION = senVersion;

          configurePhase = ''
            runHook preConfigure

            export HOME=$TMPDIR/conan-home
            mkdir -p $HOME/.conan2/profiles
            cp ./.conan/profiles/sen_gcc $HOME/.conan2/profiles/sen_gcc

            BUILD_PROFILE=sen_gcc

            conan install ./conanfile.py \
              --build=missing \
              -s:a build_type=Release \
              -pr:b=$BUILD_PROFILE \
              -pr:h=$BUILD_PROFILE \
              -o "&:mode=basic" \
              -o "&:with_examples=False" \
              -o "&:with_tests=False" \
              -o "&:with_docs=False" \
              -c tools.system.package_manager:mode=report \
              -c:a tools.build:compiler_executables='{"c":"${pkgs.gcc12}/bin/gcc","cpp":"${pkgs.gcc12}/bin/g++"}'

            runHook postConfigure
          '';

          buildPhase = ''
            runHook preBuild
            conan build ./conanfile.py \
              --build=missing \
              -s:a build_type=Release \
              -pr:b=sen_gcc \
              -pr:h=sen_gcc \
              -o "&:mode=basic" \
              -o "&:with_examples=False" \
              -o "&:with_tests=False" \
              -o "&:with_docs=False" \
              -c tools.system.package_manager:mode=report \
              -c:a tools.build:compiler_executables='{"c":"${pkgs.gcc12}/bin/gcc","cpp":"${pkgs.gcc12}/bin/g++"}'

            runHook postBuild
          '';

          installPhase = ''
            runHook preInstall
            cmake --install build/gcc/Release --prefix $out
            runHook postInstall
          '';

          # Tests use a separate `run_tests` CMake target; enable if you want
          # `nix flake check` to run them.
          doCheck = false;
          checkPhase = ''
            runHook preCheck
            cmake --build build/gcc/Release --target run_tests
            runHook postCheck
          '';
        };

        # nix build .#rpm
        rpm = sen.overrideAttrs (old: {
          pname = "sen-rpm";
          nativeBuildInputs = old.nativeBuildInputs ++ [ pkgs.rpm ];

          # Stdenv would otherwise re-inject /nix/store RPATHs and keep the
          # nix-store ld-linux as the interpreter, which makes the RPM
          # unusable outside Nix.
          dontPatchELF = true;

          installPhase = ''
            runHook preInstall
            # The interpreter and RPATH are fixed up on the staged tree by
            # CPACK_PRE_BUILD_SCRIPTS, after cmake --install has run.
            cd build/gcc/Release
            cpack -G RPM \
            -D CPACK_PRE_BUILD_SCRIPTS=${cpackFixupScript} \
            -D CPACK_PACKAGING_INSTALL_PREFIX=/opt/sen \
            -D CPACK_COMPONENTS_ALL='Unspecified;rpm' \
            -D CPACK_PACKAGE_CONTACT="juan@bitomia.com" \
            -D CPACK_PACKAGE_RELOCATABLE=0
            mkdir -p $out
            cp *.rpm $out/
            runHook postInstall
          '';

          # $out only contains .rpm archives here; the patchelf above already
          # ran against the build tree before cpack consumed it.
          postFixup = "";
        });

        # nix build .#deb
        deb = sen.overrideAttrs (old: {
          pname = "sen-deb";
          nativeBuildInputs = old.nativeBuildInputs ++ [ pkgs.dpkg ];

          # Stdenv would otherwise re-inject /nix/store RPATHs and keep the
          # nix-store ld-linux as the interpreter, which makes the DEB
          # unusable outside Nix.
          dontPatchELF = true;

          installPhase = ''
            runHook preInstall
            # The interpreter and RPATH are fixed up on the staged tree by
            # CPACK_PRE_BUILD_SCRIPTS, after cmake --install has run.
            cd build/gcc/Release
            cpack -G DEB \
            -D CPACK_PRE_BUILD_SCRIPTS=${cpackFixupScript} \
            -D CPACK_PACKAGING_INSTALL_PREFIX=/opt/sen \
            -D CPACK_PACKAGE_CONTACT="juan@bitomia.com" \
            -D CPACK_PACKAGE_RELOCATABLE=0
            mkdir -p $out
            cp *.deb $out/
            runHook postInstall
          '';

          # $out only contains .deb archives here; the patchelf above already
          # ran against the build tree before cpack consumed it.
          postFixup = "";
        });

        default = pkgs.symlinkJoin {
          name = "sen-all-${senVersion}";
          paths = [
            sen
            (pkgs.runCommand "sen-rpm-nested" { } ''
              mkdir -p $out/rpm
              cp -r ${rpm}/. $out/rpm/
            '')
          ];
        };
      };

      devShells.${system}.default = pkgs.mkShell {
        packages =
          nativeBuildInputs
          ++ buildInputs
          ++ [
            pkgs.glibcLocales
            pkgs.bashInteractive
            tools.mise
          ];
        shellHook = ''
          unset LD_LIBRARY_PATH
          export LOCALE_ARCHIVE=${pkgs.glibcLocales}/lib/locale/locale-archive
          export LANG=C.utf8
          export LC_ALL=C.utf8
          export PKG_CONFIG_PATH="${pkgConfigPath}:$PKG_CONFIG_PATH"
          mise install
          eval "$(mise env -s bash)"
        '';
      };
    };
}
