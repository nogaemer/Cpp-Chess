cmake_minimum_required(VERSION 3.29)
project(Cpp_Chess)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")


add_executable(Cpp_Chess src/main.cpp
        src/pieces/Piece.h
        src/Board/Square.cpp
        src/Board/Square.h
        src/pieces/Knight.cpp
        src/pieces/Knight.h
        src/board/BoardManager.cpp
        src/board/BoardManager.h
        src/misc/Color.h
        src/misc/Pair.h
        src/pieces/Rook.cpp
        src/pieces/Rook.h
        src/pieces/King.cpp
        src/pieces/King.h
        src/board/Display.cpp
        src/board/Display.h
        src/pieces/Bishop.cpp
        src/pieces/Bishop.h
        src/pieces/Queen.cpp
        src/pieces/Queen.h
        src/pieces/Pawn.cpp
        src/pieces/Pawn.h
        src/pieces/MoveUpdater.cpp
        src/pieces/MoveUpdater.h
        src/pieces/Piece.cpp)
