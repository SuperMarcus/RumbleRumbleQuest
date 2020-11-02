#!/bin/bash

adb shell am force-stop com.beatgames.beatsaber

adb push libs/arm64-v8a/librumblerumblequest*.so /sdcard/Android/data/com.beatgames.beatsaber/files/mods/
