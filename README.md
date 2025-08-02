<img width="1469" height="777" alt="image" src="https://github.com/user-attachments/assets/6a941f6e-9742-4c52-9898-2996d65babe5" />

# Phasmophobia Stopwatch

### What is this
A crossplatform stopwatch overlay window supporting Windows and Linux intended to be used while playing [Phasmophobia](https://store.steampowered.com/app/739630/Phasmophobia/). There are a few scenarios within Phasmophobia where tracking time is a huge benefit to the player like timing your smudges, timing various grace periods like the hunt grace period and more. This stopwatch was designed with ease of use in mind. Once launched, it will always stay on top of your game*. Hotkeys are used to control the stopwatch, and will work even if you're in game. The hotkeys are re-assignabnle in settings as well. Speaking of settings, this stopwatch features many customizations including font, font size, font color, font color states, font background, font border, display modes, and color effects with more planned features coming down the pike. This stopwatch also features a clock overlay window, which will output your system time alongside your stopwatch for all the imnmersive fullscreen gamers that don't always see their taskbar time!

* If you're using Linux, see [this](#known-caveats)

# Install instructions
## Windows
- Setup installer coming soon...
## Linux
### Prerequisites: Install build tools and ensure compatibility
- You must be running X11 or something comparable to Xwayland. Please see [caveat for running on Linux](#running-on-linux)
- After ensuring you have the proper setup, install the necessary tools:
  - **Arch / EndeavourOS / Manjaro**
    - `sudo pacman -S --needed git cmake base-devel qt6-base qt6-tools`
  - **Debian/Ubuntu**
    - `sudo apt install git cmake build-essential qt6-base-dev qt6-base-dev-tools -y`
  - **Fedora**
    - `sudo dnf install git cmake gcc-c++ make qt6-qtbase-devel qt6-qttools-devel -y`
### Step 1: Clone
`git clone https://github.com/Bo0sted/CrossplatformStopwatch.git`

`cd CrossplatformStopwatch`
### Step 2: Create a build folder
`mkdir build`

`cd build`
### Step 3: Run CMake
`cmake ..`
### Step 4: Compile
`make`
### Step 5: Launch app
`./CrossPlatformStopwatch`

## Running on Linux
### X11 and its relevance to this app
- This project uses libuiohook, which is dependent on the [X11 protocol](https://is.gd/GhPqpn). As a result, your Linux installation must either be running X11 as its display protocol or running Wayland with an X11 compatibility layer on top of it like [Xwayland](https://wiki.archlinux.org/title/Wayland)
  **- Forcing this app to run under Wayland alone without any compatiblity will either prevent the app from launching all together or hotkeys will 100% not work thus rendering the app useless.**
### Important notice about registering hotkeys specifically on Linux:
- It is strongly recommended to use modifier keys when registering a hotkey on Linux. This is because modifier keys (such as Ctrl, Alt, Meta, Shift among others) force X11 to process the hotkey **immediately** instead of queueing it up and processing it as a [passive key grab](https://www.x.org/releases/current/doc/man/man3/XGrabKey.3.xhtml) which happens when no modifiers are used. If you are experiencing any issues with hotkeys not triggering or triggering with a delay while the app is not in focus, please consider registering your hotkeys with a modifier. Numpad keys seem to also process **immediately** on Arch running Zen
### Known caveats
- If you're running Phasmophobia in fullscreen mode then this stopwatch wont be able to overlay the game. This is not an issue with the stopwatch, this is moreso an issue with how Linux works internally with compositors. It is recommended you run Phasmophobia in window mode and set special rules for it within your desktop compositor to achieve a fullscreen borderless mode.
 
## How to use
- Press Left Shift + 1 to Toggle timer on/off
- Press Left Shift + 2 to Reset timer
- Press Left Shift + 3 Undo reset

## How to change hotkeys and other settings
- Right click on Stopwatch window

## Comprehensive feature list for users
- 3 functions with assingable hotkeys
  - Toggle on/off, Reset, Undo reset
- System clock overlay window
  - This is a tiny overlay window that displays your system clock. Just like your stopwatch window, this window will attempt to stay on top of all other windows on your desktop.
- Assignable hotkeys in settings
  - Note: Left and Right modifiers count as different keys. For example, Left Ctrl and Right Ctrl are two seperate hotkeys despite sharing the same name "Ctrl". Same with left side Meta/Windows key and right side.
  - The following modifier keys are supported:
    - Ctrl
    - Alt
    - Shift
    - Meta/Windows key
    - Caps Lock
- Hotkeys are always global and will work from anywhere on your system.
- Many UI customizations
  - Font
  - Display formats
    - Standard mode (Hour\:Minute:Second)
    - Minutes only mode. This will display the total number of minutes as a decimal.
    - Seconds only mode. This will display the total number of seconds only. 
  - Colors
  - Conditional colors
  - Toggle background on/off
- Your settings are automatically handled in tbe background. No need to ever save or load any settings.
- This program remembers window placements. Meaning once you pick a spot for your timer and system clock, the windows will always return to that spot when you launch it the next time.

## Breakdown of the backend
- Written in pure C++ using the lovely Qt framework to make this app cross platform* (for the most part, see caveats)
- Multi-threaded design to ensure the time is never delayed and user interface is responsive and fluid.
- Using libuiohook to handle global hotkeys in a non-blocking fashion. This is a major improvement over QHotkey, which I used in version 1.3 and below. While Qhotkey was super easy and intuitive to use, it came with many limiations like no numpad key support and worst of all it handled keys in an exclusive fashion. Keys would not get passed onto other programs once the stopwatch captured the key.

### MacOS support
- This project (should) compile on Mac. It uses the Qt framework and libuiohook, both of which are cross platform. The problem with Mac is I don't personally have one, and in order to release anything related to Mac I need to own one since virtualizing that platform is not really possible. After compiling, the program should run and register hotkeys 100% fine, but the issue is many of the keys that are defined in libuiohook are outdated and no longer correspond with the operating system. That means when you go to register a hotkey, instead of showing the name of the key that's registered, it'll simply show "Unknown". If this is not an issue for you, then all you need to do is compile the code and everything should work fine.

# License
This project is licensed under the GNU General Public License version 3.0 (GPL‑3.0). It is a copyleft license that ensures the software remains free and open for all users.

Key terms of GPL‑3.0:

    You are free to use, copy, modify, and distribute the software for any purpose, including commercial use.

    If you distribute the software in modified form or as a compiled binary, you must also provide the complete corresponding source code.

    Any distributed version, modified or unmodified, must remain under the GPL‑3.0 license. You cannot relicense it under a proprietary or more restrictive license.

    Contributors grant a patent license for any patents that their contributions might cover.

    The license includes an anti‑tivoization clause, which means that if the software is distributed on hardware that restricts modification, you must still allow users to install and run modified versions.

    Additional restrictions that limit the rights granted by GPL‑3.0 cannot be applied.

The full text of the license is available here: https://www.gnu.org/licenses/gpl-3.0.en.html

By contributing to this project, you agree that your contributions will be licensed under GPL‑3.0.
For more details, see the full license text here: https://www.gnu.org/licenses/gpl-3.0.en.html

### Transparency about remote capabilities
When launching the app (and a period of 2 hours has passed since last connection), if an internet connection is available, the stopwatch will attempt exactly two network requests: 
- First connection is to this website, github.com, and it checks the [version](https://github.com/Bo0sted/CrossplatformStopwatch/blob/master/version/version) file for the "official" version of this app.
- Next connection is to my humble [worker](https://workers.cloudflare.com/) server that collects a generic and anonymous log about your system.
  - The log consists of:
    - The name of your operating system. If you run Linux, this will probably include the name of your distro.
    - Program version.
    - Your UUID. This is a randomly generated combination of letters and numbers to distinguish your anonymous log from other people. It is generated when the program starts up and doesn't detect a UUID in your config file.
      - The code for UUID generation can be found [here](https://github.com/Bo0sted/CrossplatformStopwatch/blob/master/qsettingsmanager.cpp#L281)
    - Your country code. This information is obtained 100% offline by looking at the language of your system.
    - And last but not least a unix timestamp
    - The function that handles this logging can be found [here](https://github.com/Bo0sted/CrossplatformStopwatch/blob/master/updatemanager.cpp#L66)

### Why the log?
As somebody who takes my own privacy very seriously I would never try to infringe on the privacy of somebody else. With that being said I am naturally a curious person, and quite frankly, I think it's really interesting to see roughly how many people use my app and from what parts of the world! That is why I designed these logs to be as privacy friendly as possible, and why I am being so transparent about it.

### Can I disable them?
As of right now since there isn't any public release yet, I haven't gotten around to adding the code that disables it. But once I do make an official release, the functionality will be there. If you're compiling my code for yourself, you may go to this [line](https://github.com/Bo0sted/CrossplatformStopwatch/blob/master/mainwindow.cpp#L332), comment it out and the logging functionality will be completely disabled.

# Credits
[libuiohook](https://github.com/kwhat/libuiohook)




