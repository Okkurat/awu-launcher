# NOTE

This is work in process and the application is not feature complete. Games can be launched and winetricks and winecfg can be used, but no config files can be created though the GUI. More on the config files

# What is this?

![app image](img/app.png)

This is my simplistic Qt6 frontend for umu-launcher (previously named ULWGL) that can use Valves Proton to launch Windows game on Linux. The name is a subject to change.

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
you are free to modify the install location if you want.

The Umu-launcher has documentation on the config files. In the /.config/awu/umu-conf directory, you should place those config files and the GUI automatically uses them There is also /.config/awu/proton directory that you can use to place your version of proton.

This is an example config files
```shell
# config.toml
[umu]
prefix = "/path/to/your/prefix"
proton = "/path/to/your/proton"
game_id = "0"
exe = "/path/to/your/game/file"
launch_args = ["-opengl", "-SkipBuildPatchPrereq"]
store = "none"
name = "The game"
awu_args = "gamescope -w 1920 -h 1080 -- mangohud"
```

Only things are necessarily needed are prefix, exe, awu_args, name and proton. Name and awu_args are lated to Awu launcher. Name is the name of the game and awu_args is used for programs like gamescoe and mangohud.
