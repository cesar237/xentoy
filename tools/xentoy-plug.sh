#!/bin/bash
# xentoy-plug.sh <frontend-domid> [devid]
# Plug a device into a Xen frontend domain.

D=$1
ID=${2:-0}
T=xentoy

BE=/local/domain/0/backend/$T/$D/$ID
FE=/local/domain/$D/device/$T/$ID

# Register the backend device first. Write to xenstore
xenstore-write $BE/frontend $FE 
xenstore-write $BE/frontend-id $D
xenstore-write $BE/online 1
xenstore-write $BE/state 1
xenstore-chmod -r $BE -n0 r$D # Owner dom0, read permission for frontend domain

# Frontend device setup
xenstore-write $FE/backend $BE
xenstore-write $FE/backend-id 0
xenstore-write $FE/state 1
xenstore-chmod -r $FE -n$D r0 # Owner frontend domain, read permission for dom0