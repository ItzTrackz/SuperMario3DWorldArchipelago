## Build

From a devkitPro/MSYS2 terminal:

    make

or:

    make DEBUG=1

## Install

Copy:

    SM3DWPlugin.wps

to:

    sd:/wiiu/environments/<your-aroma-environment>/plugins/

The plugin displays a notification when the NotificationModule is available.

## Important

This is only the plugin foundation. It contains no guessed SM3DW function
addresses. The next step is reverse-engineering your exact `RedCarpet.rpx`
and adding a verified hook.
