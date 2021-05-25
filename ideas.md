# ⚙️👨‍💻 Features
(This list may be uncomplete. Up to date list at : https://github.com/tudoroancea/cube-timer/projects/1)
- [x] Settings panel. Categories:
  - Timer : Launching interval (uint)
  - Auto save
- [x] Add new metadata (time stamp, scramble)
[ ]=> add preferences for which metadata to display
- [x] Delete times by selecting a line or more in the TimesList and right-clicking on one of them.There will be a
warning dialog to confirm you want to proceed.
- [x] Add commentary metadata. You can modify them by right clicking on one.
- [x] change colors on the lines of the times with missing metadata (no scramble or no timestamp).
- [x] Add PB stats (best time, mo3, ao5, ao12)\
- [x] let users see the times included in a certain mo3, ao5 or ao12 => create a class Time that is a subclass of
QtableWidgetItem and also contains information about its row in the TimesList table and in the csv::Document.
- [ ] Several autoSave options : only on app closing, at each new time/action, never.
- [ ] Multiple sessions
- [ ] Use GitHub gists for synchronization
- [ ] Starting/Stoping timer with click

# 🛫 Better use of Qt classes and portability
- [ ] replace Duration by QTime that already has the necessary methods.
- [ ] replace fs::path by QDir/QFile which already have the necessary methods. This would help to port the app to 
windows as it won't use filesystem anymore (which currently is not fully supported by mingw).
- [ ] Use qrc instead of the macOS bundle Resources. UPDATE : to do so, we would have to modify rapidcsv.h to use
QFile, QTextStream and QStrings. Although it would be a great exercise to understand how to really handle text files,
it would be very time consuming and not necessarily worth the effort (for the moment).

# Miscalleneous ideas:
- We can insert new columns in the CSV files each time we pass a new bound (5, 12, 25, 50, 100, 200, 500, 1000…).\
- For the comment we can add buttons right under the timer to quickly insert things like "OLL skip", "PLL skip", "DNF", "wrong PLL", wrong OLL"
- Timer locked and only scrambles
- Use GitHub gists to sync the default-historic.csv file
- historic.csv contains Several columns (one for each session)

# Long shots
- [ ] Add scramble image
## Very long shots:
- [ ] iCloud synchronization
- [ ] Moves detection with computer vision/AI