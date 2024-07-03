![](header.png?raw=true)

A general purpose tool suite for the Retro Engine (versions 1-5).

Available tools:
- Datapack Unpacker/Repacker
- Scene Editor
- Animation Editor
- GameConfig Editor
- Palette Editor
- Model Manager (v4/v5)
- Script Compiler (v1/v3/v4)
- GFX Manager (v1)
- UserDB Manager (v5)
- RSV Manager (v2)

# **SUPPORT THE DEVELOPERS OF THE RETRO ENGINE**
We do not own the Retro Engine in any way, shape or form, and this project would not have been possible had they not developed RSDK in the first place. Retro Engine is currently owned by [Evening Star](https://eveningstar.studio/); we highly urge you to follow & support their projects if you enjoyed this project of ours!

# How to Build

If you prefer to use a prebuilt executable, there are Windows and Linux builds available in the Releases section.

## Get the source code
In order to clone the repository, you need to install Git, which you can get [here](https://git-scm.com/downloads).

Clone the repo, using: `git clone https://github.com/RSDKModding/RetroED`

If you've already cloned the repo, run this command inside of the repository: `git submodule update --init --recursive`

## Windows
### Pre-requisites
- Download and install Python 3 from [here](https://www.python.org/downloads/), then reboot the computer.
- In Command Prompt, run the command `pip install pytz`.
- Get and run the Qt Online Installer from [here](https://www.qt.io/download-qt-installer-oss).
- When you reach the `Installation Folder` step of the installer, choose `Custom Installation`.
- In the `Select Components` step, enable the `Archive` filter and click `Filter` to refresh the component list.
- From here, you can choose to enable either of the following:
  - `Qt` > `Qt 5.15.2` > `MSVC 2019 64-bit`. (Requires Visual Studio installed with Build Tools and CMake)
  - `Qt` > `Qt 5.15.2` > `MinGW 8.1.0 64-bit` and `Qt` > `Developer and Designer Tools` > `MinGW 8.1.0 64-bit`
- Proceed with the rest of the installation.
  - If Qt Online Installer has a very slow download speed, cancel the install and open the exe through a command line with the argument `--mirror [mirror url]`. A list of the available mirrors can be found [here](https://download.qt.io/static/mirrorlist/); copy the one closest to your location.
- In the `RetroEDv2` project folder, run `python versiongen.py` in Command Prompt.
- Follow the [Qt Creator building steps](#building-using-qt-creator).

## Linux
### Pre-requisites
- Install the following dependencies:
  - **apt (Debian/Ubuntu):** `sudo apt install build-essential python3.6 python3-pip libqt5svg5-dev qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools`
  - Your favorite package manager here, [make a pull request](https://github.com/RSDKModding/RetroED/fork)
    - Qt Creator can also be installed by adding `qtcreator` to the command above, allowing you to use the same building process as the other platforms.
- Run the command `pip install pytz`.
- In the `RetroEDv2` project folder, run the command `python3 versiongen.py`.
- Follow the [Qt Creator building steps](#building-using-qt-creator) (or build from the command line).

### Building from the command line
- In the `RetroEDv2` project folder, run the following commands:
```
mkdir build
qmake -o build RetroEDv2.pro
cd build
make -j$(nproc)
```
The -j switch is optional, but will make building faster by running it parallel on multiple cores (nproc will use all available cores).

## MacOS
### Pre-requisites
- Download and install Xcode from the App Store.
- Download and install Python 3 from [here](https://www.python.org/downloads/) or from a package manager such as [Homebrew](https://brew.sh/).
- In Terminal, run the command `python3 -m pip install pytz`.
- Get and run the Qt Online Installer from [here](https://www.qt.io/download-qt-installer-oss).
- When you reach the `Installation Folder` step of the installer, choose `Custom Installation`.
- In the `Select Components` step, enable the `Archive` filter and click `Filter` to refresh the component list.
- From here, enable of the following:
  - `Qt` > `Qt 5.15.2` > `macOS`.
- Proceed with the rest of the installation.
  - If Qt Online Installer has a very slow download speed, cancel the install and open the exe through a command line with the argument `--mirror [mirror url]`. A list of the available mirrors can be found [here](https://download.qt.io/static/mirrorlist/); copy the one closest to your location.
- In the `RetroEDv2` project folder, run `python3 versiongen.py` in the Terminal.
- Follow the [Qt Creator building steps](#building-using-qt-creator).

## Building using Qt Creator
- Open `RetroEDv2.pro` in Qt Creator, then in the Projects tab, make sure `Release` builds are enabled and press `Configure Project`.
- Press the `RetroEDv2` monitor icon, then ensure the Build is set to `Release`.
- Press the `Build Project "RetroEDv2"` button. The built executable should be located in `build/[used kit name]/release`.

## Other Platforms
The only supported platforms are the ones listed above. There are no plans to support other platforms for the time being.

# FAQ
### Q: I found a bug!
A: Submit an issue in the issues tab and it will get followed up on as soon as we get the chance to. Please provide as much information as possible when submitting bugs! We also accept PRs for bug fixes & revisions where applicable!

# Contact:
Join the [Retro Engine Modding Discord Server](https://dc.railgun.works/retroengine) for any questions you may have about the editor or how to use it.
