This is a room based map/game engine implementation using ncurses for visualization.
The classes of the engine (namespace Room_System) don't depend on ncurses library.

Qt-creator can't open the terminal for ncurses, so I have been using command "xterm -e ./Rooms" on the build folder. To debug with Qt comment the ncurses initialization lines from the beginning of main()

