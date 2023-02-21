# Funchook

This project relies on the [Funchook Library](https://github.com/kubo/funchook) to hook onto the existing UE4 and UT4 libraries.


## Building

Pre-compiled libraries exist under the `Lib/` directory. You can build this yourself by generating the VS2015 project and building the `funchook-static` project in VS. Note: You can use the latest Visual Studio Community edition so long as you have the VS2015 toolset installed.

```console
$ mkdir build
$ cd build
$ cmake -G 'Visual Studio 14 2015' -A x64 ../funchook-1.1.1
```