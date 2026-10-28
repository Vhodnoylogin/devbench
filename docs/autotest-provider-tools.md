# Autotest provider tools: compatibility candidate

This fork carries three separately authored native modules staged from the
independent Skyrim Autotest repository. They register with the existing
`ToolRegistry`; the ordinary MCP and REST adapters expose them on the existing
local endpoint. They add no server, launcher, microphone or voice relay.

Source, a successful build and simulated-provider tests establish implementation
behavior only. This composed host is a candidate, not a runtime-qualified platform.
Polygon must qualify its exact installed DLL composition, native interfaces,
owned lifecycle and independent gameplay evidence before selecting it for a mod
order. Previous DevBench qualification does not automatically cover these additions.

The28October source update is authored in Skyrim Autotest commit4b83d7b
(`native/devbench/`). It adds `npc_persistence` visibility_state/set_visibility
for persistent dynamic NPCs: exact provider/VM/runtime/cell/world guards,
one consumed mutation UUID, Disable/Enable(false), and fresh disabled/loaded3D
observations. Only the external executor's owned creation registry admits targets.
No inventory reset, deletion or preset mutation occurs. Runtime-handle stability
across hide/show remains a required live qualification. The morph provider admits
at most12actors; the external platform retains six unless explicitly opted in.
The existing six-reference owned save/load contract is unchanged.

| Authored module | Tool | Responsibility |
| --- | --- | --- |
| `MorphProvider.cpp/.h` | `morph_provider` | Official OBody API control/catalog and bounded supplementary SKEE fixture-key mutations |
| `NPCPersistence.cpp/.h` | `npc_persistence` | Persistent reference/VM identity, actual equipment/name/position inspection, reference-only naming and SKSE co-save remapping |
| `FixtureConfig.cpp/.h` | `fixture_config` | Read the actual fixed OBody distribution file through the current game VFS |

The independent World Observer supplies read-only observations. It does not own
these mutation functions. Domain sequencing, business assertions, actual fixture
creation witnesses and game/process/profile ownership remain in the external
executor and the origin's pinned subject plan.

## Reproduce source staging

The authored source and primary contracts live in the Autotest repository:
`native/devbench/`, `docs/morph-provider.md`, `docs/npc-persistence.md`,
`docs/interactive-bindings.md` and `tools/apply_devbench_morph_provider.py`.
Keep the host checkout, acquired dependency headers and generated output separate
from that repository. This document does not replace the host's existing SDK,
submodule or dependency-lock instructions.

The staging tool requires host HEAD
`5068162637dc2b5621b7224b8eded76a0c9e3d4f`. It verifies the original or already
identical reviewed host seams before writing. It copies the six authored source
files, registers the tools in `src/Tools.cpp`, forwards SKSE messages from
`src/main.cpp`, initializes NPC serialization after `SKSE::Init`, and adds the
external provider-header include directory in `xmake.lua`. Foreign seam edits are
refused. The returned inventory identifies the staged file hashes; it is not a
binary build, installation or live-test receipt.

Acquire these unmodified official headers into an external directory. Save the
upstream OBody `API.h` file as `OBodyAPI.h` without changing its bytes.

| Local include name | Official source pin | Required SHA-256 |
| --- | --- | --- |
| `IPluginInterface.h` | [expired6978/SKSE64Plugins, `skee64/IPluginInterface.h`](https://github.com/expired6978/SKSE64Plugins/blob/9ebcb733e17be695f994cd2e9cc383043446bc02/skee64/IPluginInterface.h), commit `9ebcb733e17be695f994cd2e9cc383043446bc02` | `875410dd22230f76a014a6841004e7d346f5fa1c748cfff695f9f495234da277` |
| `OBodyAPI.h` | [Aietos/OBody-NG, `include/API/API.h`](https://github.com/Aietos/OBody-NG/blob/13513609145313f721970c2ebe113290f6add6e8/include/API/API.h), commit `13513609145313f721970c2ebe113290f6add6e8` | `f04945202d9daa6408d7555e46eb2a553a1041ab5d84a8f4efc288168563ea18` |

The hashes refer to the official raw LF bytes. A CRLF checkout can be textually
equivalent and still have a different, rejected byte identity. The staging tool
checks both hashes. It does not download or vendor the headers.

Run the staging tool from the separately pinned Autotest checkout:

```text
python "<autotest-source>/tools/apply_devbench_morph_provider.py" --source "<external-host>" --provider-api-dir "<external-header-directory>"
```

For the later host build, point `AUTOTEST_PROVIDER_API_DIR` to that same external
header directory, or stage those verified files in the existing default
`build/external/provider-api` include directory. Record the actual compiler,
SDK/submodule identities, authored source inventory, external header hashes and
resulting DLL hash in the build evidence. Do not treat the host commit or header
pins as proof of which provider DLL is loaded in the game.

## ABI and morph control

The code includes the official external declarations directly. It negotiates
OBody plugin API v1 and SKEE BodyMorph v5 at SKSE `kPostPostLoad`; missing or
different interfaces remain unavailable. It does not reproduce a private vtable
or assume a similarly named Papyrus function has the same ABI.

OBody interface pointers and the readiness listener have process lifetime. The
module borrows them and does not delete them. Readiness invalidation and API use
share a gate, including the becoming-unready callback before provider storage
changes. Returned string views are copied while that gate is held. A ready
interface is not evidence of automatic distribution, applied morphs or rendering.

`capabilities`, paged `catalog`, `actor_status` and `action_status` are observations.
Catalog pages are bounded to 256 names and report the actual sex/blacklist counts.
Actor status reads the real assignment/processed/blacklist API state. Automatic
origin requires the actual stock-generation event and independent evidence.

Mutations require an exact prior actor/base/race/runtime-handle/model/process/world
guard, a loaded non-deleted dynamic FF actor other than the player, and separate
32-character lowercase hexadecimal owner/command tokens. Actor/root lifetime is
retained during a mutation. At most six references and three supplementary test
keys are admitted per world. Model identity is an opaque sampled token; it cannot
detect every unobserved destruction/reuse event and is not renderer continuity.

Supported actions call the real assignment, empty-name unassignment and
`RemoveOBodyMorphsFromActor` APIs, or write/remove only the explicitly submitted
SKEE fixture keys. Supplementary keys must contain `.Test.`, names are bounded to
128 printable ASCII bytes, and coefficients must be finite in -2..2. The domain
adapter additionally restricts these to its frozen plan keys. Reset does not
substitute an arbitrary morph clear, assignment rewrite or direct actor generation.

Owner/command tokens are consumed before queueing and retained across loads for
the process lifetime. The 512-receipt bound does not evict tokens to allow replay.
Receipts distinguish queueing, rejected/abandoned-before-call, provider-call start,
returned calls and unknown outcomes. A started call can finish after HTTP expiry;
poll its existing receipt rather than resubmitting it. Provider acceptance and
`completion:unobserved` do not pass a subject assertion.

## Persistent NPCs and save/load identity

The external executor must first prove that it created each disposable reference
in this run. A dynamic FF ID, matching base or this native tool's persistent flag
cannot establish ownership by itself. Persistent creation uses the explicit
`PlaceAtMe(..., forcePersist=true, ...)` path and checks actual reference/VM
identity before and after placement.

`identity` and `inspection` return actual current reference/base, VM handle,
runtime handle, PID and native provider-session identity. VM handles use exact
decimal uint64 strings to avoid JSON number precision loss. Inspection reads
the actual display name, position, all armor slots30..61, equipped hand forms,
native weapon-type flags and the typed movement-block flag. The collision-command
counter is not a measurement of global collision state or physical contacts.

Native `prepare` accepts only reference-name preparation, with a unique consumed
request UUID and a bounded name. It calls the reference display-name setter and
retains the actual readback. It does not edit the base NPC or outfit. Unequip and
hold use the external executor's separately frozen typed `Actor.UnequipAll` and
`Actor.SetDontMove` bindings. No item-removal or collision-disable command is
substituted. The executor records the exact command ledger and actual position
samples; a stationary endpoint interval does not prove unobserved motion absent.

`bind` establishes one native identity bundle with immutable run/save/binding and
creation IDs. SKSE serialization uses unique ID `0x5356544E`, record type
`0x4E504350`, record version1, and a bounded 64KiB payload. The native record admits
at most sixteen rows; the external six-NPC route requires exactly six, one owned
save pair and one load attempt. The native tool itself does not execute save/load.

At save time, the callback freshly checks each actor and serializes its actual
current FormID and VM handle. On load, it invokes both SKSE `ResolveFormID` and
`ResolveHandle`, retains the resolved VM handles and preserves both mappings in
the receipt. A fresh native check resolves the mapped actor, verifies the base
and persistent state, and proves the mapped VM handle identifies that exact
current actor. Old FF IDs or cached base/actor records are not substitutes.

The external owner separately requires the unchanged exclusive-readable ESS/SKSE
hash pair, ordered native pre/post-load events, the same owned process, common
gameplay readiness and later Observer load generations. It invalidates all old
bindings before load and publishes all six replacements atomically after every
native/Observer witness succeeds. The held companion accepts only the exact
outstanding delivered same-process load command, updates its bootstrap once,
and preserves the original session deadline. Its load bound is at most90seconds;
this does not extend a shorter action, command claim or original held window.

For an MO2 local-save alias, `mapping_probe` reads an exact random32-byte nonce
created in the owned physical save directory before launch. It derives the native
default save directory from the actual game setting and user directory. The
external owner compares the PID, path, tokens and length; it does not invent an
ESS file to establish mapping for a fresh New Game.

## Fixed configuration observation

`fixture_config` accepts only `action:"obody"` and a bounded timeout in1..3000ms.
It derives the actual executable directory and reads
`Data/SKSE/Plugins/OBody_presetDistributionConfig.json` through the game's VFS.
It validates JSON and returns up to1MiB of raw text with the path, PID and byte
count. It accepts no caller-selected path and performs no writes.

The returned `inMemoryConfigObserved:false` is deliberate. Visible file bytes do
not prove that a provider already parsed or applied that configuration. Exact
staging/startup evidence and genuine generation events establish that separately.

## Qualification boundary

Missing metadata, stale readiness/world identity, native remap failures, incomplete
equipment observations and unknown mutation outcomes remain explicit failures or
unavailable evidence. They never become default zero values or permission to retry.
Morph arithmetic, foreign-key preservation, linked/suppressed state, restored
weights, visible geometry and rendering require independent actual observations.
Physics/contact behavior and a live voice control chain are outside this candidate.

This document records source contracts and reproduction inputs. It is not an
installation instruction, launch authorization, test result or owner acceptance.
