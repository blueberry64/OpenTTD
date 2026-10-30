# Window layout audit against September 2004

Compared the owner's `historical/0.3.4` worktree (`e8c8a3f397`) with the personal
fork at `46357d7d14`. This is a source-coordinate audit, not a recommendation to
replace all current layouts with 2004 layouts. No game code or historical-worktree
files were changed for this review.

Historical widget coordinates are inclusive: x=0..129 is 130 pixels wide.
Positions below are relative to the window's top-left corner at the original
1x scale. Modern `SetMinimalSize` and `WindowDesc` sizes are layout inputs, not
necessarily the final rendered size: text, sprites, retained controls, settings
and saved dimensions can enlarge them. Do not mistake a 2004 fixed rectangle
for a modern preferred size. Earlier in-game checks are identified separately.

## Accepted follow-ups

The owner subsequently accepted industry half-width Location, normal-town button
order/proportions (keeping information and height), station/airport button order,
and removal of depot Rename. These are tracked as tasks 18–21 in
[the checklist](UI_RESTORATION.md). Depot layouts otherwise and airport pickers
stay as they are. The comparisons below describe the pre-follow-up fork at the
commit named above; they are not a claim that the mismatches still exist.

## Most useful follow-ups

The accepted subset is recorded above; the remaining proposals await choices.

- **Industry Location width:** restore the left-half button and a plain right-half
  panel, keeping the resize grip at the far right. Its 2004 width was **130**, not
  nearly the entire **260**-pixel window. Our task 16 filled the row after removing
  Chain/Graph, so this is a real mismatch in our restoration.
- **Town button order/proportions:** restore **Location | Local Authority | Rename**
  and roughly equal thirds. Our fork currently uses **Location | Rename | Local
  Authority**, with unequal minimum widths. Keep the resize grip and useful town
  information; a 150-pixel total height would require removing information.
- **Station button order:** restore **Location | Ratings/Accepts | Rename** within
  the modern row. Our fork has **Location | Rename | Ratings/Accepts**. Decide
  separately whether newer controls should stay in that row or move elsewhere.
- **Vehicle details label:** optionally change the restored text button from
  **Rename** to the old **Name**. Its location in the details caption row is already
  correct in principle; the old button was at the extreme right because there
  were no shade/default-size/sticky buttons after it.

The first three are layout adjustments that can retain current gameplay and
most conveniences. Exact 2004 station/depot/airport dimensions are a much larger
redesign because of added controls. Leave those broader decisions open.

## Town and industry views

| Element | 0.3.4 exact layout | Current fork / implication |
| --- | --- | --- |
| Industry outer window | 260 × 160 | Four ordinary industry types were previously checked in-game at this same size. Custom text/editor controls can enlarge it. |
| Industry map image | x=3, y=17; 254 × 86 | Same viewport minimum and surrounding 2-pixel outer padding plus inset bevel at 1x. |
| Industry info panel | x=0..259, y=106..147; 260 × 42 | Task 17 reserves acceptance, heading and two output rows. Ordinary 1x height matches. |
| Industry Location | x=0..129, y=148..159; **130 × 12** | Current Location fills all available width before the resize grip; its declared minimum 45 × 12 does **not** describe its final width. |
| Industry right-hand bottom half | x=130..259, y=148..159; **130 × 12 plain panel** | Currently absent. The 2004 source comments identify a removed Destroy Industry button in this slot; the active widget is blank. No need to restore that removed action. |
| Industry acceptance / production text | Requires at (2,107); production heading (2,117); outputs (4,127), (4,137) | Row structure restored. Icons are deliberately retained, so output text starts farther right than the old x=4. |
| Town outer window / map image | 260 × 150; map at (3,17), 254 × 86 | Same declared viewport minimum. Modern town growth status adds a row; growth requirements, noise and script text can add more. |
| Town info panel | x=0..259, y=106..137; **32 high**, three lines | Population/houses, passengers and mail. Modern growth information explains a taller window even after Coverage/Graph removal. |
| Town Location | x=0..85, y=138..149; 86 × 12 | Current first button, minimum 45 × 12 and flexible width. |
| Town Local Authority | x=86..171, y=138..149; 86 × 12 | Current third button, minimum 80 × 12. |
| Town Rename | x=172..259, y=138..149; 88 × 12 | Current second button, minimum 45 × 12. |

Sources: [old industry](historical/0.3.4/industry_gui.c),
[old town](historical/0.3.4/town_gui.c), [current industry](src/industry_gui.cpp),
[current town](src/town_gui.cpp).

The **editor town** was different even in 2004: Rename was a **75 × 14** caption
button at x=185..259; the bottom row was Location (86 wide), Expand (86), Delete
(88). Our editor deliberately retains separate Expand buildings / Expand roads
controls and has Rename below. Copying the old editor row wholesale would remove
those conveniences; this is a choice, not a missing normal-town restoration.

## Vehicle view and details

| View | 0.3.4 outer size | Actual old viewport | Old side column |
| --- | --- | --- | --- |
| Train | 250 × 134 | (3,17), 226 × 102 | x=232..249; six 18 × 18 buttons |
| Road vehicle | 250 × 116 | (3,17), 226 × 84 | x=232..249; five 18 × 18 buttons |
| Ship | 250 × 116 | (3,17), 226 × 84 | x=232..249; five 18 × 18 buttons |
| Aircraft | 250 × 116 | (3,17), 226 × 84 | x=232..249; five 18 × 18 buttons |

Train button top edges: **14, 32, 50, 68, 86, 104**. Order: eye, depot, ignore
signal, reverse/refit, orders, details. Other vehicles: **14, 32, 50, 68, 86**;
order: eye, depot/hangar, reverse (road) or refit (ship/aircraft), orders, details.

The restored shared view uses these same **18 × 18 minimums and button order**.
The train-only signal control still occupies its own row. A clone control can
replace depot in current contextual states; retain that convenience.

In 2004 the status/action strip occupied the **entire 250-pixel width**, was
**12 high**, and began at y=122 for trains or y=104 otherwise. The fork keeps the
newer destination pin and resize grip in that strip, so its clickable status
area is narrower. That is an existing explicit keep decision. Current status
height is calculated from font/sprite sizes and padding; matching the preferred
250 × 116/134 descriptors alone does not prove every final pixel matches.

| Details window | 0.3.4 size | Old Name button rectangle |
| --- | --- | --- |
| Train | 370 × 164 | x=330..369, y=0..13; 40 × 14 |
| Road vehicle | 380 × 101 | x=340..379, y=0..13; 40 × 14 |
| Ship | 405 × 101 | x=365..404, y=0..13; 40 × 14 |
| Aircraft | 390 × 114 | x=350..389, y=0..13; 40 × 14 |

Current details have preferred sizes **405 × 178** (train) and **405 × 113**
(other types), plus content-driven minimums. Rename has a 40-pixel minimum but
uses the longer label and is followed by window controls. Old service arrows
were two **11 × 6** buttons stacked vertically; current service arrows sit
horizontally beside a service-mode dropdown. The train's old four bottom tabs
became three; do not undo that solely to recover its older footprint.

Sources: old [train](historical/0.3.4/train_gui.c),
[road vehicle](historical/0.3.4/roadveh_gui.c),
[ship](historical/0.3.4/ship_gui.c), [aircraft](historical/0.3.4/aircraft_gui.c);
current [vehicle GUI](src/vehicle_gui.cpp).

## Stations, airports as stations, and depots

The old station view was **249 × 110**. Its bottom row at y=98..109 contained
three **83 × 12** buttons: Location (x=0..82), Ratings (83..165), Rename (166..248).
The expanded ratings version was **249 × 210**, with the same bottom buttons at
y=198..209 and Accepts replacing Ratings. Its waiting panel was 52 pixels high;
its normal acceptance panel 32, expanding to a 132-pixel ratings panel.

The current station descriptor prefers 249 × 117 but the actual layout includes
Group and Sort rows, flexible cargo lists, and more bottom controls. The bottom
order is Location, Rename, Ratings/Accepts, conditional Close airport, Coverage,
four scheduled-vehicle buttons, resize. An airport uses this same station view;
there was no separate 2004 airport-information layout with those extra controls.
The close-airport and scheduled-vehicle actions are useful candidates to retain
if the row is reorganized. A literal three-button/249-pixel restoration needs a
plan for them, not just smaller button minimums.

| Depot | 0.3.4 size | Old New vehicles / Location widths | Current preferred size |
| --- | --- | --- | --- |
| Train | 349 × 121 | 174 / 175; y=109..120 | 362 × 123 |
| Road | 315 × 68 | 157 / 158; y=56..67 | 316 × 97 |
| Ship | 305 × 74 | 152 / 153; y=62..73 | 306 × 99 |
| Aircraft hangar | 331 × 74 | 165 / 166; y=62..73 | 332 × 99 |

All those old bottom buttons were **12 pixels high**, split roughly half/half.
There was no depot Rename or Clone button. The fork's shared row also includes
Clone, Rename where applicable, a vehicle-list shortcut, start/stop-all and resize;
the side area has sell-all and autoreplace. Returning Location to half the width
would require relocating these. Keep their functions pending a deliberate layout
choice; our restored depot row follows a later OpenTTD era, not exact 0.3.4.

Sources: [old station](historical/0.3.4/station_gui.c),
[current station](src/station_gui.cpp), [current depot](src/depot_gui.cpp),
and the old per-vehicle files linked above.

## Airport construction and other pickers

Distinguish the **airport construction picker**, **airport station information**
window, and **aircraft hangar**; all three have different layout histories.

The 0.3.4 airport picker was **148 × 131**, already with five choices:

| Choice/control | Old rectangle / size |
| --- | --- |
| Small / City | x=2..73 / 74..145, y=27..38; each 72 × 12 |
| Metropolitan | x=2..145, y=39..50; 144 × 12 |
| International | x=2..145, y=51..62; 144 × 12 |
| Heliport | x=2..145, y=63..74; 144 × 12 |
| Coverage Off / On | x=14..73 / 74..133, y=88..98; each **60 × 11** |

Current airport selection is content-sized: class dropdown, preview, scrollable
five-row type list, orientation controls, extra information, coverage and cargo
acceptance. Coverage buttons retain **60 × 12 minimums**. A compact selection of
built-in airports is a possible future design; do not lose newer airport types
or rotation/custom-airport support just to impose 148 × 131.

The old **airport construction toolbar** itself was **130 × 36**: Build airport
42 × 22, then demolish/lower/raise/buy-land at 22 × 22 each. The current toolbar
contains Build airport and demolish, with landscaping handled separately. This
is different from the additional controls in the picker and station view.

For comparison, old rail-station selection was **148 × 200**, with two **66 × 48**
orientation pictures, **15 × 12** platform-count/length buttons, a Drag & Drop
button, and coverage controls. Old road-stop selection was **140 × 177**, with
four **66 × 50** direction pictures and two **60 × 12** coverage buttons. Newer
pickers support additional types/layouts such as drive-through road stops;
these are not good candidates for blanket fixed-size replacement.

Sources: [old airport](historical/0.3.4/airport_gui.c),
[current airport](src/airport_gui.cpp), [old rail picker](historical/0.3.4/rail_gui.c),
[old road picker](historical/0.3.4/road_gui.c), [current road](src/road_gui.cpp).

## Other useful boundaries

- **Small map:** 0.3.4's small form was **248 × 212**, with seven **22 × 22**
  buttons down the **right side**, plus a legend below. Its other preset sizes
  were 446 × 302 and 607 × 452. Our chosen bottom-controls arrangement is a later
  classic OpenTTD layout, not a 2004 one. Keep the user's bottom-layout decision.
  Sources: [old](historical/0.3.4/smallmap_gui.c), [current](src/smallmap_gui.cpp).
- **Main menu:** 0.3.4 was **336 × 197**, using **158 × 12** paired text buttons
  and **76 × 54** climate buttons. Our classic grid retains newer entries such
  as heightmaps/content/script configuration and combined Options; its dynamic
  overall size is intentional. Source: [old](historical/0.3.4/intro_gui.c),
  [current](src/intro_gui.cpp).
- **Small Message news popup:** still **280 × 87** with a **274 × 47** map image
  at (3,17) in this 2004 checkout. This does not supply evidence for a taller
  historical map preview; task 14 stays deferred. Source:
  [old](historical/0.3.4/news_gui.c), [current](src/news_gui.cpp).
- **Waypoints/buoys:** not a direct match to the modern shared viewport window.
  Rail checkpoints in this checkout opened a rename query; station-tile clicks
  went through the station view. Do not infer an old 260 × 118 waypoint-view
  layout from the present one. Sources: [rail click handler](historical/0.3.4/rail_cmd.c),
  [station click handler](historical/0.3.4/station_cmd.c),
  [current waypoint](src/waypoint_gui.cpp).

No automated tests were run for this documentation-only audit. The existing
build checks for tasks 15–17 still stand; this review adds historical coordinate
checks rather than claiming new runtime measurements of every modern window.
