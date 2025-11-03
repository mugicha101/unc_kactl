/**
 * Author: Alexander Yoshida
 * Date: 202X
 * License: idc
 * Description: manachers algorithm computes longest palindrome centered at index i in O(N)
 * output format:
 *   pal = manacher(s)
 *   pal[i].first = length of longest odd length palindrome in s centered at index i
 *   pal[i].second = length of longest even length palindrome in s centered between index i and index i+1
 * note: doesn't work if chars with ascii value 0, 1, 2 are used
 */
#pragma once

vector<int> manacher_helper(const string &s) {
  int n = s.size() - 2;
  vector<int> p(n + 2);
  int l = 1, r = 1;
  for(int i = 1; i <= n; i++) {
    p[i] = max(0, min(r - i, p[l + (r - i)]));
    while(s[i - p[i]] == s[i + p[i]]) {
      p[i]++;
    }
    if(i + p[i] > r) {
      l = i - p[i], r = i + p[i];
    }
  }
  return p;
}

// finds odd and even palindromes
vector<pair<int,int>> manacher(const string &s) {
  string t;
  t += (char)1;
  for(char c : s) {
    t += (char)0;
    t += c;
  }
  t += (char)0;
  t += (char)2;
  vector<int> man = manacher_helper(t);
  vector<pair<int,int>> res(s.size());
  for (int i = 0; i < (int)s.size(); ++i) {
    res[i].first = man[i * 2 + 2] - 1; // odd
    res[i].second = man[i * 2 + 3] - 1; // even
  }
  return res;
}

// finds odd palindromes
vector<int> manacher_odd(const string &s) {
  string t;
  t += (char)1;
  t += s;
  t += (char)2;
  vector<int> man = manacher_helper(t);
  vector<int> res(man.size() - 2);
  for (int i = 0; i < (int)s.size(); ++i) {
    res[i] = man[i+1] * 2 - 1;
  }
  return res;
}