# Installation

The binaries export symbols for both injection and usage as a [Valve Server Plugin](https://developer.valvesoftware.com/wiki/Server_plugins).

1. Copy or link the binaries in the root folder of the game, where the executable is located.
2. Make (not link) the folder structure `supero/lua` which contains the main script (can be linked) and its dependencies.

   You can use the premade folder from releases.

   These steps need to be done if you don't want to change the default script and binary locations.

Depending on the usage method, see the next steps below :

## Injection

1. On Linux you can run `script.sh` from `src/scripts`. Needs root and gdb. Modify the file path or run it in the same folder as the binary.
2. Windows does not export DLLMain. You're on your own or run as a VSP, which works on client too.

## Valve Server Plugin

1. Copy `supero_x64.vdf` and the platform specific binaries in `\<gamefolder\>/addons`.

# Considerations

It might crash, not work properly or have bugs.
This has really been tested mostly on Linux.

# Compilation

Uses Zig. Do not use debug builds for the executable since linking is broken.

# Credits

* [Amalgam](https://github.com/rei-2/Amalgam)
* [8dcc's TF2 Linux internal cheat](https://github.com/8dcc/tf2-cheat)
* [libmem](https://github.com/rdbo/libmem)
* [libdetour](https://github.com/8dcc/libdetour)
