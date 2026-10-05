# Skyrim VR object-array metadata correction

This maintenance branch is based on DevBench v1.25.0, commit
91590a9d3c81208c7efa5614e18a20d4f1caf9d1. DevBench remains the work of
its upstream authors; this fork preserves their licenses and credits.
It is a compatibility candidate, not an upstream release.

The correction lives in the pinned CommonLib submodule: upstream CommonLibVR
abe9ca7b7318dbc04bccfcd59fc3fb670244c2d7, fork commit
9106d402cfc7dcbc5bf7458be6748af19d7fc914. This network is exposed on GitHub
as Vhodnoylogin/CommonLibSSE-NG. The changed expression clears only the object
tag bit when decoding an object-array class pointer. Clearing enum11 destroyed
address bit3 for valid eight-byte-aligned class objects. No game addresses,
SDK layouts or DevBench public API were changed.

A Skyrim VR crash stack reached TypeInfo::TypeAsString through Papyrus
DescribeFunction while describing CreateEnchantment's EffectSetting[] argument.
The original compiled decoder failed the aligned synthetic pointer control;
the corrected compiled decoder passed eight object/object-array cases.
The included typeinfo-regression.cpp and optional xmake target retain that
regression. Existing DevBench native tests passed 203 cases. Eight subsequent
bounded minimal New Game launches passed required metadata checks without the
previous describe crash, with restored environments. Other prior HTTP failures
are not all attributed to this bug, and combined Body Pouches acceptance is
still pending.

Clone this branch recursively and follow the upstream Windows build instructions.
Use xmake3.1.1 and Visual Studio C++23 tools. The optional decoder target is
devbench-typeinfo-regression. The host build's generated dependency lock, SDKs,
binaries, game data and private logs are not added by this fork. The independent
executor at https://github.com/Vhodnoylogin/skyrim-autotest also includes the
pinned acquisition/correction/native-control recipe tools/build_devbench_compat.py.
Only stage a paired DLL/PDB with recorded hashes for authorized tests; retain
and restore the installed original. GPL-3.0-or-later applies to derived binaries.
