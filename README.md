# ChessQD

## Introduction

ChessQD, developed by Frank Hans and Wertzuz, is an implementation of the ancient strategic board game Chess. The game is played on an 8x8 chessboard, where each player commands an army of 16 pieces, including a king, queen, rooks, knights, bishops, and pawns. The objective is to checkmate the opponent's king, trapping it in a position from which it cannot escape. This was done as a school project.

## Features

-     Graphical User Interface
-     In-game accessible buttons:
  -     Rotate Board
  -     Play Online
    -     Play against another person on the local network
  -     Resign
    End the current game
-     Display of the game result
-     Timer display
-     Variable resolution and window size
-     Implementation of the Chess board game following official rules


## Prerequisites Running

Before running or building ChessQD, make sure you have the following libraries installed on your system:

### For Running ChessQD:

- libsdl2-2.0-0
- libsdl2-image-2.0-0
- libsdl2-ttf-2.0-0

### Installing Required Libraries

To install the libraries(on Ubuntu) needed for running ChessQD, open a terminal and run the following command:

```bash
sudo apt-get install libsdl2-2.0-0 libsdl2-image-2.0-0 libsdl2-ttf-2.0-0
```

## Installation (Release Version)

To install the release version of ChessQD, follow these steps:

1.      Visit the [Releases](https://github.com/Hckerx/ChessQD/releases) section of our GitHub repository.

2.      Download the latest release exe file or ELF file from the available assets.

3.      Run the file from the bin directory or make sure that it had access to the subfolders font/ and gfx/


## Prerequisites Building

### For Building ChessQD:

- libboost-all-dev
- libsdl2-image-dev
- libsdl2-ttf-dev
- libsdl2-dev
- libasio-dev


For building ChessQD, use the following command to install the necessary development libraries:

```bash
sudo apt-get install libboost-all-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-dev libasio-dev
```

## Building ChessQD

After installing the required libraries, you can build ChessQD by executing the following commands in the terminal:

```bash
# Navigate to the directory containing ChessQD
cd /path/to/chessqd

# build
make build
```


## Future Enhancements

In future versions, we plan to:

-     Enhance the online mode
-     Add PGN-Notation
-     Add a time limit feature
-     Implement new features based on user feedback.

## Contributing

Any type of contribution via GitHub Pull Requests is welcome: new features, bug fixes, etc.

## License

ChessQD is licensed under the GNU General Public License, version 2.0 (GPL-2.0).

## Support

If you encounter any issues or have questions, please feel free to open an issue on our GitHub repository.

## Acknowledgments

We would like to express our gratitude to our teacher who has made this possible.
