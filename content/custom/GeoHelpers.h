
/**
 * Author: Alexander Yoshida
 * Date: 202X
 * License: idc
 * Description: Geometry Helpers
 * Time: O(V + E)
 */
#pragma once

// compares 2 vectors, returns true if a has smaller angle than b (0 angle = x+ axis)
// when used in sort, orders clockwise (closest first when colinear)
template<typename T>
bool angleCmp(pair<T,T> a, pair<T,T> b) {
    auto [ax, ay] = a;
    auto [bx, by] = b;
    bool aHalf = (ay > 0) || (ay == 0 && ax >= 0);
    bool bHalf = (by > 0) || (by == 0 && bx >= 0);
    T cross = ax * by - ay * bx;

    // aHalf != bHalf: upper half < lower half
    // cross != 0: ccw < cw
    // cross == 0: colinear, choose closest first
    return aHalf == bHalf ? cross == 0 ? ax * ax + ay * ay < bx * bx + by * by : cross > 0 : aHalf;
}

// get value v representing separation (not actual separation, but can use for comparisons)
// v = a dot b / (mag(a) * mag(b))
template<typename T>
bool sepRank(pair<T,T> a, pair<T,T> b) {
    auto [ax, ay] = a;
    auto [bx, by] = b;
    return (long double)(ax * bx + ay * by) / sqrt((long double)((ax * ax + ay * ay) * (bx * bx + by * by)));
}