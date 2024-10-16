ChromaFire
------------------------------------------------------------------
- ChromaFire is a **white-hat** Pixel Gun 3D triggerbot tool. I
built this for the sole purpose of demonstrating exploitability of
in-game features.
- This program operates discreetly by nature, acting based on the
state of specific display pixels and user-specified key inputs via
the Win32 API. As ChromaFire does not read or write memory
belonging to the game, even kernel-level anti-cheats such as EAC
should have trouble detecting it.
- **This repository has been opened to the **public** as I have
showcased ChromaFire's potential to the game development team, and
they have patched the exploits accordingly.**

Usage for testers:
------------------------------------------------------------------
- Download and unzip the latest release.
- Customize the "CF_config.txt" file if necessary.
- Launch "ChromaFire.exe".
- When detection is active, ChromaFire repeatedly scans the center 
of your screen. It sends a click whenever that pixel is a close 
enough shade of red.
- Focus the terminal window and press **Ctrl+Q** to quit.

Configurable Customizations:
------------------------------------------------------------------
- Weapon keybinds
- Weapon detection pause intervals - Prevent unnecessary scans while the weapon cannot fire due to firerate
- Switch pause interval - Prevent unnecessary scans while the weapon cannot fire due to pullout 
- Enable detection keybind
- Detection by Toggle or Hold
- Detection color tolerances/thresholds

Performance Statistics:
------------------------------------------------------------------
- Pixel-to-click latency of 3.2 +- 0.2ms measured using std::chrono
library functions
- 94% accuracy (when functioning properly)

2.1 Patch Notes:
------------------------------------------------------------------
- You can now choose the trigger mode - HOLD or TOGGLE
- New "switch" interval - Delays detection until a weapon is ready to fire, preventing misinputs. Ignore if you do not play old school
private matches
- Fixed an issue where the sleep interval for one weapon category 
can "creep" into another weapon category, causing the latter to not 
fire when ready

2.0 Update Notes:
------------------------------------------------------------------
- The program has been fully rewritten from AutoHotkey to C++
- Detection is significantly faster than ChromaFire 1.2
- Positive detection to click latency is significantly more 
responsive than ChromaFire 1.2
- Standard ChromaFire and Autoscope ChromaFire have been unified
into a single program
- Customizable weapon keybinds to track when to use autoscope fire
or regular fire
- Automatic resolution detection to determine the center crosshair
pixel
- Fixed an issue where ChromaFire would always send clicks in rapid 
succession when it detects positive. Users can now set customizable 
detection intervals for each weapon category to prevent rapid 
succession of clicking which may trigger anti-cheats
- Fixed an issue where teammates and pets caused ChromaFire to 
send a false trigger due to the crosshair turning blue when placed 
over them
  
Known issues:
------------------------------------------------------------------
- Weapon effects that alter the chromatic nature of your screen
(blindness, charm, etc.) may throw off the detection and disable
ChromaFire for the length of their effect
- Surfaces that are a shade of red will cause false positive
detections
- ChromaFire breaks when toggle bind is set to a mouse click 
button. Currently, there is error handling to prevent users from 
setting it that way
- ChromaFire only functions properly with weapons that have a 
center dot with their crosshair. This is how the program detects
enemies by nature and I am not planning to fix this

Roadmap:
------------------------------------------------------------------
- Implement reload glitch into ChromaFire
- Integrate low-level mouse/keyboard hooks into ChromaFire to
block native toggle inputs from sending through
- Fix other known issues
- Massive code cleanup because this organization is abhorrently horrendous
