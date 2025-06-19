#!/bin/bash
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 1024M -audiodev pa,id=speaker -machine pcspk-audiodev=speaker
