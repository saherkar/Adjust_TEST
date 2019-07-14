# Usage:
# make          # compile all binary
# make clean  # remove ALL binaries and objects
.PHONY = all clean

export PATH:=$(PATH):$(TOOLCHAIN)/bin/
export CC:=i686-linux-android-gcc
export LD:=i686-linux-android-ld
export AR:=i686-linux-android-ar
export ARFLAGS:=cr
export RANLIB:=i686-linux-android-ranlib
export INSTALL_DIR:=/usr/local/libsodium
export PKG_CONFIG_PATH:=/usr/local/libsodium/lib/pkgconfig/

all: build_ndk compile_libsodium hasher

build_ndk:
	$(NDK)/build/tools/make_standalone_toolchain.py --arch x86 --install-dir $(TOOLCHAIN)

compile_libsodium:
	mkdir $(INSTALL_DIR); \
	cd $(LIBSODIUM); wget https://download.libsodium.org/libsodium/releases/libsodium-1.0.17.tar.gz; \
	tar -xvf libsodium-1.0.17.tar.gz; \
	rm -rf libsodium-1.0.17.tar.gz; \
	cd libsodium-1.0.17/ && ./configure --host=x86-none-eabi --prefix=$(INSTALL_DIR) && make install
	export PKG_CONFIG_PATH=$(INSTALL_DIR)/lib/pkgconfig/

hasher:
	$(CC) -o hasher.bin hasher.c `pkg-config --cflags libsodium` `pkg-config --libs libsodium`

clean:
	rm -rf *.o
	cd $(LIBSODIUM)/libsodium-1.0.17/ && make clean
	rm -rf $(LIBSODIUM)/libsodium-1.0.17/
	rm -rf $(TOOLCHAIN)
	rm -rf $(INSTALL_DIR)
