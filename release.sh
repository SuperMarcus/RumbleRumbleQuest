#!/bin/bash

rm -f RumbleRumbleQuest.zip
zip -j RumbleRumbleQuest.zip bmbfmod.json cover.gif
find ./libs/arm64-v8a \
  ! -name 'libmodloader.so' \
  ! -name 'libcodegen_*.so' \
  ! -name 'libcustom-types.so' \
  ! -name 'libquestui.so' \
  -type f -exec zip -j RumbleRumbleQuest.zip {} +
