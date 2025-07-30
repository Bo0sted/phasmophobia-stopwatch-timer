<img width="1469" height="777" alt="image" src="https://github.com/user-attachments/assets/6a941f6e-9742-4c52-9898-2996d65babe5" />

# Welcome to Crossplatform Stopwatch:

## How to use
- Press 1 to Toggle timer on/off
- Press 2 to Reset timer
- Press 3 Undo reset

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

# Caveats

## Regarding MacOS support
- This project is compatible on MacOS, but it requires work. Unfortunately, I do not have a Mac (nor would I ever purchase one) so unfortunately no releases or extra support will be added for MacOS. However, if you have a Macbook and some time, this project will compile on a Mac. All that needs to be done is adding raw scan code translations for libuiohook since its an old library and some of the keycodes that are defined in it are outdated and do not register properly. I did this for Linux and Windows since I have access to both types of machines, but a Mac is something im not willing to touch.

## Running on Linux
- This project uses a hotkey library that is dependent on X11. As a result, your Linux installation must either be running X11 as its display protocl or Wayland with an X11 compatibility layer on top of it like [Xwayland](https://wiki.archlinux.org/title/Wayland)
  - Forcing this app to run under Wayland alone without any compatiblity will either prevent the app from launching all together or hotkeys will 100% not work thus rendering the app useless.

## Understanding the need for internet connectivity
If an internet connection is available, the stopwatch will attempt to connect during these two specific scenarios:
- Everytime the program starts up, it connects to Github.com, looks at this repository and checks the [version](https://github.com/Bo0sted/CrossplatformStopwatch/blob/master/version/version) file for the latest version of this program.
- Every time the program starts up it will also attempt to post an anonymous usage log.
  - The code that handles this logging can be found [here](https://github.com/Bo0sted/CrossplatformStopwatch/blob/master/updatemanager.cpp#L66)
  - But the log only consists of:
    - The name of your operating system. If you run Linux, this will probably include the name of your distro.
    - The version of your program.
    - Your UUID. This is a randomly generated combination of letters and numbers to distinguish your anonymous log from other people. It is generated when the program starts up and doesn't detect a UUID in your config file.
      - The code for UUID generation can be found [here](https://github.com/Bo0sted/CrossplatformStopwatch/blob/master/qsettingsmanager.cpp#L281)
    - Your country code. This information is obtained 100% offline by looking at the language of your system.
    - And last but not least a unix timestamp


That's it! I a am privacy concious person myself so I designed these logs to be as privacy friendly as possible.

### But why the ping log?
Because it's interesting. There is no ulterior motive. It's simply interesting to get an idea of how many people are using my program and from what regions of the world!

### Can I disable them?
As of right now, I haven't added a way to disable the logs just yet since I haven't officially released anything yet. But once I do make an official release, the option will be there. If you are downloading my code and building it yourself, you may go to this [line](https://github.com/Bo0sted/CrossplatformStopwatch/blob/master/mainwindow.cpp#L332), comment it out and the logging functionality will be completely disabled.

# Install instructions
coming soon...

# License
Licensed under the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.en.html).


# Credits
[libuiohook](https://github.com/kwhat/libuiohook)
