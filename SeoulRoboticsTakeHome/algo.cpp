#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

// coordinate of line segment (x0, y0) - (x1, y1)
struct LineSegment {
    int x0;
    int y0;
    int x1;
    int y1;

    LineSegment(int x0, int y0, int x1, int y1) : x0(x0), y0(y0), x1(x1), y1(y1) {}
};

// In sweep-line algorithm, line segment is sweeped from left to right.
// A vertical line is seen as one node, and horizontal line includes starting node and ending node.
// 
// @param is_vertical   whether the line segment is a vertical line
// @param y             current y of line sweeping process
// @param x_top         upper x coordinate
// @param x_bot         lower x coordinate (x_top != x_bot if is_vertical, x_top == x_bot if !isvertical)
// @param priority      the priority of finding the nodes if in the same vertical line
//                      -1, if end point of a horizontal line (highest priority)
//                       0, if it is a vertical line
//                       1, if start point of a horizontal line (lowest priority)
struct SweepNode {
    bool is_vertical;
    int y;
    int x_top;
    int x_bot;
    int priority;

    SweepNode(bool is_vertical, int y, int xTop, int xBot, int priority) : is_vertical(is_vertical), y(y), x_top(xTop), x_bot(xBot), priority(priority) {}
};

// Custom compare funtion for sorting the sweep line nodes
// Nodes are found from left to right, if y axis is the same then compare priority
static bool CompareSweepLine(SweepNode& a, SweepNode& b) {
    if (a.y != b.y) {
        return a.y < b.y;
    }
    return a.priority < b.priority;
}

// Calculate the line segments from a certain starting point, output line segments into horizontal and vertical lines respectively
// Return true if the beam is able to reach the goal
// @param row           the positions of mirrors in the row
// @param col           the positions of mirrors in the column
// @param hor_lines     output horizontal line segments
// @param vert_lines    output vertical line segments
// @param x_start       starting point of x
// @param y_start       starting point of y
// @param len_row       length of row
// @param len_col       length of column
// @param dir           direction, 'u' : up, 'd' : down, 'l' : left, 'r' : right
static bool GetLineSegments(std::vector<std::set<std::pair<int, int>>>& row, std::vector<std::set<std::pair<int, int>>>& col, std::vector<LineSegment>& hor_lines, std::vector<LineSegment>& vert_lines, int x_start, int y_start, int len_row, int len_col, char dir) {
    int x_cur = x_start;
    int y_cur = y_start;
    while (x_cur >= 0 && x_cur < len_row && y_cur >= 0 && y_cur < len_col) {

        // find the cloest mirror on the right
        if (dir == 'r') {
            auto cloest = row[x_cur].upper_bound({ y_cur, 1 });
            // if no mirrors, insert the last line the reaches the boarder then break
            if (cloest == row[x_cur].end()) {
                hor_lines.push_back({ x_cur, y_cur, x_cur, len_col });

                // if no mirror and the beam reach the goal then it's safe, return true
                if (x_cur == len_row - 1) {
                    return true;
                }
                break;
            }
            hor_lines.push_back({ x_cur, y_cur, x_cur, cloest->first });
            y_cur = cloest->first;
            if (cloest->second == 0) {
                dir = 'u';
            }
            else {
                dir = 'd';
            }
        }

        // find the cloest mirror on the left
        else if (dir == 'l') {
            auto cloest = row[x_cur].lower_bound({ y_cur, 0 });
            if (cloest == row[x_cur].begin()) {
                hor_lines.push_back({ x_cur, -1, x_cur, y_cur });
                break;
            }
            --cloest;
            hor_lines.push_back({ x_cur, cloest->first, x_cur, y_cur });
            y_cur = cloest->first;
            if (cloest->second == 0) {
                dir = 'd';
            }
            else {
                dir = 'u';
            }
        }

        // find the cloest mirror below
        else if (dir == 'd') {
            auto cloest = col[y_cur].upper_bound({ x_cur, 1 });
            if (cloest == col[y_cur].end()) {
                vert_lines.push_back({ x_cur, y_cur, len_row, y_cur });
                break;
            }
            vert_lines.push_back({ x_cur, y_cur, cloest->first, y_cur });
            x_cur = cloest->first;
            if (cloest->second == 0) {
                dir = 'l';
            }
            else {
                dir = 'r';
            }
        }

        // find the cloest mirror above
        else if (dir == 'u') {
            auto cloest = col[y_cur].lower_bound({ x_cur, 1 });
            if (cloest == col[y_cur].begin()) {
                vert_lines.push_back({ -1, y_cur, x_cur, y_cur });
                break;
            }
            --cloest;
            vert_lines.push_back({ cloest->first, y_cur, x_cur, y_cur });
            x_cur = cloest->first;
            if (cloest->second == 0) {
                dir = 'r';
            }
            else {
                dir = 'l';
            }
        }
    }
    return false;
}

// Calculate intersections from a set of horizontal lines and a set of vertical lines
// Implementation of sweep line algorithm. 
// @param hor_lines         input horizontal line segments
// @param vert_lines        input verical line segments
// @param intersections     output all the intersections
static void FindIntersections(std::vector<LineSegment>& hor_lines, std::vector<LineSegment>& vert_lines, std::vector<std::vector<int>>& intersections) {
    std::vector<SweepNode> sweepLine;
    for (auto& v : vert_lines) {
        SweepNode node(true, v.y0, v.x0, v.x1, 0);
        sweepLine.push_back(node);
    }
    for (auto& h : hor_lines) {
        SweepNode nodeLeft(false, h.y0, h.x0, h.x1, -1);
        sweepLine.push_back(nodeLeft);
        SweepNode nodeRight(false, h.y1, h.x0, h.x1, 1);
        sweepLine.push_back(nodeRight);
    }

    // sort the nodes from coordinate left to right by using customized comparison
    sort(sweepLine.begin(), sweepLine.end(), CompareSweepLine);

    std::set<int> hor_nodes;
    for (auto& node : sweepLine) {
        if (!node.is_vertical) {
            // for all non-ending horizontal lines, insert them into a set
            // remove the starting node of horizontal line if we find the ending node
            if (node.priority == -1) {
                hor_nodes.insert(node.x_top);
            }
            else if (node.priority == 1) {
                hor_nodes.erase(node.x_top);
            }
        }
        else {
            // for each vertical line, check if there are non-ending horizontal lines in the set which is in the range of (x_top, x_bot)
            int top = node.x_top;
            int bot = node.x_bot;
            auto it_left = hor_nodes.upper_bound(top);
            auto it_right = hor_nodes.lower_bound(bot);
            if (it_right == hor_nodes.begin()) {
                continue;
            }
            it_right--;
            if (*it_left > *it_right) {
                continue;
            }
            for (auto i = it_left; i != next(it_right); i++) {
                intersections.push_back({ *i, node.y });
            }
        }
    }
}

int MirrorsAndRasors(std::vector<std::set<std::pair<int, int>>> row_mirrors, std::vector<std::set<std::pair<int, int>>> col_mirrors, int len_row, int len_col, std::vector<std::vector<int>>& add_mirrors) {

    std::vector<LineSegment> hor_from_start, vert_from_start;
    std::vector<LineSegment> hor_from_end, vert_from_end;

    // 1) walk through the grid and get all line segments from starting point
    // output line segments in hor_from_start if horizontal lines
    // output line segments in vert_from_start if vertical lines
    bool is_safe = GetLineSegments(row_mirrors, col_mirrors, hor_from_start, vert_from_start, 0, 0, len_row, len_col, 'r');

    // 2) if the route ends at the safe point, return 0
    if (is_safe) {
        return 0;
    }

    // 3) if safe point is not reach, trace back from the safe point and get all line segments
    // output line segments in hor_from_end if horizontal lines
    // output line segments in ver_from_end if vertical lines
    GetLineSegments(row_mirrors, col_mirrors, hor_from_end, vert_from_end, len_row - 1, len_col - 1, len_row, len_col, 'l');

    // 4) find all the intersections between the horizontal and vertical lines
    // each intersection refer to a possible position of added mirror
    FindIntersections(hor_from_start, vert_from_end, add_mirrors);
    FindIntersections(hor_from_end, vert_from_start, add_mirrors);

    // sort all the possible mirrors in lexicographical order
    sort(add_mirrors.begin(), add_mirrors.end());

    // if no mirror, return -1 (not possible to add mirror to reach the safe point)
    if (add_mirrors.size() == 0) {
        return -1;
    }

    // return number of mirrors if the safe point can be reached
    int num_mirrors = add_mirrors.size();
    return num_mirrors;
}