#!/bin/bash

SYSTEM_WSI=$(loginctl show-session $(loginctl | grep $(whoami) | awk '{print $1}') -p Type | cut -d'=' -f2)

case $SYSTEM_WSI in
    'x11')
        VKB_WSI='XLIB'
        ;;
    'xcb')
        VKB_WSI='XCB'
        ;;
    'wayland')
        VKB_WSI='WAYLAND'
        ;;
    'd2d')
        VKB_WSI='D2D'
        ;;
esac

mkdir -p build
cmake -S . -B build -DVKB_WSI_SELECTION=$VKB_WSI
