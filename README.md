<img width="1469" height="777" alt="image" src="https://github.com/user-attachments/assets/6a941f6e-9742-4c52-9898-2996d65babe5" />

**If you are using Windows, please use my native [Windows version](https://github.com/Bo0sted/SimpleUsefulTimer) of this stopwatch for a better experience.**

# Welcome to Crossplatform Stopwatch
- Stopwatch has Start, Stop and Reset state. Press 1 to Start/Stop and 2 to reset. Hotkeys are re-assigable in settings.
- System clock monitor available. Enable in settings. 
- Settings are automatically saved and loaded on startup
- Lots of customization options
- Multi-threaded design to ensure the time is never delayed and user interface is responsive and fluid.
- Stopwatch window always on top of other windows by default
- Hotkeys are always global. This means the hotkeys will work anywhere on your system even if the stopwatch is not in focus. **Please see caveats sections for more information regarding this**
- Written in pure C++ using the lovely Qt framework to make this app cross platform. 

## Caveats with Hotkeys
- Numpad keys are non-assignable.
- Whichever hotkeys are assigned in settings (whether thats default or custom hotkeys), they will become **exclusive** to the stopwatch. Which means as long as the stopwatch is open, the two hotkeys will **not** work in any other window except the stopwatch. Please choose two keys you do not often use in your day-to-day typing.
- There are a few other minor [known limitations](https://github.com/Skycoder42/QHotkey?tab=readme-ov-file#known-limitations) with the hotkey library I chose for this project. Unfortunately, there are no good "one size fits all" solutions for cross platform hotkeys.

## Caveats with program behavior
- Turning the background on/off is broken and requires a program restart to see the effect changed.


# Install instructions
coming soon...

# License
Licensed under the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.en.html).


# Credits
[Skycoder's cross platform hotkey library...](https://github.com/Skycoder42/QHotkey)
