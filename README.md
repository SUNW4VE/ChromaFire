ChromaFire - a triggerbot for Pixel Gun 3D
------------------------------------------------------------------

HELP WANTED:
------------------------------------------------------------------
- Integrate low-level mouse/keyboard hooks into ChromaFire to
block native toggle inputs from sending through

How it works:
------------------------------------------------------------------
- When detection is active, ChromaFire repeatedly scans the center 
of your screen. It sends a click whenever that pixel is a close 
enough shade of red. The threshold and tolerance of this shade, as
well as the detection bind and mode, can be modified via the 
config file.
- If you use custom weapon keybinds, set them in the config file.
If the last weapon bind you selected with the sniper bind, the 
program will autoscope and fire for 100% accuracy.
- ChromaFire should be undetectable so long as the developers do
not implement a kernel-level anti-cheat. The program does not read
or write memory that belongs to the game.

Performance Statistics:
------------------------------------------------------------------
- Pixel-to-click latency of 3.2 +- 0.2ms measured using std::chrono
library functions
- 94% accuracy (when functioning properly)

2.1 Patch Notes:
------------------------------------------------------------------
- You can now choose the trigger mode - HOLD or TOGGLE
- New "switch" interval - IGNORE if you do not play old school
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
(blindness, love, etc.) may throw off the detection and disable
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
- Fix known issues
- Code cleanup
