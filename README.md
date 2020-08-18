# streamdeck-notch

Stream Deck Plugin for controlling Notch playback & values using OSC.

This plugin enables you to control playback of Notch Standalone and send OSC values (both numbers and text) into Standalone & Builder.

**IMPORTANT: Transport commands (Play, Pause, Switch Layer, Go To Time, Reset Timeline) only work in Standalone not Builder.**

To use the plugin within your Notch project, enable OSC and OSC Transport Control:

1. Menu->Project Settings
1. Go to Protocols tab
   1. Tick: OSC Enabled
   1. Tick: Transport

**IMPORTANT: The playhead MUST be playing for OSC to be received in Builder.**

To monitor incoming OSC into Notch:

1. Menu->View->Connection Monitor
1. Pane will appear that shows inbound OSC traffic

Side not: You can actually use the plugin to send to any OSC device, it's just pre-configured with all the OSC addresses for Notch.

# Pre-built Plugin Releases

Pre-compiled releases for Windows are at [https://github.com/lukemalcolm/streamdeck-notch/releases](https://github.com/lukemalcolm/streamdeck-notch/releases)

# Compiling the project

At present the project is Windows only and tested for compilation on Visual Studio 2019. The project file and solution are in the Windows folder.

# File structure

Key files in Sources directory:

* com.elgato.notch.sdPlugin\manifest.json - Configuration file StreamDeck app. Sets the actions / icons etc.
* com.elgato.notch.sdPlugin\pi.html - The Property Inspector HTML file that renders the GUI in Stream Deck app.
* com.elgato.notch.sdPlugin\pi.js - The Property Inspector JS logic, which receives and sends messages to the Stream Deck app via WebSockets and acts on the HTML.
* Windows\NotchStreamDeckPlugin.h/cpp - C++ plugin app. Inherits class from ESDBasePlugin. Receives and Sends actions to the Stream Deck app over WebSockets.
* Windows\NotchOSCActions.h/cpp - Looks after the actual sending of OSC messages.
* Common - Contains the boilerplate for Stream Deck plugins.

# Libraries used

This project uses the following opensource libraries:

* ASIO from Boost
* [JSON for Modern C++](https://github.com/nlohmann/json/releases)
* [!oscpp library](https://github.com/kaoskorobase/oscpp) by Stefan Kersten <sk@declaredvolatile.org> 
* [Websocket++](http://www.zaphoyd.com/websocketpp/)

# Packaging your own release

When developing, you'll be putting files directly into your plugin folder. However if you wish to package your release into a plugin package for distribution, you'll need to use the [Distribution Tool](https://developer.elgato.com/documentation/stream-deck/sdk/exporting-your-plugin/).

Command (run from Sources directory): DistributionTool.exe -b -i com.elgato.notch.sdPlugin -o ..\Release