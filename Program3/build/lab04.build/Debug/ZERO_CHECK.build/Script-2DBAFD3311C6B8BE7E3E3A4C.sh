#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd "/Users/hanktsai/Documents/CSC471/lab anim/build"
  make -f /Users/hanktsai/Documents/CSC471/lab\ anim/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd "/Users/hanktsai/Documents/CSC471/lab anim/build"
  make -f /Users/hanktsai/Documents/CSC471/lab\ anim/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd "/Users/hanktsai/Documents/CSC471/lab anim/build"
  make -f /Users/hanktsai/Documents/CSC471/lab\ anim/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd "/Users/hanktsai/Documents/CSC471/lab anim/build"
  make -f /Users/hanktsai/Documents/CSC471/lab\ anim/build/CMakeScripts/ReRunCMake.make
fi

