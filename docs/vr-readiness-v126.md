# VR readiness maintenance candidate (DevBench 1.26.0)

This branch merges upstream v1.26.0 (6b4b7ddd5c7d5064557c2edcccc188ac8b8f86f6)
into our metadata correction branch. The CommonLib fork pin remains
9106d402cfc7dcbc5bf7458be6748af19d7fc914: object-array class pointers clear
only tag bit0. See typeinfo-compatibility-fork.md for the retained crash evidence.

A separate Body Pouches attempt loaded QASmoke successfully, but 87 scene reads
failed while dumping invalid UTF-8 (type_error.316, incomplete byte0xC7).
The actual source field/codepage were not captured. A localized display string
must not make cell identity, position and every other observation inaccessible.

ToolRegistry now validates results once the handler completes. Valid UTF-8
results keep their content and shape. Invalid string values in object results
become null (unavailable text) and the top-level textEncodingDiagnostics records
JSON-pointer paths, unknown source encoding, original byte lengths and raw hex
prefixes. At most32 records of128 raw bytes each are retained; explicit flags
report omitted records/bytes. Numeric values and valid identities remain intact.
No CP1251/system-codepage guess, replacement characters, input conversion,
handler repeat or silently dropped bytes are used. Clients must not count null
text as an empty/valid name or a passing text assertion.

Invalid object keys, invalid scalar/array roots and a collision with the reserved
diagnostic field fail explicitly instead of renaming keys or changing result
shape. This correction covers registry tool results used by both REST and MCP,
including internal scenarios; it is not a complete encoding policy for event
payloads, tool registration descriptors, engine input or previously written files.
The public extension ABI and its headers are unchanged.

Tests reproduce the observed final0xC7 byte; preserve Russian UTF-8 and embedded
NUL; cover nested arrays/pointer escaping, bounds, overlong/surrogate/out-of-range
UTF-8, refused key/root collisions and a single handler call followed by REST-
compatible JSON and MCP content serialization. Native tests and the actual
compiled CommonLib decoder regression must pass before staging a matching
DLL/PDB pair. Live Body Pouches acceptance remains pending until a collected run.

Clone this branch recursively and follow the upstream Windows/Xmake instructions.
Use xmake3.1.1 with Visual Studio C++ tools, build/run devbench-tests and
devbench-typeinfo-regression, then build devbench with deployment disabled
(unset SkyrimPluginTargets). Keep original installed files intact; the external
executor stages/restores the pinned candidate in an isolated run. Generated host
dependency locks, third-party tools, binary output, credentials and game logs
stay outside the published source changes. Upstream licensing/credits apply.
