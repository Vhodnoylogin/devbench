# Native startup menu contract (compatibility candidate)

`menu` action `startup` exposes contract `devbench-startup-ui/1`. The only extra
argument is `startupOperation`: `snapshot`, `select-new` or `confirm-new`. Arbitrary
movie paths, method names, input strings and Papyrus scripts are not accepted.

The server marshals to the existing fenced main-thread task primitive, obtains
the actually open Main Menu movie and reads the identified stock
`_root.MenuHolder.Menu_mc` state, entries and selected entry directly through GFx.
It does not dispatch a Papyrus callback and can therefore be qualified before
the VM begins processing game scripts. Snapshot returns actual typed metadata;
unknown/missing movie data is unavailable, never fabricated defaults.

`select-new` requires state Main and exactly one enabled `$NEW` entry with native
index1. It sets the actual selected position, independently reads back that NEW
selection and invokes the identified list's item-press method once. `confirm-new`
requires MainConfirm and the actual selected enabled NEW entry, then invokes its
native movie acceptance method once. Both are checked within one main-thread task.
Wrong/changed state or selection, duplicate/missing/disabled NEW and unavailable
UI data refuse mutation. They never confirm Continue, Quit, Load or an arbitrary
dialog. Other menu operations and registered consumer handlers are unchanged.

Mutation replies retain the before snapshot and `completed:true`, meaning only
that the identified native movie invocation returned. Clients must independently
observe actual lifecycle/menu/world transitions. This is not proof that New Game
started, scripts initialized or gameplay became ready. Use the real unique SKSE
newGame event and common gameplay readiness before subject actions. Timeout may
occur after invocation started; never blindly replay select/confirm.

The actual stock SkyrimSE1.7.104 movie was inspected externally; no game asset or
external decompiler is included here. A replacement movie must satisfy the runtime
metadata contract and its own reviewed pins. The exact built compatibility DLL,
modern CommonLib dependency and effective UI assets need a separate bounded live
qualification. This source/API addition grants no launch or old-order replay.
