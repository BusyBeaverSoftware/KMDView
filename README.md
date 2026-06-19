# KMDView

A lightweight, KDE-native application for previewing Markdown files on Linux.

KMDView renders `.md`, `.mkd`, and `.markdown` documents with readable typography, adjustable column width, and multiple reading themes. It integrates with KDE Plasma through standard reverse-DNS packaging, a desktop entry, AppStream metadata, and a hicolor icon.

![License: GPL v2+](https://img.shields.io/badge/License-GPL%20v2%2B-blue.svg)
![Platform: Linux](https://img.shields.io/badge/Platform-Linux-informational)
![Built with: Qt 6](https://img.shields.io/badge/Built%20with-Qt%206-green)

## Features

- **Native markdown preview** using Qt's built-in renderer
- **Relative image and link resolution** for local documents
- **Readable typography** with clear heading hierarchy, code blocks, blockquotes, and tables
- **Adjustable content width** via toolbar slider (480 px up to full window width)
- **Full-height document panel** with internal scrolling for long files
- **Six reading themes**: Light, Dark, Sepia, Solarized Light, Solarized Dark, High Contrast
- **Persistent settings** for theme, width, and full-width mode
- **Multiple ways to open files**: command line, File menu, drag-and-drop, Dolphin "Open With"
- **Reload** without restarting the application
- **KDE Plasma integration**: taskbar icon, MIME associations, Office category in the app launcher

## Screenshots

Open a markdown file and use **View → Theme** to switch reading themes. Adjust **Width** in the toolbar to change the document column size.

## Requirements

### Build tools

| Dependency | Minimum version |
|------------|-----------------|
| CMake | 3.20 |
| Extra CMake Modules (ECM) | 6.0 |
| Qt 6 | 6.6 (Core, Gui, Widgets) |
| C++ compiler | C++17 |

### Debian / Ubuntu

```bash
sudo apt install \
  cmake \
  extra-cmake-modules \
  qt6-base-dev \
  build-essential
```

### Fedora

```bash
sudo dnf install \
  cmake \
  extra-cmake-modules \
  qt6-qtbase-devel \
  gcc-c++
```

### Optional: KDE Frameworks

KMDView works with Qt 6 and ECM alone. For deeper KDE Frameworks integration (KAboutData, Ki18n), you can additionally install:

```bash
sudo apt install libkf5coreaddons-dev libkf5i18n-dev libkf5xmlgui-dev
```

## Building from source

Clone the repository:

```bash
git clone https://github.com/BusyBeaverSoftware/KMDView.git
cd KMDView
```

Configure, build, and install to `~/.local` (recommended for user installs):

```bash
cmake -B build -DCMAKE_INSTALL_PREFIX="$HOME/.local"
cmake --build build --parallel
cmake --install build
```

Ensure `~/.local/bin` is on your `PATH`:

```bash
export PATH="$HOME/.local/bin:$PATH"
```

### System-wide install

```bash
cmake -B build -DCMAKE_INSTALL_PREFIX=/usr
cmake --build build --parallel
sudo cmake --install build
sudo update-desktop-database /usr/share/applications
```

### Uninstall

If you used the default ECM uninstall target:

```bash
cmake --build build --target uninstall
```

## Usage

```bash
# Open a specific file
kmdview README.md

# Open an empty window
kmdview
```

Inside the application:

- **File → Open** to browse for a markdown file
- **File → Reload** (or press F5) to refresh the current document
- **View → Theme** to switch reading themes
- **Width** slider in the toolbar to adjust the document column (drag fully right for 100% width)
- Drag and drop `.md` files onto the window

After installation, Dolphin and other file managers can open markdown files with KMDView via **Open With**.

## KDE integration

KMDView follows KDE application conventions:

| Item | Value |
|------|-------|
| Application ID | `org.kde.kmdview` |
| Binary | `kmdview` |
| Desktop file | `org.kde.kmdview.desktop` |
| AppStream ID | `org.kde.kmdview` |
| Icon name | `org.kde.kmdview` |
| Launcher category | Office |
| MIME types | `text/markdown`, `text/x-markdown` |

After installing or updating the desktop file locally:

```bash
update-desktop-database ~/.local/share/applications
```

If the launcher does not update immediately, log out and back in or restart Plasma.

## Project layout

```
KMDView/
├── CMakeLists.txt              # Root build and install rules
├── org.kde.kmdview.desktop     # Desktop entry for Plasma / XDG
├── org.kde.kmdview.metainfo.xml
├── icons/                      # App icon (hicolor scalable)
├── src/                        # Application source
│   ├── main.cpp
│   ├── mainwindow.cpp
│   └── documenttheme.cpp
└── examples/sample.md          # Sample document for testing
```

## Contributing

Bug reports and pull requests are welcome on [GitHub Issues](https://github.com/BusyBeaverSoftware/KMDView/issues).

When submitting changes:

1. Fork the repository
2. Create a feature branch
3. Build and test locally
4. Open a pull request with a clear description of the change

## License

KMDView is licensed under **GPL-2.0-or-later**, which is compatible with KDE applications and the wider free software ecosystem.

See [LICENSE](LICENSE) for the full text.

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
