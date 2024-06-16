ChromaFire - a triggerbot for Pixel Gun 3D
------------------------------------------------------------------

How it works:
------------------------------------------------------------------
- Detection is active whenever the game window is focused and the
**toggle** is ON. The toggle key is by default E, but can be
changed to a different keybind via the config file.
- When detection is active, ChromaFire repeatedly scans the center
of your weapon crosshair. It sends a click whenever that pixel is
a close enough shade of red. The threshold and tolerance of this
shade can be modified via the config file.
- If you use custom weapon keybinds, set them in the config file.
If the last weapon bind you selected with the sniper bind, the 
program will autoscope and fire for maximum accuracy.
- ChromaFire should be undetectable so long as the developers do
not implement a kernel-level anti-cheat. Nothing is injected into 
the game files; the program simply scans a certain pixel from your 
monitor screen

Update Notes:
------------------------------------------------------------------
- The program has been fully rewritten from AutoHotkey to C++
- Detection is significantly faster than ChromaFire 1.2
- Positive detection to click latency is significantly more 
responsive than ChromaFire 1.2
- ChromaFire is now based off a switch toggle instead of a hold 
toggle
- Customizable toggle keybind
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
(blindness, love, etc.) throw off the detection and disable
ChromaFire for the length of their effect
- Low detection intervals mean that rapid clicks will be sent when
ChromaFire detects positive
- ChromaFire breaks when toggle bind is set to a mouse click 
button. Currently, there is error handling to prevent users from 
setting it that way
- ChromaFire sleep interval for one weapon category can "creep" 
into another weapon category, causing the latter to not fire when
ready
- ChromaFire only functions properly with weapons that have a 
center dot with their crosshair. This is how the program detects 
enemies by nature and I am not planning to fix this

Roadmap:
------------------------------------------------------------------
- Fix known issues
