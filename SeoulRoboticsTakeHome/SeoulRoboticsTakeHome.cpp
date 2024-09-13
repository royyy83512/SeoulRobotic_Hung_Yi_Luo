//
//  SeoulRoboticsProject
//
//  Created by Hung Yi Luo on 2024/9/11.
//

#include <iostream>
#include <vector>
#include <set>
#include "algo.h"

int main(int argc, const char* argv[]) {
    int len_row, len_col, num_slash, num_back_slash;
    std::cin >> len_row >> len_col >> num_slash >> num_back_slash;

    // main data structure to store the positoins of mirrows in row and column respectively.
    // pair<int, int> : first parameter refers to the position of mirrow, 
    // second parameter refers to the mirror orientation, 0 if '/', 1 if '\'
    std::vector<std::set<std::pair<int, int>>> row_mirrows(len_row);
    std::vector<std::set<std::pair<int, int>>> col_mirrows(len_col);

    int x, y;
    for (int i = 0; i < num_slash; i++) {
        std::cin >> x >> y;
        row_mirrows[x - 1].insert({ y - 1, 0 });
        col_mirrows[y - 1].insert({ x - 1, 0 });
    }

    for (int i = 0; i < num_back_slash; i++) {
        std::cin >> x >> y;
        row_mirrows[x - 1].insert({ y - 1, 1 });
        col_mirrows[y - 1].insert({ x - 1, 1 });
    }

    std::vector<std::vector<int>> add_mirrows = {};
    int res = MirrorsAndRasors(row_mirrows, col_mirrows, len_row, len_col, add_mirrows);

    if (res == 0) {
        std::cout << 0 << std::endl;
    }
    else if (res == -1) {
        std::cout << -1 << std::endl;
    }
    else {
        std::cout << res << " " << add_mirrows[0][0] + 1 << " " << add_mirrows[0][1] + 1 << std::endl;
    }
    return 0;
}


