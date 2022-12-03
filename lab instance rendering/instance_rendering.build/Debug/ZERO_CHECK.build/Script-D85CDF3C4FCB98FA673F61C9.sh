#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd "/Users/hanktsai/Documents/CSC471/lab instance rendering"
  make -f /Users/hanktsai/Documents/CSC471/lab\ instance\ rendering/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd "/Users/hanktsai/Documents/CSC471/lab instance rendering"
  make -f /Users/hanktsai/Documents/CSC471/lab\ instance\ rendering/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd "/Users/hanktsai/Documents/CSC471/lab instance rendering"
  make -f /Users/hanktsai/Documents/CSC471/lab\ instance\ rendering/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd "/Users/hanktsai/Documents/CSC471/lab instance rendering"
  make -f /Users/hanktsai/Documents/CSC471/lab\ instance\ rendering/CMakeScripts/ReRunCMake.make
fi

