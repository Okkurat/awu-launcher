# NOTE

This is work in process and the application is not ready yet. Games can be launched and winetricks and winecfg can be used, but no config files can be created.

# What is this?

This is my simplistic Qt5 frontend for umu-launcher (previously named ULWGL) that can use Valves Proton to launch Windows game on Linux. The name is a subject to change.

This launcher is a hobby project and for personal use. You are free to expand or use the code however you want.

Umu-launcher needs to be installed on your system and must be in your path
https://github.com/Open-Wine-Components/umu-launcher
In the directory run
```shell
cd build
```
Then run Cmake
```shell
cmake ..
```
To make executable in the build directory
```shell
make
```
To install it to /.local/bin

```shell
make install
```
