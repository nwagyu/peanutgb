# Peanut-GB for NumWorks

[![Build](https://github.com/nwagyu/peanutgb/actions/workflows/build.yml/badge.svg)](https://github.com/nwagyu/peanutgb/actions/workflows/build.yml)

This app is a [Game Boy](https://en.wikipedia.org/wiki/Game_Boy) emulator that runs on the [NumWorks calculator](https://www.numworks.com). It is based off of the [Peanut-GB](https://github.com/deltabeard/Peanut-GB) emulator.

## Install the app

To install this app, you'll need to:
1. Download the latest `peanutgb.nwa` file from the [Releases](https://github.com/nwagyu/peanutgb/releases) page
2. Extract a `cartridge.gb` ROM dump from your GameBoyt cartridge, or, alternatively, use the provided `src/flappyboy.gb` file.
2. Head to [my.numworks.com/apps](https://my.numworks.com/apps) to send the `nwa` file on your calculator along the `gb` file.

## How to use the app

The controls are pretty obvious because the GameBoy's gamepad looks a lot like the NumWorks' keyboard:

|Game Boy controls|NumWorks|
|-|-|
|Arrow|Arrows|
|A|Back|
|B|OK|
|Select|Shift|
|Start|Backspace|

The following keys will change the behavior of the emulator:

|Key|Behavior|
|-|-|
|1|Use the original Game Boy color palette|
|2|Use a pure grayscale palette|
|3|Use an inverted grayscale palette|
|+|Render on the entire screen|
|-|Render at a 1:1 scale|

## Build the app

To build this sample app, you will need to install the [embedded ARM toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain) and [nwlink](https://www.npmjs.com/package/nwlink).

```shell
brew install numworks/tap/arm-none-eabi-gcc node # Or equivalent on your OS
npm install -g nwlink
make clean && make build
```
