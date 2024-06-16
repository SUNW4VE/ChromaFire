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

MsgBox, Launched ChromaFire ver. 1.2. Resolution Mode: 1080p. Press CTRL+Q to quit.

Toggle := false

RButton::
    Toggle := true
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
	    SendInput {Click, down}
    	    DllCall("Sleep", "UInt", 1.1)
	    SendInput {Click, up}
	}
    }
return

^Q::
    MsgBox Closing ChromaFire.
    ExitApp
