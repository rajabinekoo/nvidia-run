# run-nvidia

A tiny native Linux wrapper for launching applications with NVIDIA PRIME Render Offload.

It is the compiled equivalent of running:

```sh
__NV_PRIME_RENDER_OFFLOAD=1 \
__GLX_VENDOR_LIBRARY_NAME=nvidia \
__VK_LAYER_NV_optimus=NVIDIA_only \
<command> [args...]
```

Useful on hybrid-GPU laptops and desktops where you want selected applications to run on the discrete NVIDIA GPU without exporting these variables globally.

## Features

- Native C executable with no runtime dependencies beyond libc
- Works with arbitrary commands and arguments
- Supports OpenGL/GLX and Vulkan PRIME offload environment variables
- Preserves the launched application's normal process behavior by using `execvp(3)`
- Returns shell-compatible errors when a command cannot be executed
- Supports custom installation prefixes and packaging via `DESTDIR`
- Installs a `man(1)` page
- MIT licensed

## Requirements

- Linux
- A working NVIDIA driver and PRIME Render Offload configuration
- A C compiler such as GCC or Clang
- `make`

`glxinfo` is optional and is only needed for `make info`.

## Build

```sh
make
```

The resulting executable is:

```text
./run-nvidia
```

## Install

The default installation path is `/usr/local/bin/run-nvidia`:

```sh
sudo make install
```

To install specifically to `/bin/run-nvidia`:

```sh
sudo make install BINDIR=/bin
```

To install to another prefix:

```sh
sudo make install PREFIX=/usr
```

For package building/staging:

```sh
make install DESTDIR=/tmp/package-root PREFIX=/usr
```

## Usage

```sh
run-nvidia <command> [args...]
```

Examples:

```sh
run-nvidia firefox
run-nvidia steam
run-nvidia blender
run-nvidia glxinfo -B
```

If a command begins with `-`, use `--` to stop option parsing:

```sh
run-nvidia -- <command> [args...]
```

Show help:

```sh
run-nvidia --help
```

Show version:

```sh
run-nvidia --version
```

After installation, the manual page is also available:

```sh
man run-nvidia
```

## Verify NVIDIA offload

If `glxinfo` is installed:

```sh
make info
```

or:

```sh
run-nvidia glxinfo -B
```

The renderer should report your NVIDIA GPU when PRIME Render Offload is configured correctly.

For Vulkan, tools such as `vulkaninfo` can be launched the same way:

```sh
run-nvidia vulkaninfo
```

## Uninstall

For the default installation:

```sh
sudo make uninstall
```

If you installed to `/bin`:

```sh
sudo make uninstall BINDIR=/bin
```

## Make targets

```text
make            Build the executable
make check      Run lightweight self-checks
make info       Run glxinfo -B through run-nvidia
make install    Install the executable and manual page
make uninstall  Remove the installed executable
make strip      Strip debug/symbol information
make release    Clean, build, check, and strip
make clean      Remove the local executable
```

## Why not put `sudo` inside the Makefile?

Privilege elevation is intentionally left to the caller. This keeps the Makefile usable by package managers, CI systems, containers, staged installs, and non-root prefixes.

## License

MIT. See [LICENSE](LICENSE).
