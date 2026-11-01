#!/bin/bash
# xenstore-unplug.sh <frontend-domid> [devid]
# Unplug a device from a Xen frontend domain.

D=$1
ID=${2:-0}
T=xentoy

BE=/local/domain/0/backend/$T/$D/$ID
FE=/local/domain/$D/device/$T/$ID

# Unregister the backend device first. Write to xenstore
xenstore-write $BE/online 0
xenstore-write $BE/state 5

# Closing: ask both ends to tear down
# wait until both states read 6, then:
# xenstore-rm $FE; xenstore-rm $BE
