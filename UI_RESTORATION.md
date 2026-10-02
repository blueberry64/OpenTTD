# Personal UI restoration checklist

This is the implementation backlog for this personal OpenTTD fork. It records
the owner's choices from the UI history review. Numbers in parentheses refer
to that review, not the execution order. Follow-up requests are identified
separately. Tasks 01–13 are complete. Task 14 is optional and deferred.
Tasks 15–17 are requested follow-ups, queued for a later implementation pass.

## How to work through this file

- Make forward edits to the current code. Use historical patches as references;
  do not use `git revert`.
- Each top-level checkbox is one independently working change and one commit.
  The order below is the suggested execution order, not a requirement to do
  everything in one session.
- Complete the implementation and relevant checks before checking off a task.
  Update its completion note in the same commit, recording what was verified
  and any remaining limitation. Do not mark untested UI behaviour as verified.
- Compile affected code and language resources as appropriate. For layout and
  interaction changes, exercise the affected windows in-game when possible,
  including small window sizes and the scenario editor where applicable.
- Keep unrelated work out of each commit. Avoid adding tests that merely
  duplicate widget declarations; use focused tests where logic warrants them.
- Default changes apply when configuration values are absent. Preserve explicit
  existing preferences; note if an existing config needs an adjustment to see
  a new default. Verify defaults with a temporary config, not by overwriting
  the owner's normal config.
- Preserve current gameplay, save compatibility, and the decisions in
  "Leave as-is" below. This is a personal fork; these changes need not be
  designed for upstream acceptance.

## Implementation queue

- [x] **01 — Default to traditional toolbar selection (review 1).**
  Set `gui.toolbar_dropdown_autoselect` to `true` by default. Keep the option
  available. A normal click/release and a held click followed by menu selection
  should behave as they did before the stay-open default.
  Entry point: `src/table/settings/gui_settings.ini`; behaviour in
  `src/toolbar_gui.cpp`. Reference: `3a5eaf367a`.
  Suggested commit: `Change: Default to traditional toolbar menu selection`
  Completion: release build passed; fresh-config runtime saved the traditional
  selection default as true. Existing explicit preferences are preserved.

- [x] **02 — Default to the traditional sprite font (review 8).**
  Default `prefer_sprite_font` to `true`; retain font configuration controls.
  Check startup, settings, and representative windows for correct text sizing.
  Entry points: `src/table/settings/misc_settings.ini`, `src/fontcache.cpp`.
  Suggested commit: `Change: Default to the traditional sprite font`
  Completion: release build passed; fresh-config startup uses the sprite font.
  Main menu, settings and representative game windows visually checked at 1x.

- [x] **03 — Default interface scale to 1x / 100% (review 9).**
  Default `gui_scale` to `100` instead of automatic detection (`-1`). Retain
  manual scaling and automatic-scale controls. No separate bevel change was
  requested; check the resulting borders at 1x. Verify that fresh startup does
  not override this with an automatically chosen larger scale.
  Entry points: `src/table/settings/misc_settings.ini`, `src/settings_gui.cpp`,
  and interface-scale initialization.
  Suggested commit: `Change: Default interface scaling to 100 percent`
  Completion: release build passed; isolated startup visually checked at 1x
  with sprite font. Saved fresh defaults confirm gui_scale=100 and
  prefer_sprite_font=true.

- [x] **04 — Remove the tram toolbar button entirely (review 2).**
  Remove the separate tram button from normal and scenario-editor toolbars,
  including compact/wrapped arrangements. Close up its space. Do not add a
  replacement entry to the road menu. Retain underlying tram support and
  unrelated shortcuts; this task removes the unwanted button, not the feature.
  Check widget lookups and enable/disable logic for references to removed
  widgets, and ensure adjacent buttons still perform the correct actions.
  Entry points: `src/toolbar_gui.cpp`, `src/widgets/toolbar_widget.h`.
  Historical reference: toolbar portions of `c02ef3e456` only.
  Suggested commit: `Change: Remove the tram construction toolbar button`
  Completion: release build passed; normal and editor toolbars checked at
  1024x768 and 640x480 without a tram button or gap. Compact arrangements
  balanced in source; duplicate map shortcut removed from the lower editor
  row. Underlying tram shortcuts retain their direct construction handlers.

- [x] **05 — Restore large Location buttons (review 3).**
  Restore bottom-row text buttons in town, industry, station, and waypoint/buoy
  windows instead of their title-bar location pins. Include town windows in the
  scenario editor. Preserve current location actions and modifier behaviour.
  Check centering, resizing, and bottom-row layout without empty gaps.
  Entry points: `src/town_gui.cpp`, `src/industry_gui.cpp`,
  `src/station_gui.cpp`, `src/waypoint_gui.cpp`, `src/lang/english.txt`.
  Historical reference: `5bfcd742b8`.
  Suggested commit: `Change: Restore text Location buttons in object windows`
  Completion: release build passed. Town and industry windows visually checked
  in an isolated game; both bottom Location buttons centre the map. Station
  and scenario-editor town Location buttons also checked in-game. Waypoint
  and buoy declarations retain existing handlers but were not exercised.

- [x] **06 — Restore the vehicle eye button (review 4; preserve 6 and 7).**
  Replace the vehicle title-bar location pin with the larger eye button in the
  side column. Preserve the current click/double-click/Ctrl-click actions and
  follow-state indication. Keep the separate destination pin and current
  start/stop status-bar behaviour. The owner wants the ordinary eye-button
  centering action back, not the old Ctrl-click shortcuts.
  Check all four vehicle types, including vehicles stopped in depots.
  Entry points: `src/vehicle_gui.cpp`, `src/widgets/vehicle_widget.h`, sprites.
  Historical reference: layout portions of `589feba0eb` only.
  Suggested commit: `Change: Restore the vehicle location eye button`
  Completion: release build passed; train side-column eye visually checked and
  clicked in-game. Shared vehicle layout and unchanged click/follow handlers
  retained for all types; other vehicle types were not individually exercised.

- [x] **07 — Restore text Rename buttons (review 5).**
  Replace the small rename icons with the previous text controls and placement
  in town, station, waypoint/buoy, and depot windows. Restore vehicle renaming
  to the vehicle details window. Follow the old layouts where other retained
  features permit; do not disturb the restored Location/eye controls.
  Check rename dialogs, ownership restrictions, and vehicle types separately.
  Entry points: the relevant `src/*_gui.cpp` and `src/widgets/*_widget.h` files,
  plus `src/lang/english.txt`.
  Historical references: `5bfcd742b8`, `589feba0eb`.
  Suggested commit: `Change: Restore text Rename controls and their placement`
  Completion: release build passed. Town buttons/dialog and train details
  Rename checked in-game, including submitting a new vehicle name. Restored
  depot Location alongside Rename to keep its original bottom row. Ownership
  checks retained. Station and editor-town rename dialogs also checked;
  remaining vehicle/depot variants were not individually exercised.

- [x] **08 — Always show all signal types; add a hide-semaphores setting (review 10–11).**
  Always expose block, entry, exit, combo, path, and one-way path signal types.
  Remove the path-only picker mode as a user-facing choice; an old saved
  path-only preference must not hide block/pre-signals. Do not restore the
  basic/advanced toggle. Keep signal-type cycling behaviour separate.
  Add a client UI setting to hide semaphore variants, defaulting to hidden for
  this fork. When hidden, collapse their space and select electric/light signals
  for new placement, including early game years and placement without an open
  picker. Avoid leaving a hidden semaphore variant selected after toggling.
  When enabled again, semaphore variants should remain usable. Preserve already
  built semaphores and signal simulation; this is a picker/placement preference.
  Check toggling live, reopening the picker, early/late years, all six signal
  types, and existing semaphore signals. Account for configuration compatibility.
  Entry points: `src/rail_gui.cpp`, `src/settings_type.h`,
  `src/table/settings/gui_settings.ini`, `src/widgets/rail_widget.h`,
  `src/lang/english.txt`. References: `7469f00c5b`, `8f01ae675e`.
  Suggested commit: `Change: Show all signal types with optional semaphore hiding`
  Completion: Release build passed. All six light-signal buttons and live
  semaphore toggling/reopening verified in a 1939 game at 1x. Hiding removes
  the row and resets selection to electric; old path-only config is ignored.
  Both picker-free placement paths share the new default helper. Signal
  simulation is unchanged; a full placement matrix was not exercised.

- [x] **09 — Restore the old main menu with climate pictures (review 14).**
  Restore the familiar text-button grid and four clickable climate pictures.
  Use the pre-redesign menu as the layout reference, while keeping current
  Options/Settings integration and other explicitly retained functionality.
  Restore appropriate direct menu entries using current handlers. Verify all
  four climate choices carry through to new-game generation and that loading,
  scenarios, multiplayer, content, settings, and editor actions still work.
  Entry points: `src/intro_gui.cpp`, `src/widgets/intro_widget.h`,
  `src/lang/english.txt`. Historical reference: `d00196cd65` and its parent.
  Suggested commit: `Change: Restore the classic main menu and climate pictures`
  Completion: Release build passed. Classic grid checked at 1024x768 and
  640x480; all four climate pictures carry through to world generation.
  Options and Load Game opened successfully. Other entries retain current
  handlers, with direct AI, Game Script and NewGRF entries restored.

- [x] **10 — Restore station-list cargo filter buttons (review 18).**
  Replace the multi-select cargo dropdown in the company station list with
  directly clickable cargo buttons. Preserve filtering semantics, including
  Ctrl-click multi-selection and the relevant all/none controls.
  Check filtering and compact layout with the default cargo set; avoid breaking
  larger cargo sets even though NewGRFs are not a priority for this fork.
  Entry points: `src/station_gui.cpp`, `src/widgets/station_widget.h`,
  `src/lang/english.txt`. Historical reference: `46da56b191`.
  Suggested commit: `Change: Restore station list cargo filter buttons`
  Completion: Release build passed. In-game filters changed the list from
  286 stations to 14 for coal, 147 for coal plus passengers using Ctrl-click,
  53 with no cargo rating, and back to 286 with ALL. Resizing and name search
  retained. Larger cargo sets wrap in rows of 16; no large NewGRF set was tested.

- [x] **11 — Move small-map controls back to the bottom (review 26).**
  Restore the bottom control layout without leaving a vacant side strip.
  Preserve current map modes, legends, filters, and interactions. Check minimum
  size, resizing, and expanded legends.
  Entry points: `src/smallmap_gui.cpp`, `src/widgets/smallmap_widget.h`.
  Historical reference: `368ade294a`.
  Suggested commit: `Change: Restore small map controls below the map`
  Completion: Release build passed. Bottom controls, industry and cargo-flow
  legends checked in-game, including shrinking to minimum size and expanding
  again. The map fills the full width with no vacant side strip.

- [x] **12 — Remove search from dropdown menus (review 28).**
  Remove the dropdown text-filter UI and associated filtering/focus behaviour
  across affected menus. Keep ordinary selection, scrolling, keyboard handling,
  and multi-select behaviour. Ensure stale persistent query strings cannot
  invisibly filter entries. Retain dedicated search fields in directories,
  build windows, and other non-dropdown interfaces.
  Check rail/road menus and representative searchable dropdowns outside the
  toolbar; traditional toolbar mode alone does not complete this task.
  Entry points: `src/dropdown.cpp`, `src/dropdown_type.h`, and callers using
  `DropDownOption::Filterable` or persistent dropdown filter text.
  Historical references: `38b74639be`, `d685fc97f1`, `d02d57b348`.
  Suggested commit: `Change: Remove dropdown menu text search`
  Completion: Release build passed. Language and currency dropdowns checked
  in-game without search fields; wheel scrolling and selection work. Removed
  filter widgets, text matching, persistent queries and caller flags. Dedicated
  station/build-window searches remain; badge multi-select retains Persist.

- [x] **13 — Default to immediate viewport movement (follow-up).**
  Default `gui.smooth_scroll` to `false`, retaining the option. Location/eye
  actions and small-map clicks should jump directly to their destination.
  Arrow-key panning should apply each movement directly without easing or
  trailing motion after release; keep its existing speed and Shift modifier.
  Both paths use the same viewport destination/update mechanism, so a second
  keyboard-specific setting should not be necessary.
  Check short and long location jumps, arrow-key press/hold/release, diagonal
  movement, and Shift+arrows with a fresh config and with the option toggled.
  Entry points: `src/table/settings/gui_settings.ini`,
  `src/viewport.cpp` (`UpdateViewportPosition`),
  `src/window.cpp` (`HandleKeyScrolling`, `ScrollMainViewport`).
  Historical reference: `36579dd18b` (2024-01-25, enabled smooth scrolling by
  default); `ea8c1d8597` and `05b253b33b` changed animation timing that month.
  Suggested commit: `Change: Default to immediate viewport scrolling`
  Completion: Release build passed; fresh config saves smooth_scroll=false.
  Station and editor-town Location buttons jump directly to their targets.
  Ordinary, diagonal and Shift-arrow panning checked; viewport image regions
  immediately after release and 0.2 seconds later are identical for ordinary
  and Shift-arrow movement. The existing smooth-scroll option is retained.

- [ ] **14 — Optional/deferred: taller status-message popups (screenshot follow-up).**
  The owner wants the blue "Message" news popup to have the taller appearance
  they remember. Reference example: "Plondingbury East now accepts Mail" above
  the bottom status bar. This targets the popup, not the status-bar ticker.
  Resolve the historical/reference height before implementing: the current
  small-news viewport is 274 x 47, and 0.5.3 already used that viewport size
  in a 280 x 87 window. No recent viewport-height reduction has been confirmed.
  `568bb1f5da` (2024-11-30, #13134) changed small-news padding and text sizing,
  but retained the 47-pixel viewport. `910dd65710` rearranged other news styles
  that day. Compare the remembered appearance with small, thin, and normal
  news styles at 1x/sprite font; obtain a reference or agree on a taller target
  if no matching historical reduction exists. Do not claim an arbitrary new
  height is the old height. NewsDisplay Summary is the ticker, whereas these
  popup layouts are selected by NewsStyle; do not conflate the two settings.
  Give the map preview the intended vertical space, retaining readable message
  text, width, click-to-location, close controls, slide-in/out behaviour, and
  positioning above the status bar/chat. Check short and wrapped messages,
  station acceptance news, and vehicle advice sharing the small-news layout.
  Entry points: `src/news_gui.cpp` (`_nested_small_news_widgets`,
  `NewsWindow::UpdateWidgetSize`), `src/station_cmd.cpp` (acceptance messages).
  Suggested commit: `Change: Restore taller status message popups`
  Completion: pending; exact historical/desired height remains to be resolved.

- [x] **15 — Remove Coverage and Cargo Graph from town windows (follow-up).**
  Remove both buttons from the individual town view, including Coverage in the
  scenario editor. Close up the bottom row so Location, Local Authority and
  Rename can use the compact space. Preserve editor-specific town controls.
  Remove associated widget lookups and view-specific handlers where obsolete;
  retain the underlying station coverage and graph features elsewhere.
  Check normal/editor layouts at 1x, resizing, and the remaining buttons.
  Entry points: `src/town_gui.cpp`, `src/widgets/town_widget.h`.
  References: coverage `81d335b081`, `b454ca7110`; graph `bd57aee3a9`.
  Suggested commit: `Change: Remove coverage and graph buttons from town views`
  Completion: Release build passed. Normal and editor town windows checked
  at 1x and 640 × 480; remaining rows fill their width without gaps. Location,
  Local Authority and editor resizing exercised in an isolated test session.

- [x] **16 — Remove Industry Chain and Cargo Graph from industry windows (follow-up).**
  Remove both buttons from the individual industry view, leaving Location and
  the resize control without unused gaps. Retain the chain/graph windows and
  their other entry points; this supersedes the earlier decision only about
  keeping the chain button in this particular window.
  Check narrow/default widths, resizing, and Location in game and editor.
  Entry points: `src/industry_gui.cpp`, `src/widgets/industry_widget.h`.
  References: graph `3fca0cf3ee`, later renamed by `56942a15c7`.
  Suggested commit: `Change: Remove chain and graph buttons from industry views`
  Completion: Release build passed. Industry window, Location action and
  resizing checked in a normal game at 1x and 640 × 480; the button spans
  the bottom row without gaps. Editor layout also covered by task 17 checks.

- [ ] **17 — Compact industry accepts/produces into single lines (follow-up).**
  Put accepted cargoes together on one line and produced cargoes together on
  one line, keeping the cargo icons. Aim for the same default viewport and
  whole-window dimensions across ordinary base-game industries, including
  industries with no inputs, multiple inputs, or multiple outputs; reserve a
  consistent information-panel height rather than growing it per cargo.
  Preserve access to production amounts and transported percentages. Check
  whether these fit in the compact row before settling the presentation; do
  not silently discard them to achieve a fixed size. Keep production editing
  usable in the editor. Long/custom cargo descriptions, stockpiles, closure
  notices and script text may need an exceptional detailed layout.
  Historical distinction: 1.6.1 used one inline Requires list for ordinary
  industries, but already had a Production last month heading and a separate
  row per output. One-line production and uniform overall sizes are the owner's
  requested design, not a verified exact restoration of the 2016 layout.
  Earlier evidence does support uniform sizing: 0.3.4 (2004) had fixed
  260 × 160 industry windows, reserving space for two separate production rows.
  Thus uniform size can be restored without combining all production into one
  row; keep this distinction available when implementing the requested layout.
  Check a coal mine, factory, farm (two outputs), and an accepting-only industry
  at 1x, plus resizing and production changes. Coordinate with task 16.
  Entry points: `src/industry_gui.cpp` (`DrawInfo`, sizing and production-edit
  hit testing), `src/lang/english.txt`.
  References: `1.6.1`, `dcc240f826`, `a7adadcf9b`, `481fa7dbb5`.
  Suggested commit: `Change: Compact industry cargo information`
  Completion: pending; added to the backlog, not implemented.

## Window size and shape investigation

The owner requested investigation of industry, town, and vehicle window shape.
Tasks 15–17 record the latest choices arising from this investigation; other
findings are not additional implementation tasks. Existing tasks 02–03 and
05–07 already cover font/scale and Location/Rename/eye controls.
"Industry menu" could mean the individual industry view, Industry Directory,
or Fund New Industry, so this investigation covers all three.

Comparison: local `1.10.3`, `1.11.0`, `13.0`, `14.0`, and current source at
`97862f4a4b`. This was a source/history comparison, not a rendered pixel-size
comparison. Font metrics, interface scale, content, minimum button widths,
and saved window sizes can change actual window dimensions.

### Earlier baseline: TTD manual and OpenTTD 0.3.4 (2004)

The original TTD manual's screenshots on printed pages 35 and 42 show the
shorter road-vehicle view and taller train view with the additional side button.
Source: [MicroProse TTD manual](https://drwjf.github.io/ctm/references/Transport_Tycoon_Deluxe_Manual_EN.pdf).
The scan supports the proportions; exact pixel values below were verified in
OpenTTD `0.3.4` (`e8c8a3f397`), not measured from the original TTD executable.

| Window | Fixed whole-window size | Actual map viewport |
| --- | --- | --- |
| Train | 250 × 134 | 226 × 102 |
| Road vehicle | 250 × 116 | 226 × 84 |
| Town | 260 × 150 | 254 × 86 |
| Industry | 260 × 160 | 254 × 86 |

Verified via `_train_view_desc`, `_roadveh_view_desc`, `_town_view_desc`,
`_industry_view_desc` and their `AssignWindowViewport` calls in the tag's
`train_gui.c`, `roadveh_gui.c`, `town_gui.c`, and `industry_gui.c`.
Train and road map views differed by exactly one 18-pixel button row; town and
industry map views matched despite different total window heights.

All industries used the same fixed window size in that version. Requires was
drawn at y=107; Production last month at y=117; output rows at y=127 and y=137.
Unused rows remained blank rather than shrinking the window. This supports the
owner's memory of consistent industry sizes much better than the 2016 baseline.
It also corrects the implication that uniform sizes must be a new design: the
new part is combining production rows, not reserving a uniform panel height.
By `f8294b1386` (2008-03-28), industry sizing was already content-dependent;
that commit revised an existing cargo-line-count resizing mechanism.

### Ten-year comparison: 1.6.1 (2016-06-30) versus this fork

Compared the actual widget declarations, window descriptors, drawing code and
relevant history against `1.6.1` (`4627d5c28f`) and this fork at `d43f9a98d3`.
These are source measurements, not measurements of side-by-side screenshots.

| Window | 2016 sizing declarations | Current fork | 2016 arrangement |
| --- | --- | --- | --- |
| Town | Viewport minimum 254 × 86; preferred window 260 × 150 | Same viewport minimum and preferred window size | Location, Local Authority, Rename across the bottom; no Coverage or Cargo Graph. Editor had Rename in the title bar and Location/Expand/Delete below. |
| Industry | Viewport minimum 254 × 86; preferred window 260 × 120, expanded to fit information | Same viewport minimum and preferred window size | Location and Display chain below; inline Requires list for ordinary industries; production heading plus one row per output. |
| Road vehicle (also ship/aircraft) | Shared viewport minimum 226 × 84; preferred window 250 × 116 | Same declarations | Five 18-pixel side buttons including the eye; status bar below. |
| Train | Same shared viewport minimum; separate preferred window 250 × 134 | Same declarations; still a separate train descriptor | Six 18-pixel side buttons, including the additional ignore-signal button. |

**The train/road height difference is real.** The extra train button needs an
additional 18 pixels of side-column height at 1x, and the resizable viewport
beside it grows to match. Sharing the 226 × 84 minimum never meant equal final
viewport heights. The current fork again has the eye in that column, and still
selects `_train_view_desc` for trains and `_vehicle_view_desc` otherwise. The
previous summary of the shared viewport minimum alone did not capture this.

Subtle changes found since that baseline:

- **2016-04-17, `b8189977b4`:** long vehicle captions now widen the viewport
  instead of stretching the side buttons. Already present in 1.6.1, but relevant
  to memories of earlier versions.
- **2017-02-26, `dcc240f826`:** accepted industry cargo became a heading plus
  individual rows. This really changed information-panel height by cargo count.
  Production was already multiline before this change; old windows were not
  all identical in total height either.
- **2019, `81d335b081` / `b454ca7110`:** Coverage added to normal/editor town
  windows, introducing another bottom button that can affect minimum width.
- **2020-12-27, `589feba0eb` (released in 2021):** eye moved out of the vehicle
  side column, Rename moved into its title bar, destination pin added below.
  Removing one side button changed the height constraint without changing the
  shared viewport minimum. Tasks 06–07 restore eye/Rename; destination pin stays.
- **2021-05-08, `330a305c99`:** inset viewport borders switched from explicit
  scaled padding to automatic bevel padding. At 1x the intended one-pixel inset
  is retained; larger scales are where the old excess border space differed.
- **2022-09-04, `93d2d4590f`:** embedded viewport zoom made aware of GUI scaling.
  At 1x, town/vehicle views still use the traditional normal map zoom, while
  industry uses one step farther out. Beware enum names: 2016 `OUT_4X` corresponds
  to today's `Normal`. Town actually used `ZOOM_LVL_NEWS` in 1.6.1, so comparing
  the unused old `ZOOM_LVL_TOWN` constant gives the wrong answer.
- **2022-09-23, `890b2666d3`:** vehicle status-bar height switched from an
  explicit text-line minimum to font/sprite dimensions plus widget padding.
  `60404c3aea` (2022-11-14) also adjusted status-icon sizing/centering. These
  can affect the surrounding chrome; no exact rendered pixel delta established.
- **2022–2025:** production spacing/editing fixes, cargo icons and added graph
  buttons affect information-panel or bottom-row proportions. See details below.

Conclusion from source: no blanket change to the declared map-preview minimums
since 2016. Actual proportions can still change through the adjacent controls,
information rows, text width, font/scale, and saved window dimensions. Exact
pixel comparison remains unmeasured; it would require matching 1x sprite-font
screenshots with default window sizes in both versions, separating the map image,
border, information panel and button rows. No additional viewport-size change
is selected on the strength of this source comparison alone.

### Individual windows

- **Screenshot follow-up: confirmed 2017 industry layout change.** The owner
  identified the individual factory window, showing Requires on its own line
  followed by separate Livestock, Grain, and Steel rows. `dcc240f826`
  (2017-02-26, SVN r27751, included in OpenTTD 1.7.0) changed ordinary,
  non-stockpiling industries from one inline comma-separated Requires line
  to a heading plus one indented line per accepted cargo. The purpose was
  better NewGRF callback-37 cargo-suffix support, but the layout also changed
  for default industries. A three-input factory therefore gained three text
  rows (roughly 30 pixels at the traditional 1x normal-font line height).
  This is a stronger explanation for the taller information panel in the
  screenshot than the post-2021 changes alone. Verified against 1.6.1 source
  and strings; 0.5.3 also draws accepted cargoes on one line and declares a
  260 x 160 industry window. Later automatic sizing means that old fixed
  total height should not simply be imposed on every current industry.
  Now queued as task 17, including the owner's additional compact-production
  and consistent-size request. Keep cargo icons (2024) and accommodate exceptional
  custom information. Graph buttons are covered by task 16. The original 1.10.3
  comparison was too recent to catch this change.
- **No blanket enlargement of the three viewport definitions found.**
  Industry and town viewports still declare a minimum of 254 x 86; the vehicle
  viewport still declares 226 x 84, as in 1.10.3. Industry's preferred window
  size remains 260 x 120 and vehicle's 250 x 116. These are layout inputs,
  not guarantees of final on-screen dimensions. Font/scale restoration should
  be assessed before deciding to hard-code smaller window sizes.
- **2021: controls moved out of the main button rows.** `5bfcd742b8` moved town
  Location/Rename into the title bar and enlarged the Catchment button's
  declared minimum width from 14 to 40, with equal-width bottom controls.
  It moved industry Location into the title bar, leaving Display chain below.
  `589feba0eb` removed the vehicle side-column eye and added title-bar controls
  plus a destination pin below. These explain changes in proportions and
  available space even where viewport minimum sizes stayed constant.
  Already substantially addressed by tasks 05–07; include town Catchment
  proportions when comparing the restored bottom row.
- **2021: viewport border padding changed.** `330a305c99` fixed excessive
  padding at larger UI scales by moving inset padding into the widget system.
  This affects all three windows but is not an intentional larger-window
  redesign. Do not undo the padding fix just to copy old widget declarations.
- **2022–2024: industry information text changed.** `a7adadcf9b` indented
  production text and fixed production-edit controls. `481fa7dbb5` added icons
  alongside accepted/produced cargoes and reserved horizontal space for them.
  Owner decision: keep the cargo icons when compacting the cargo lists in
  task 17; do not implement a text-only panel.
- **2024–2025: graph buttons added to bottom rows.** `3fca0cf3ee` added an
  industry Production Graph button (later Cargo Graph, `56942a15c7`).
  `bd57aee3a9` added a town Cargo Graph button. These compete for bottom-row
  width and can force wider windows depending on text metrics. Removal from
  these individual windows is now queued in tasks 15–16.
- **2024: title-bar icon proportions changed.** `16eb17418b` replaced fixed
  12 x 14 minimum dimensions for Location/Rename icons with aspect-ratio sizing.
  This affects industry, town, and vehicle windows. Tasks 05–07 remove most of
  those icons, but the vehicle destination pin is intentionally retained.
- **2026: train details tabs rearranged.** `5b513abef0` removed the separate
  Capacities tab, combined capacity with cargo information, renamed Total cargo
  to Totals, and added train length there. Four tabs became three. This affects
  the train details window, not the small vehicle viewport window.

### Directory, funding, and vehicle-list windows

| Window | Confirmed change | Reference |
| --- | --- | --- |
| Industry Directory | Added a name-search row in 2023. | `6e52ceab96` |
| Industry Directory | Added horizontal scrolling in 2023 and stopped using every full row's text width as the list's minimum width; changed horizontal extent calculation to visible rows in 2024. This can make the window narrower with clipped/scrollable content. | `badce415ea`, `1f18894408` |
| Fund New Industry | Added counts of already-built industries in 2023; subsequent width fix accounts for those counts. | `3a03a12a9c`, `e83e2df023` |
| Town Directory | Added name filtering in 2019 and horizontal resizing in 2021. | `196d586849`, `0345f99180` |
| Vehicle lists | Added cargo filtering in 2022 and made dropdown widths fit their labels, changing control layout/width. | `0d303d6c3f`, `f7e2b6ef12` |

Dedicated directory search fields are separate from dropdown search. Task 12
does not remove them. The industry/cargo-chain diagram itself remains out of
scope; task 16 removes only its button from the individual industry view.

## Leave as-is

These are deliberate scope decisions, not unfinished implementation tasks.

| Review item | Decision |
| --- | --- |
| 6 | Keep current follow-vehicle and extra-viewport modifier behaviour. |
| 7 | Keep current destination pin and status-bar behaviour. |
| 12 | Keep current pressed-button rendering / TTD behaviour. |
| 13 | Leave shared construction pickers alone; owner may reconsider later. |
| 15 | Keep current combined/tabbed Options and Settings. |
| 16 | Keep current finances layout. |
| 17 | Keep current Local Authority action list. |
| 19–20 | Keep NewGRF vehicle variants, badges, and their controls. |
| 21 | Keep city icons; owner considers them an improvement. |
| 22 | Keep current graph colours. |
| 23–24 | Keep current multiplayer joining, Online Players, and Give Money UI. |
| 25 | Keep current signal dragging behaviour. |
| 27 | Keep current industry/cargo-chain window itself; task 16 removes its button from the individual industry view. |
| Factory screenshot follow-up | Keep accepted/produced cargo icons in the industry information panel. |

## Verification and existing configurations

Tasks 01–13 are complete. The Release build succeeds and all 99 CTest tests
pass. UI checks used an isolated display and temporary configurations; the
owner's normal configuration and original save files were not overwritten.
Individual completion notes distinguish runtime checks from source inspection
and record coverage limitations.

Existing saved preferences take precedence over defaults. To apply all five
new defaults to an existing configuration, merge these values into its existing
sections while the game is closed; do not replace the whole configuration:

```ini
[misc]
prefer_sprite_font = true
gui_scale = 100

[gui]
toolbar_dropdown_autoselect = true
hide_semaphores = true
smooth_scroll = false
```

## Resume prompt

> Read UI_RESTORATION.md and implement the next explicitly authorized unchecked
> task. Make forward edits, verify the change, update its checkbox and completion
> note, and create one commit for that working change. Preserve the other recorded
> choices. Tasks 15–17 are queued follow-ups. Task 14 remains deferred; the other
> historical observations do not by themselves authorize further UI changes.
