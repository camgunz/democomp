#!/bin/sh

set -e

( cd build && make )

build/democomp pr+dump.bin d2kdump.bin

