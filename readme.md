# SeoulRobotics Take-home project
## Table of Contents
- Problem analysis
- Algorithms and time complexity
- How to compile

## Problem Analysis
A raser enters the grid from the starting point. The beam reflects whenever a mirror is reached. The goal is to calculate:

1) If the dectector(ending point) can be reached.
2) if not, check if it is possible to place a mirror that makes the route ends at the dectector(ending point).
3) Output the position (r, c) where the mirror can be placed with the smallest lexicographical order.

To solve (1), we can simply traverse the route from the starting point to the dectector and see if the ending point is reached.

For (2) and (3), We need to find another route which start from the ending point. Although the two routes end at somewhere exceeding the grids, as long as the two routes intercect with each other, a mirror can be placed at the intersection that combines the two routes into one.

In other words, the number of intersections refers to the number of mirrors can be placed. If there are no intersections at all, then there are no possible solutions.

*note : a position that already has a mirror cannot be regard as an intersection, because the to two route change the direction when the mirror is reached. That is, while implementation, single grid overlap will not be regarded as intersection.*

## Algorithms and time complexity

The implementation can be seperated into 2 parts:

1. Find the route from both starting and ending point
2. Find intersections between two routes

### Find the route
The easiest way is to traverse each grid from the starting point, which we get the time and space complexity of O(n+m) in the worst case, where n refers to the length of row and m refers to the length of column.

However, we only need to check the cloest mirror in the same row or column for the next step instead of look through every grid in the row or column.

By storing mirrors in a std::set in each row and column, we can find the next mirror in O(log(n)) or O(log(m)) time. This approach is obviously better when the grid is huge with few mirrors. The total time complexity can improve to O(N * log(N)), where N is the number of mirrors.

*note: std::set is implemented by red-black tree, where the data is sorted. build-in functions, e.g., upper_bound() and lower_bound() provided a binary search approach.*

### Find intersections
Since in 2D grid, only horizontal lines may intersect with vertical lines, we check the 2 groups if they have intersections:
1. horizontal lines from starting point and vertical lines from ending point
2. vertical lines form starting point and horizontal lines from starting point

Instead of comparing every horizontal and vertical line segment, which needs O(n * m), we can implement Sweep line algorithm that optimize the time to O(N * log(N)), where N = n + m.

### Sweep line algorithm

reference: https://en.wikipedia.org/wiki/Sweep_line_algorithm

We sweep every line segment in the space from left to right vertically. A vertical line segment is considered a node, and a horizontal line segment is considered 2 nodes (leftmost node and right most node).

Firstly, we sort all nodes, then we iterate the nodes from left to right. The 3 cases are:

1. horizontally leftmost node: store it in the std::set (some my call it an active event).
2. vertical node: check if any active events is in the range from the vertical line segment(x_top, x_bot), if exist more than 1 node, then we considered it/them intersections.
3. horizontally rightmost node: delete the corresponding active event from the set.

Analysis:

1. sorting takes O(N * log(N)), where N is the number all line segments
2. inserting takes O(N * log(N)), more specifically, all horizontal line segments
3. deleting takes O(N * log(N)), more specifically, all horizontal line segments

Overall, the whole implmentation requires O(N * log(N)) time and space complexity.

## How to compile
1. This project is developed on Visual Studio 2022 on Windows. `SeoulRoboticsTakeHome.exe` is the default execution file compile on my computer. However, it might not work on every PC due to the fire wall to 3rd-party software. 
Besides, it can be opened directly by`SeoulRoboticsTakeHome.sln` on Visual Studio.

2. gcc compiler
There are 3 source code files in the project:
`SeoulRoboticsTakeHome.cpp`, `algo.cpp` and `algo.h`.
`SeoulRoboticsTakeHome.cpp` includes the main function,  `algo.cpp` includes the used functions, and `algo.h` is the header file to `algo.cpp`.

- Make sure you have the gcc compiler
- Goto the directory
`cd SeoulRobotics_Hung_Yi_Luo/SeoulRoboticsTakeHome`
- used the command below to complie the 2 cpp files
`g++ -o demo SeoulRoboticsTakeHome.cpp algo.cpp -I.`

Directories


```

¢|¢w¢w SeoulRobotics_Hung_Yi_Luo
    ¢u¢w¢w readme.md
    ¢u¢w¢w SeoulRoboticsTakeHome.sln
    ¢u¢w¢w x64
        ¢|¢w¢w Debug
            ¢|¢w¢w SeoulRoboticsTakeHome.exe
    ¢|¢w¢w SeoulRoboticsTakeHome
        ¢u¢w¢w SeoulRoboticsTakeHome.cpp
        ¢u¢w¢w algo.cpp
        ¢|¢w¢w algo.h
