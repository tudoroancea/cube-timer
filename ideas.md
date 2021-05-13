Central widget : timer we can start/stop with space button

- Time the resolutions
- Save the times :\
    [x] saveToCurrentCSV times and time stamps (date and hour )
    [ ] mark them dnf/+2
    - add commentary
    => CSV file with numeric/string attributes
    - delete/modify saved times
    - several profiles
- statistics :
    - nombre de résolutions
    - calculate ao5, aO12,…
    - graphs : numpy for C++

- Scrambles :
  - generate random scrambles (3x3 at least)
  - scramble image
## Important:
- Settings panel. Categories:
  - Timer : Launching interval (uint)
  - Saved times
  Label on the left, 
- Delete/Modify times by selecting a line or more in the TimesList. There will be a warning dialog to confirm you want to proceed.
- Starting/Stoping timer with click

## Miscalleneous ideas:
We can insert new columns in the CSV files each time we pass a new bound (5, 12, 25, 50, 100, 200, 500, 1000…).\
À la fermeture de l'application, on enregistrera les données dans le current CSV, pas forcément dans le default. On pourra ouvrir une fenêtre de dialogue pour proposer d'aussi écraser/append dans le default CSV.

Always save mo3, ao5, ao12, scramble, timestamp, commentary, but ony display what the user wants.

For the comment we can add buttons right under the timer to quickly insert things like "OLL skip", "PLL skip", "DNF", "wrong PLL", wrong OLL"

include graphics (like documentation/cheat sheets) with moves, OLL and PLL algorithms

Timer locked and only scrambles

Redo a scramble (select a previous time and load the scramble)

add button for next scramble and previous scramble (keep the last 10 generated scrambles)

## Very long shots:
- iCloud synchronization
- Moves detection with computer vision/AI 