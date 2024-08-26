# Tamerlane Chess C++

[![GitHub](https://img.shields.io/badge/GitHub-Download-blue?style=flat-square&logo=github)](https://github.com/mirror-shades/tamerlane-cpp/releases/latest/download/Tamerlane.Chess.zip)

I am building a Tamerlane chess engine in C++. This engine was previously built in Pygame but the performance made building a proper AI messy. This engine will let us better explore the strategies of this ancient chess varient.

## Compiling

The game is compiled using minGW 6.1.0 > 6.3.0.

There is a batch file to compile the game in the root directory called `run.bat`. Use the flag `release` to compile the game in release mode.

Alternatively, run in powershell:
`mingw32-make -f Makefile; if ($?) { ./build/main.exe }`  
or run in terminal:
`mingw32-make -f Makefile && build\main.exe`

## todo

[ ] qol and bug fixes  
[ ] proper highlighting for fortress squares  
[ ] previous move highlighting  
[ ] capture piece list
[ ] AI menu, select which side to play as and difficulty

analysis mode:  
[ ] move by move history  
[ ] best move analysis  
[ ] move accuracy evaluation

## done

[x] init SFML  
[x] draw board  
[x] implement clicking  
[x] draw pieces  
[x] piece highlighting

implement moves:  
[x] pawn moves  
[x] rook moves  
[x] Taliah moves  
[x] Elephant moves  
[x] Camel moves  
[x] Mongol moves  
[x] Khan moves  
[x] Adminstrator moves  
[x] Vizir moves  
[x] Giraffe moves  
[x] War Engine moves

implement game logic:  
[x] checks  
[x] checkmates  
[x] draws  
[x] promotions
[x] turn history  
[x] implement menus  
[x] implement more menu features  
[x] implement AI
