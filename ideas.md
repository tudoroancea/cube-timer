## Important (next to work on):
[x] Settings panel. Categories:
  - Timer : Launching interval (uint)
  - Saved times
  Label on the left

[x] Add new metadata (time stamp, scramble)
[ ]=> add preferences for which metadata to display

[x] Delete times by selecting a line or more in the TimesList and right-clicking on one of them.There will be a
warning dialog to confirm you want to proceed.

[x] Add commentary metadata. You can modify them by right clicking on one.

[x] change colors on the lines of the times with missing metadata (no scramble or no timestamp).

[ ] Add PB stats (best time, mo3, ao5, ao12)

[ ] let users see the times included in a certain mo3, ao5 or ao12.

[ ] Use qrc instead of the macOS bundle Resources

[ ] Several autoSave options : on app closing, or even at each new time/action.

[ ] Starting/Stoping timer with click

[ ] replace Duration by QTime that already has the necessary methods.\
[ ] replace fs::path by QDir/QFile which already have the necessary methods.


## Miscalleneous ideas:
We can insert new columns in the CSV files each time we pass a new bound (5, 12, 25, 50, 100, 200, 500, 1000…).\
À la fermeture de l'application, on enregistrera les données dans le current CSV, pas forcément dans le default. On pourra ouvrir une fenêtre de dialogue pour proposer d'aussi écraser/append dans le default CSV.

Always save mo3, ao5, ao12, scramble, timestamp, commentary, but ony display what the user wants.

For the comment we can add buttons right under the timer to quickly insert things like "OLL skip", "PLL skip", "DNF", "wrong PLL", wrong OLL"

include graphics (like documentation/cheat sheets) with moves, OLL and PLL algorithms

Timer locked and only scrambles

Redo a scramble (select a previous time and load the scramble)

add button for next scramble and previous scramble (keep the last 10 generated scrambles)

## Long shots:
- Add scramble image
## Very long shots:
- iCloud synchronization
- Moves detection with computer vision/AI