#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd "/Users/hanktsai/Documents/CSC471/lab2 base code/build"
  echo Build\ all\ projects
fi
if test "$CONFIGURATION" = "Release"; then :
  cd "/Users/hanktsai/Documents/CSC471/lab2 base code/build"
  echo Build\ all\ projects
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd "/Users/hanktsai/Documents/CSC471/lab2 base code/build"
  echo Build\ all\ projects
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd "/Users/hanktsai/Documents/CSC471/lab2 base code/build"
  echo Build\ all\ projects
fi

