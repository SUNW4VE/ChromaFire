# fastCF

## Overview

`fastCF` is a high-performance, low-latency color-based trigger bot designed to showcase exploitable features to Pixel Gun 3D game developers. 

## Why Use fastCF Over Legacy Chromafire?

✅ **Better accuracy**: 94% → 98% detection rate improvement  
✅ **Lower overhead**: Reduced CPU/GPU usage  
✅ **Lower-level implementation** for better performance  
✅ **More reliable detection**: Uses non-white pixel checks instead of red-detection  
✅ **Minimal input lag**: Faster signal processing  
✅ **No setup required**: Just download and run  
✅ **No reported bugs**  
✅ **Not world infamous chicken fettuccine alfredo code**

## Installation & Usage

### Download and Run

1. Download `fastCF.exe`
2. Double-click the executable to launch.
3. To quit, use any of the following methods:
   - Close the window
   - Press `CTRL + C` in the terminal
   - Press `CTRL + Q` anywhere

## How It Works

`fastCF` operates by continuously checking the pixel color at the center of the screen. If the pixel intensity falls below a certain threshold, it simulates a mouse click to trigger an action. The program uses a background thread for handling input events and efficiently processes pixel data using Windows API functions.

## Controls

```plaintext
CTRL + Q                  -> Quit program
Right Mouse Button (Hold) -> Activate trigger bot
```

## Development

### Building from Source

#### Prerequisites

- Windows OS
- `g++` (MinGW recommended)
- Windows SDK

#### Compile and Run

```sh
g++ -o fastCF.exe fastCF.cpp -mwindows -O2
fastCF.exe
```

## Notes

- Ensure you have the required permissions to run applications that use low-level hooks.
- Since `fastCF` modifies input behavior, some systems might flag it as a potential risk. Add an exception in your security settings if necessary.

## Disclaimer

This software is provided for educational and testing purposes only. The author is not responsible for any misuse.

---
