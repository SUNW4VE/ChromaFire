#SingleInstance, force
SetWorkingDir %A_ScriptDir%

; Optimizations
    ; Disable environment variables
	#NoEnv
    ; Disable logging
	#KeyHistory 0
	ListLines Off
    ; Set above-normal priority
	Process, Priority,, High
    ; Remove per-line latency
	SetBatchLines, -1
    ; SetDelays
	SetKeyDelay, -1, -1
	SetDefaultMouseSpeed, 0
	SetWinDelay, -1
	SetControlDelay, -1
    ; Prioritize SendInput mode for fastest inputs
	SendMode, Input

; X-Y COORDS (Center of crosshair) (Set for 1080p by default)
X = 957
Y = 538

MsgBox Launched ChromaFire+Scope ver 1.2.1. Please set ADS mode to "Hold" in-game. Resolution Mode: 1080p. Press CTRL+Q to quit.

; Get CooldownInterval from user
InputBox, CooldownInterval, CFScope, Enter a cooldown interval(for your weapon firerate) in milliseconds. The cooldown interval must be higher than the firerate in order for CFScope to function properly:, , 450, 160
if ErrorLevel {
    MsgBox, You did not enter a cooldown interval. Closing CFScope.
    ExitApp
}
MsgBox Set the cooldown interval to %CooldownInterval%ms. You may proceed.

Toggle := false
Iterations := 0

RButton::
    Toggle := true
    SetTimer, ToggleOff, -1000
    SetTimer, ChromaSearch, 0
return

RButton Up::
    Toggle := false
    SetTimer, ChromaSearch, Off
    SendInput {Click up}
return

ChromaSearch:
    while (Toggle) {
	PixelSearch, Px, Py, X, Y, X, Y, 0xffffff, 90, RGB
	if ErrorLevel {
    	    SendInput {RButton Down}
	    Send, Click
	    SendInput {RButton Up}
	    DllCall("Sleep", "UInt", CooldownInterval)
	}
	Iterations++
    }
return

ToggleOff:
    Toggle := false ; Turn off Toggle
    MsgBox, ChromaFire iterations in one second: %Iterations%
    Iterations = 0
return

^Q::
    MsgBox Closing CFScope.
    ExitApp
