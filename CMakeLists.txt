cmake_minimum_required(VERSION 3.16)
project(WinterRank)

set(CMAKE_CXX_STANDARD 14)

add_executable(WinterRank main24a2.cpp Team.h Team.cpp olympics24a2.cpp olympics24a2.h hash_table.cpp hash_table.h
        Player.cpp to_bubble_info.h)
#add_executable(WinterRank RankTree.h main.cpp Team.h Team.cpp Team_Key.h Player.h Player.cpp AvlTree.h
#        olympics24a2.cpp hash_table.h
#        )