# Adjust_TEST

External Dependency used: libsodium-1.0.17

Explaination:

     This is basic implementation of consuming generic hashing using
     libsodium(https://dl.google.com/android/repository/android-ndk-r18b-linux-x86_64.zip).
     Multiple types supported by libsodium generic hashing are used in this code.
     1) Single Part without Key.
     2) Single Part with Key.
     3) Multipart without Key.
     This is a binary and not an app.

Usage:

     Checkout this code from: (https://github.com/saherkar/Adjust_TEST.git)
     To use this library following are the environment variable that need to be set.
             - Export NDK to installed path of android-ndk(android-ndk-r18b is used in this code).
             - Export TOOLCHAIN to directory where the executable will be deployed /root/x86.
             - Export LIBSODIUM to directoru of libsodium(libsodium-1.0.17 is used in this code).

     After setting these, go to checked out folder, run `make` and your binary is ready to use.
     Push the binary on either emulator or your device, attach to shell(emulator/device) using adb(Android tools)
     and execute it.
