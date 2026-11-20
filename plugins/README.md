# Root-level plugin mirror

This directory mirrors the verified Linux 32-bit plugin files that were downloaded and validated from upstream releases in this environment.

The files here are not a substitute for the target VM/server installation, but they document the current repo state for the 7780 runtime path.

## Present and verified

- crashdetect.so - downloaded from the upstream crashdetect v4.22 Linux release
- streamer.so - downloaded from the upstream samp-streamer-plugin v2.9.6 release
- sscanf.so - downloaded from the upstream sscanf v2.13.8 Linux release
- mysql.so - downloaded from the upstream samp-plugin-mysql 2.1.1 release

## Not yet verified in this repository as real Linux runtime modules

- sqlite
- zcmd
- foreach
- MapAndreas
- pawn-plus

These remain pending on the actual target VM or host where the 32-bit Linux SAMP runtime is installed. No placeholder or fake binaries are committed.
