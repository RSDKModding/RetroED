![](header.png?raw=true)

# How to Build

## Get the source code
In order to clone the repository, you need to install Git, which you can get [here](https://git-scm.com/downloads).

Clone the repo, using: `git clone https://github.com/Rubberduckycooly/RetroED-v2`

If you've already cloned the repo, run this command inside of the repository: `git submodule update --init --recursive`

### Windows
#### Pre-requisites
- Download and install Python 3 from https://www.python.org/downloads/, then reboot the computer
- From a Command Prompt, run `pip install pytz`
- Get Qt Online Installer from https://www.qt.io/download-qt-installer-oss
- Make a custom install, Enable `Archive` and refresh, then on `Qt 5.15.2` enable the component `MSVC 2019 64-bit`, all the other necessary components should be selected by default
  - If Qt Online Installer has a very slow download speed, cancel the install and open the exe through a command line with the argument `--mirror [mirror url]`, a list of the available mirrors can be found [here](https://download.qt.io/static/mirrorlist/), copy the one closest to your location.
- On the folder of the project, run in a Command Prompt `python versiongen.py`

#### Building
* Open `RetroEDv2.pro` in Qt Creator, on Projects make sure `Release` builds are enabled and press `Configure Project`, from there just press the `Build Project RetroED2` button
* Alternatively, you can grab a prebuilt executable from the releases section.

## Linux
#### Pre-requisites
Install the following dependencies:
- **apt (Debian/Ubuntu):** `sudo apt install build-essential python3.6 python3-pip libqt5svg5-dev qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools`
- Your favorite package manager here, make a pull request
  - Qt Creator can also be installed by adding `qtcreator` to the command above, allowing to use the same building process as Windows
- Run the command `pip install pytz`
- Inside the project folder, run the command `python3 versiongen.py`

#### Building
- On the folder of the project, run the following commands:
```
mkdir build
qmake -o build RetroEDv2.pro
make -j$(nproc)
  * -j switch is optional, but will make building faster by running it parallel on multiple cores (nproc will use all available cores).
```
* Alternatively, you can grab a prebuilt executable from the releases section.

## MacOS
* Clone the repo and open the project in Qt creator, from there just press the "build project RetroED2" button

## Other Platforms
The only supported platforms are the ones listed above. There are no plans to support other platforms for the time being.

# FAQ
### Q: I found a bug!
A: Submit an issue in the issues tab and it will get followed up on as soon as we get the chance to. Please provide as much information as possible when submitting bugs! We also accept PRs for bug fixes & revisions where applicable!

# Contact:
Join the [Retro Engine Modding Discord Server](https://dc.railgun.works/retroengine) for any questions you may need to know about the editor or how to use it.
