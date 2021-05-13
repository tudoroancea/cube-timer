# Cube timer
This is a simple GUI project using Qt to create a cube timer suited to my needs.
This implies :
- It is purely intended for macOS users (on other OS's the CMakeLists won't even create the main target). It uses in particular the ```.app``` bundle particularities such as the ```Resource``` directory which can contain the CSV file used to saveToCurrentCSV the times.
- The available features won't ever be very ground-breaking, and I won't develop the ones I don't find useful (even though they may be very popular in the community). *Eg* : I won't implement inspection times or scrambles for cubes larger that 3x3. I won't develop neither stackmat or other physical timers integration, nor smart cubes integration.

## Current features :
- Timing at milliseconds precision. One must keep the space bar pressed for 0.3s before launching the timer (customizable).
- 20 moves scramble generation (non customizable) after each attempt.
- Generating new scrambles before launching the timer (if you don't like the current one)
- ao5 and ao12 computed live and saved along with the actual times once the app is closed.
- Settings Panel.

## Copyright
This project is licenced under the MIT licence (see ```LICENCE``` file).