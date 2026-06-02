# NMO-SDK

**Historical archive** — experimental cross-platform C++ SDK, developed between 1996 and 2001 by Henri Michelon with the 
supportive help from Christophe Decanini. Version 0.1 was publicly released on November 3, 2001. 
The project was never officially finished; this repository is its archive.

Only one application have been developped with this SDK : the "361 Mixer" a mixing deck for the french rapper
[Akhenaton](https://en.wikipedia.org/wiki/Akhenaton_(rapper)), released in 2002 with his second album "Sol Invictus"
on a hybrid Audio/Data CD.

**24 years after the first official release, in 2025, the GUI subsystem was extracted, adapted to modern C++ 23 standards and ported as the foundation for the widget framework of the [Lysa Game Engine](https://github.com/LysaEngine)
and now live in his own repository [Lysa UI](https://github.com/LysaEngine/lysa_ui).**

---

## Context

NMO-SDK was an attempt at building a portable, self-contained application framework in C++ at a time when no dominant cross-platform solution existed. The goals were to provide a single API surface covering GUI, graphics, networking, audio, threading, filesystem, and databases, with each subsystem backed by native platform implementations. The architecture targeted more than fifteen operating systems simultaneously, including several platforms that were already declining (AmigaOS, BeOS) or still maturing (MacOS X, QNX).

The codebase is written in standard C++ (98!) with no external runtime dependency beyond the system libraries of each target platform.

---

## Supported platforms (at time of archive)

| OS | Versions | Architecture | Compiler |
|---|---|---|---|
| Linux | 2.4 | x86 | GCC 2.95.2 |
| Windows | 98, Me, NT, 2000 | x86 | Visual Studio 6/7, Borland C++ 5.5 |
| MacOS Classic | 8.x, 9.x | PowerPC | CodeWarrior 5 |
| MacOS X / Darwin | 1.x | PowerPC | GCC 2.95.2, CodeWarrior 5 |
| FreeBSD | 3.5, 4.x | x86 | GCC 2.95.2 |
| NetBSD | 1.5 | x86 | GCC 2.95.2 + Pth |
| OpenBSD | 2.9 | x86, PowerPC, SPARC | GCC 2.95.3 |
| AmigaOS | 3.x | m68k | GCC 2.95.2 (GeekGadgets) |
| AmigaOS / WarpUP | 3.x | PowerPC | GCC 2.95.1 |
| BeOS | 5 | x86 | GCC |
| IRIX | 6.5.x | MIPS (n32) | GCC 2.95.2 |
| QNX RTP | — | x86 | GCC 2.95.2 |
| SunOS | 5.8 | SPARC | GCC 2.95.x |
| AtheOS | 0.x | x86 | GCC 2.95.2 |

---

## Architecture

The SDK is organized into two libraries (`libnmo`, `libiface`) and a set of optional addons. The build system is GNU Autoconf + GNU Make, with Visual Studio 6/7 and Borland C++ 5.5 project files for Windows, and CodeWarrior 5 projects for MacOS Classic and early MacOS X.

### Layer model

```
┌──────────────────────────────────────────────────────────┐
│  Addons  │  GUI (GWidget…)  │  Shell  │  Digest  │  MB  │
├──────────┴──────────────────┴─────────┴──────────┴───────┤
│  Application framework  (WindowApplication, Console…)    │
├──────────────────────────────────────────────────────────┤
│  Interface layer  (IDisplay, IWindow, IPixmap, IFont…)   │
├──────────────────────────────────────────────────────────┤
│  System  │  Network  │  Sound  │  Thread  │  Database    │
├──────────┴───────────┴─────────┴──────────┴──────────────┤
│  Base  (NMOObject, collections, strings, exceptions…)    │
└──────────────────────────────────────────────────────────┘
```

Every class inherits from `NMOObject`. Platform-specific code is isolated in dedicated translation units (e.g. `IDisplayX11.cpp`, `IDisplayWin32.cpp`, `ThThreadPosix.cpp`) and selected at build time; the public headers expose only the abstract interface.

---

## Subsystems

### Base (`nmo/base/`, `src/base/`)

Foundational layer: `NMOObject` root class, `Stringz`/`UStringz` string types (8-bit and Unicode), template containers (`Array`, `Fifo`, `Lifo`, `Stack`), `Iterator`/`ListIterator`, a unified `NMOException` hierarchy, `Decimal` arithmetic, and `Args` for command-line parsing.

### Interface (`nmo/interface/`, `src/interface/`)

Low-level graphics and input abstraction..

- **Display drivers** — `IDisplayX11`, `IDisplayWin32`, `IDisplayDirectDraw`, `IDisplayMacOS`, `IDisplayBeOS`, `IDisplayAmiga`, `IDisplayCGFX` (CyberGraphX), `IDisplayPhoton` (QNX), `IDisplayChunky`. Each driver maps the abstract `IDisplay` API to the native raster engine, including accelerated paths (DirectDraw, CyberGraphX GPU).
- **Font engines** — `IFontEngineX11`, `IFontEngineWin32`, `IFontEngineMacOS`, `IFontEngineAmiga`, `IFontEngineFreeType`, `IFontEngineFixedPixmap`.
- **Core abstractions** — `IWindow` (window lifecycle and event loop), `IPixmap` (offscreen bitmap), `IPalette`, `IRGBColor`, `IColorSpace`, `IRect`, `ICursor`, `IMouse`, `IKeyb`, `IScreen`.
- **Standard dialogs** — `IStdDialog*` (open/save/message, mapped to native dialogs per platform).

### GUI addon (`nmo/addons/gui/`, `src/addons/gui/`)

High-level retained-mode widget toolkit built on top of the Interface layer. **This is the subsystem ported to C++ 23 for Lysa Engine in 2025.**

- **Base hierarchy** — `GObject` → `GWidget` → all controls.
- **Containers** — `GWindow` (top-level window with layout), `GPanel`, `GTabs`/`GTabButton`, `GScrollBox`.
- **Controls** — `GButton`, `GRoundButton`, `GToggleButton`, `GCheckButton`, `GCheckmark`, `GText`, `GTextEdit`, `GLine`, `GListBox`, `GGrid`/`GGridCell`/`GGridRect`, `GScrollBar`, `GUpDown`, `GTrackBar`, `GValueSelect`, `GProgressBar`, `GPicture`, `GFrame`, `GArrow`, `GBox`.
- **Layout engine** — `GLayout` base, `GLayoutVector` (axis-based automatic placement), `GLayoutPixmap` (manual pixel positioning).
- **Resources** — `GResource`, `GResourcePixmap`, `GResourceVector`; assets referenced through a compile-time `ResourcesTable`.
- **Events** — `GEvent` dispatching; `GWidget::Simulate()` allows synthetic injection of mouse, keyboard, and system events for testing.

### System (`nmo/sys/`, `src/sys/`)

`File`/`Dir`/`DirScan` filesystem API with Posix, Win32, and MacOS backends; `DateTime`, `Alarm`, `Timer`; `SignalEvent` synchronization primitive; `Pipe` (IPC); `DynModule` (runtime shared-library loading with `gendynmod` code generator); `SysInfo` (CPU/memory/OS query); `CfgStorage` (persistent key-value configuration, mapped to INI on Windows and dotfiles on Unix); `EnvPath`.

### Network (`nmo/net/`, `src/net/`)

`NetID`/`NetIDIP` address types; `NetTCP`, `NetUDP`; `NetStream`, `NetMessage`; `URL` parser; `InternetConnection` with platform-specific RAS/dial-up handling (Linux, Win32, FreeBSD, Carbon).

### Sound (`nmo/sound/`, `src/sound/`)

`SoundDevice` abstraction with drivers for WinMM, DirectSound, MacOS/Carbon, Unix `/dev/dsp`, and SDL. `SoundDeviceStream` for streaming playback; `SoundDeviceTracker` for tracker module music; `WaveTrack` for WAV files; `SoundBuffer`/`SoundBufferRaw` for raw PCM data.

### Threading (`nmo/thread/`, `src/thread/`)

`ThThread` (virtual `OnStart()` model) and `ThCriticalSection` mutex, backed by Win32 threads, POSIX pthreads, MacOS classic threads, MacOS MP, BeOS threads, and AmigaOS tasks.

### Database (`nmo/db/`, `src/db/`)

SQL abstraction: `DBDatabase`, `DBTable`, `DBSet`/`DBSnapshot`/`DBDynaset` result-set types, `DBRecord`, `DBColumn`, `DBValue`, `DBType`.

### Datatypes (`nmo/datatypes/`, `src/datatypes/`)

Format I/O: PNG (libpng 1.2.x), GIF (giflib), IFF (AmigaOS native), WAV/RIFF, and key-value config files. Compression via bundled zlib 1.1.3. `FilePixmap` provides a format-agnostic image loader dispatching to the above.

### Application framework (`src/`)

`Application` root class with four specializations: `WindowApplication` (GUI event loop, with platform-specific subclasses for X11, Win32, MacOS, BeOS, Amiga Intuition, QNX Photon), `ConsoleApplication`, `DaemonApplication` (BSD, MacOS, Win32 service), and `CGIApplication`. `Stream`/`FileStream`/`MemoryStream`/`GZipStream` form the I/O pipeline.

---

## License

NMO SDK Public License v1.1 — see [`licence.txt`](licence.txt).