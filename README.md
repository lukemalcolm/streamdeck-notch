# streamdeck-notch

Stream Deck Plugin for controlling Notch playback & values using OSC.

This plugin enables you to control playback of Notch Standalone and send OSC values (both numbers and text) into Standalone & Builder.

_Side note: The plugin can be used to send to any OSC device, it's just pre-configured with all the OSC addresses for Notch._

The plugin is written with: 
* Javascript, CSS & HTML for the app GUI
* C++ for functions of the plugin.

_This is an unofficial opensource plugin and support is not provided by the Notch team._

# Download Plugin Releases

Pre-compiled releases for Windows are at [https://github.com/lukemalcolm/streamdeck-notch/releases](https://github.com/lukemalcolm/streamdeck-notch/releases)

To use: 
1. Download the release file.
2. Browse to it in File Explorer
3. Double click on it to install

# Plugin Features

Buttons:

* Momentary Button (float)
* Toggle Button (float)
* Text String Button
* Restart Timeline*
* Go to time (secs)*
* Switch layer*
* Play*
* Pause*

** *IMPORTANT: Transport commands (Play, Pause, Switch Layer, Go To Time, Reset Timeline) only work in Standalone not Builder.**

The plugin allows has global settings for target IP and Port.

# Configuring your Notch project

To use the plugin within your Notch project, enable OSC and OSC Transport Control:

1. Menu->Project Settings
1. Go to Protocols tab
   1. Tick: OSC Enabled
   1. Tick: Transport

**IMPORTANT: The playhead MUST be playing for OSC to be received in Builder.**

To monitor incoming OSC into Notch:

1. Menu->View->Connection Monitor
1. Pane will appear that shows inbound OSC traffic

# Compiling the project

At present the project is Windows only and tested for compilation on Visual Studio 2019. The project file and solution are in the Windows folder.

The VS project has two targets: Release & Debug that both build to: `Sources\com.elgato.notch.sdPlugin\notchStreamDeck.exe`

To use the plugin while developing you must copy all files (including the .exe icons & manifest.json) within `Sources\com.elgato.notch.sdPlugin` to `C:\Program Files\Elgato\StreamDeck\Plugins\com.elgato.notch.sdPlugin`. There are two scripts DeployDebug.cmd and DeployRelease.cmd that make this quick and easy.

NB: Stream Deck must be fully closed to be able to overwrite files in the `StreamDeck\Plugins` directory.

# File structure

Key files in Sources directory:

* `com.elgato.notch.sdPlugin\Sources\Windows\com.elgato.notch.sdPlugin.sln/xcproj` - Visual Studio Solution & Project Files
* `com.elgato.notch.sdPlugin\manifest.json` - Configuration file StreamDeck app. Sets the actions / icons etc.
* `com.elgato.notch.sdPlugin\pi.html` - The Property Inspector HTML file that renders the GUI in Stream Deck app.
* `com.elgato.notch.sdPlugin\pi.js` - The Property Inspector JS logic, which receives and sends messages to the Stream Deck app via WebSockets and acts on the HTML.
* `Windows\NotchStreamDeckPlugin.h/cpp` - C++ plugin app. Inherits class from ESDBasePlugin. Receives and Sends actions to the Stream Deck app over WebSockets.
* `Windows\NotchOSCActions.h/cpp` - Looks after the actual sending of OSC messages.
* `Common` - Contains the boilerplate for Stream Deck plugins.

# Debugging

[Elgato SDK Debugging Documentation](https://developer.elgato.com/documentation/stream-deck/sdk/create-your-own-plugin/)

To debug the Property Inspector UI via Chrome, point a browser to: `http://localhost:23654/` while Stream Deck app is running and showing a Notch plugin button configuration dialog. See more at: 

To debug the C++ code:
* Add Registry Setting: 
  * Location: `HKEY_CURRENT_USER\Software\Elgato Systems GmbH\StreamDeck`
  * Key name: `html_remote_debugging_enabled`
  * Type: `DWORD`
  * Value: `1`
* Build a Debug target of notchStreamDeck.exe and copy it into the plugin directory: `C:\Program Files\Elgato\StreamDeck\Plugins\com.elgato.notch.sdPlugin `
* Start Stream Deck app
* Attach the Visual Studio debugger to the notchStramDeck.exe process.

# Packaging your own release

When developing, you'll be putting files directly into your plugin folder. However if you wish to package your release into a plugin package for distribution, you'll need to use the [Distribution Tool](https://developer.elgato.com/documentation/stream-deck/sdk/exporting-your-plugin/).

Command (run from Sources directory): `DistributionTool.exe -b -i com.elgato.notch.sdPlugin -o ..\Release`

# Libraries used

This project uses the following opensource libraries:

* ASIO from Boost
* [JSON for Modern C++](https://github.com/nlohmann/json/releases)
* [!oscpp library](https://github.com/kaoskorobase/oscpp) by Stefan Kersten <sk@declaredvolatile.org> 
* [Websocket++](http://www.zaphoyd.com/websocketpp/)

