/**
 * Author: idk
 * Date: 202X
 * License: idc
 * Description: ordered set/multiset that can find element rank in logarithmic time
 */
#pragma once

#include <bits/extc++.h>
// #include <ext/pb_ds/assoc_container.hpp>
// #include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
template <class T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
template <class T>
using ordered_multiset = tree<T, null_type, less_equal<T>, rb_tree_tag, tree_order_statistics_node_update>;

void foo() {
    ordered_set<int> s;
    int k = 10;
    s.order_of_key(k); // Number of items strictly smaller than k
    s.find_by_order(k); // K-th element in a set (counting from zero)
}