<img width="2160" height="1080" alt="github" src="https://github.com/user-attachments/assets/865d6b4e-3861-41fc-b0ec-ac44570871e2" />

# Phasmophobia Stopwatch

### What is this
A crossplatform stopwatch overlay window supporting Windows, Linux and macOS intended to be used while playing [Phasmophobia](https://store.steampowered.com/app/739630/Phasmophobia/). There are a few scenarios within Phasmophobia where tracking time is a huge benefit to the player like timing your smudges, timing various grace periods like the hunt grace period and more. This stopwatch was designed with ease of use in mind. Once launched, it will always stay on top of your game. Hotkeys are used to control the stopwatch, and will work even if you're in game. The hotkeys are re-assignable in settings as well. Speaking of settings, this stopwatch features many customizations including font, font size, font color, font color states, font background, font border, display modes, and color effects including gradient support with more planned features coming down the pike. This stopwatch also features a clock overlay window, which will output your system time alongside your stopwatch for all the imnmersive fullscreen gamers that don't always see their taskbar time!

## Default hotkeys
- Press Left Shift + 1 to Toggle timer on/off
- Press Left Shift + 2 to Reset timer
- Press Left Shift + 3 Undo reset

## How to change hotkeys and other settings
- Right click on Stopwatch window

## Install on Windows
Setup installer coming soon...


## Install on Linux
### Prerequisites: Install build tools and ensure compatibility
- You must be running X11 or something comparable to Xwayland. Please see [caveat for running on Linux](#running-on-linux)
- After ensuring you have the proper setup, install the necessary tools:
`git` `cmake` `qt6-base` `qt6-tools`
### Step 1: Clone
`git clone https://github.com/Bo0sted/phasmophobia-stopwatch-timer.git`
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
  - **If the app is not responding to your hotkey activations, this is a telltale sign that X11 is not being used in the environment the app is running on.**
### Important notice about registering hotkeys specifically on Linux:
- It is strongly recommended to use modifier keys when registering a hotkey on Linux. This is because modifier keys (such as Ctrl, Alt, Meta, Shift among others) force X11 to process the hotkey **immediately** instead of queueing it up and processing it as a [passive key grab](https://www.x.org/releases/current/doc/man/man3/XGrabKey.3.xhtml) which happens when no modifiers are used. If you are experiencing any issues with hotkeys not triggering or triggering with a delay while the app is not in focus, please consider registering your hotkeys with a modifier. Numpad keys seem to also process **immediately** without modifier keys but it may be different on your system.
### Known caveats
- The stopwatch wont be able to stay on top of the Phasmophobia window if fullscreen mode is turned on. This is because on Linux, the [compositor](https://en.wikipedia.org/wiki/Compositing_manager) handles how windows get stacked and in which order. Most compositors stack fullscreen windows at the very top, which is why the stopwatch still gets forced out of view by your system despite being programmed not to.
  - Despite this limitation, most compositors offer a way for users to create special rules for windows. One of these rules is stacking order. By forcing your Phasmophobia window to the bottom of the stack, or forcing the stopwatch window to the front, you can force the stopwatch to always be on top of Phasmophobia.  
    - <details><summary>Example:</summary><img width="1079" height="882" alt="image" src="https://github.com/user-attachments/assets/96c6f3d9-7088-4d7d-8e82-811ef88c7d76" /></details>




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

## Why so many customizations??
Well if its gonna be sitting on your screen all the time it might as well look pretty, right?

## Breakdown of the backend
- Written in pure C++ using the lovely Qt framework to make this app cross platform for Windows, Linux and ([limited MacOS support](#macos-support))
- Multi-threaded design to ensure the time is never delayed and user interface is responsive and fluid.
- Using libuiohook to handle global hotkeys in a non-blocking fashion. This is a major improvement over QHotkey, which I used in version 1.3 and below. While Qhotkey was super easy and intuitive to use, it came with many limiations like no numpad key support and worst of all it handled keys in an exclusive fashion. Keys would not get passed onto other programs once the stopwatch captured the key.

### MacOS support
- This project (should) compile on Mac. It uses the Qt framework and libuiohook, both of which are cross platform. The problem with Mac is I don't personally have one, and in order to release anything related to Mac I need to own one since virtualizing that platform is not easy. After compiling, the program should run and register hotkeys 100% fine, but the issue is many of the keys that are defined in libuiohook are outdated and no longer correspond with the operating system. That means when you go to register a hotkey, instead of showing the name of the key that's registered, it'll simply show "Unknown". If this is not an issue for you, then all you need to do is compile the code and everything should work fine.

# License
This project is licensed under the **GNU General Public License version 3 (GPL-3.0)**.  
The GPL-3.0 license is a free software license that allows anyone to use, copy, modify, and distribute this software for any purpose, as long as any derivative works or redistributed versions are also licensed under the same terms. This ensures that the software remains free and open for everyone, and that improvements made by one person can benefit the whole community.

When you modify or redistribute this software, you must include the same license and provide access to the source code. This protects both the original author’s work and the community of users who rely on it, guaranteeing that software freedom is preserved and that all users can study, share, and improve the project.

By using, modifying, or distributing this software, you agree to comply with the terms of the GPL-3.0 license.  
For the full license text, please visit [GNU GPL v3 License](https://www.gnu.org/licenses/gpl-3.0.txt).


### Internet access
When launching the app for the first time in a while, the app will attempt to make precisely two network requests. The sole reason for these requests is to improve the user experience by automatically checking for a new version, and to get a rough estimate of the number of active users through the use of an anonymous ping. To maintain complete transparency, please read below to get a comprehensive list of everything that is collected and how its used.
- Firstly, this repo will be checked, to get the latest official version of this program. [Source](https://github.com/Bo0sted/phasmophobia-stopwatch-timer/blob/252d701133dbf81eb04f54e559d0f696acdb945e/updatemanager.cpp#L23)
- Next, a usage ping is posted to my server.
  - The generic name of your operating system (example: "Windows, "macOS", "Linux Mint").
  - Program version.
  - Your machine's [QUUID](https://doc.qt.io/qt-6/quuid.html#details).
    - This is a randomly generated & really long number to differentiate one PC from another. QUUIDs are one way, meaning once the number is generated there is no way to extract any information about how it was generated. All that's guaranteed is the number will be unique across different machines. Without your QUUID, I would not be able to tell one PC from another and thus not be able to tell the approximate number of active users. 
  - Your operating system's [language](https://github.com/Bo0sted/CrossplatformStopwatch/blob/master/updatemanager.cpp#L77)
  - And last but not least a unix timestamp
 - You may verify the contents of the ping yourself by looking at the code [itself](https://github.com/Bo0sted/CrossplatformStopwatch/blob/master/updatemanager.cpp#L66)

### Can I disable the pinging / update checks?
Due to the program still being in beta, there is no easy way to do this except for editing the source code and re-building the binary. By the time I publish version 2.0 which marks the end of beta, the option to disable these logs will be there.

# Credits
[libuiohook](https://github.com/kwhat/libuiohook)





























