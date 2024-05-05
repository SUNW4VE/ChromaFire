ChromaFire - a triggerbot for Pixel Gun 3D
------------------------------------------------------------------
Created by: Carimatic
Discord: cari.matic
------------------------------------------------------------------
Version: 1080p, 1.2
------------------------------------------------------------------
How it works:
- Detection is active whenever RMB is held down.
- ChromaFire repeatedly scans the center of your weapon crosshair. 
It sends a click whenever that pixel is NOT a close enough shade 
of white.
- ChromaFire, especially the .exe version, should be undetectable
as long as the developers don't implement a kernel-level anti-cheat.
Nothing is injected into the game files; the program simply scans
a certain pixel from your monitor screen
------------------------------------------------------------------
Performance statistics:
- ~240 detections per second when the crosshair is white
- ~70 detections per second when the crosshair is not white
- Detection-click latency is 5-15ms
- The above stats were tested using an AMD Ryzen 9 5900X
------------------------------------------------------------------
Known issues:
- Weapon effects that alter the chromatic nature of your screen
(blindness, love, etc.) throw off the detection and causes 
ChromaFire to send a false trigger
- Teammates and pets cause ChromaFire to send a false trigger, due 
to the crosshair turning blue when placed over them
- Standard ChromaFire (not the scope version) sends a click ~70
times per second when it detects positive, which may trigger a
future anti-cheat system
- ChromaFire only works properly with weapons that have a center
dot with their crosshair. This is how the program detects enemies
by nature and I am not planning to fix this
------------------------------------------------------------------
Roadmap:
- Fix known issues
- Toggle RMB instead of hold RMB (?)
- Program ChromaFire to automatically determine the center of the 
crosshair's pixel coordinates (to support multiple monitor
resolutions)