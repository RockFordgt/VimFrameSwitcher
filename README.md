# VimFrameSwitcher

## Usage
QtCreator plugin for switching active windows/splits like in vim splits.

When you use splits in vim/neovim you can jump between them with key sequence
Ctrl+W,[HJKL]. This plugin adds the same capability to your QtCreator.

Unfortunately `Ctrl+W` is already used for closing current document.
QtCretor sequences for managing windows starts with `Ctrl+E`, therefor
this plugin uses the same start sequence.

| Shortcut | Alternative | Description |
|----------|-------------|-------------|
| Ctrl+E,H | Alt+H       | move to split on left from current |
| Ctrl+E,L | Alt+L       | move to split on right from current |
| Ctrl+E,K | Alt+K       | move to split up from current |
| Ctrl+E,J | Alt+J       | move to split down from current |

To change this default assignments go to:
Preferences | Environment | Keyboard

## Versioning

x.y.z

x - plugin rewrite
y - some minor updates or bug fixes
z - update for new QtCreator

| Version | QtCrator version|
|---------|-----------------|
| v1.1.5  |  10.0.0 |
| v1.1.4  |  9.0.0 |
| v1.1.3  |  8.0.1 |
| v1.1.2  |  4.15.2 |
| v1.1.1  |  4.15.1 |


## Installing

From [release page](https://github.com/RockFordgt/VimFrameSwitcher/releases)
Download proper release for your QtCrator version and OS.

Asets naming schema:
`VimFrameSwitcher-<QtCreator version>-<OS>`

Release version matches plugin version, but package name contains
destination QtCreator version.
ex: `VimFrameSwitcher-8.0.1-Linux-x64.7z`

VimFrameSwitcher for QtCreatorv 8.0.1 for Linux x64

Start QtCrator and from menu:

Help | About Plugins... | Install Plugin...

Select package, you don't have to decompress is, QtCreator will do it.


