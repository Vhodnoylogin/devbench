# Native startup menu contract (compatibility candidate)

`menu` action `startup` exposes contract `devbench-startup-ui/1`. The only extra
argument is `startupOperation`: `snapshot`, `select-new`, `confirm-new`,
`select-quit` or `confirm-quit`. The QUIT operations are additive to the same
`devbench-startup-ui/1` contract; existing NEW requests retain their behavior. Arbitrary
movie paths, method names, input strings and Papyrus scripts are not accepted.

The server uses the deadline/abandonment fence on SKSE's UI task queue, obtains
the actually open Main Menu movie and reads the identified stock
`_root.MenuHolder.Menu_mc` state, entries and selected entry directly through GFx.
It does not dispatch a Papyrus callback and can therefore be qualified before
the VM begins processing game scripts. Snapshot returns actual typed metadata;
unknown/missing movie data is unavailable, never fabricated defaults.

`select-new` requires state Main and exactly one enabled `$NEW` entry with native
index1. It sets the actual selected position, independently reads back that NEW
selection and invokes the identified list's item-press method once. `confirm-new`
requires MainConfirm and the actual selected enabled NEW entry, then invokes its
native movie acceptance method once. Both are checked within one UI task.
Wrong/changed state or selection, duplicate/missing/disabled NEW and unavailable
UI data refuse mutation. NEW operations never confirm Continue, Quit, Load or an
arbitrary dialog. Other menu operations and registered consumer handlers are unchanged.

For an explicitly authorized shutdown, the caller first returns the loaded world
to the native Main Menu through its separate game operation and observes the
actual open menu. `select-quit` requires state Main and exactly one enabled
`$QUIT` entry with native index9. It selects the actual list position, reads back
that exact QUIT selection, and invokes the same identified item-press method once.
`confirm-quit` requires MainConfirm and the actual selected enabled `$QUIT`/index9
entry, then invokes the movie's acceptance method once. The inspected stock
movie routes this acceptance to `GameDelegate.QuitToDesktop`. Wrong NEW/Continue/
Load selection, changed state, duplicate/missing/disabled QUIT or unavailable
metadata refuse mutation. These operations neither return a world to Main Menu
themselves nor authorize automatic startup selection or a game launch.

Mutation replies retain the before snapshot and `completed:true`, meaning only
that the identified native movie invocation returned. Clients must independently
observe actual lifecycle/menu/world transitions and, for shutdown, the exact
owned process's actual exit code0. An invocation response alone does not prove
process termination or a successful normal exit. This is not proof that New Game
started, scripts initialized or gameplay became ready. Use the real unique SKSE
newGame event and common gameplay readiness before subject actions. Timeout may
occur after invocation started; never blindly replay a NEW or QUIT select/confirm.

The actual stock SkyrimSE1.7.104 movie was inspected externally; no game asset or
external decompiler is included here. A replacement movie must satisfy the runtime
metadata contract and its own reviewed pins. The exact built compatibility DLL,
modern CommonLib dependency and effective UI assets need a separate bounded live
qualification. This source/API addition grants no launch or old-order replay.
