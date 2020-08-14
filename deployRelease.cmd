XCOPY .\Sources\Windows\x64\Release\notchStreamDeck.exe "C:\Program Files\Elgato\StreamDeck\Plugins\com.elgato.notch.sdPlugin" /Y
XCOPY .\Sources\Windows\x64\Release\notchStreamDeck.exe .\Sources\com.elgato.notch.sdPlugin /Y

XCOPY .\Sources\com.elgato.notch.sdPlugin\*.* "C:\Program Files\Elgato\StreamDeck\Plugins\com.elgato.notch.sdPlugin" /E /D /C /Y