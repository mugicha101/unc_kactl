/**
 * Author: Alexander Yoshida
 * Date: 202X
 * License: idc
 * Description: readable knuth-moris pratt
 */
#pragma once

// knuth morris pratt string matching
struct KMP {
    // returns p where p[i] = length of longest prefix of P that is a proper suffix of P[0..<i] (proper meaning cannot equal P[0..<i])
    vector<int> prefixFunc(const string &s) {
        vector<int> p(s.size());
        int k = 0;
        for (int q = 1; q < (int)s.size(); ++q) {
            while (k > 0 && s[k] != s[q]) k = p[k-1];
            k += s[k] == s[q];
            p[q] = k;
        }
        return p;
    }

    string pat;
    vector<int> pref;
    KMP(string pattern) : pat(pattern), pref(prefixFunc(pattern)) {
        pat += (char)0;
    }

    // finds first instance of pattern in s
    int match(const string &s) {
        int m = (int)pref.size();
        int q = 0;
        for (int i = 0; i < (int)s.size(); ++i) {
            while (q > 0 && pat[q] != s[i]) q = pref[q-1];
            q += pat[q] == s[i];
            if (q == m) return i-m+1;
        }
        return -1;
    }

    // finds all instances of pattern in s
    vector<int> matchAll(const string &s) {
        int m = (int)pref.size();
        int q = 0;
        vector<int> matches;
        for (int i = 0; i < (int)s.size(); ++i) {
            while (q > 0 && pat[q] != s[i]) q = pref[q-1];
            q += pat[q] == s[i];
            if (q == m) matches.push_back(i-m+1);
        }
        return matches;
    }

    // count number of matches
    int matchCount(const string &s) {
        int m = (int)pat.size();
        int q = 0;
        int matches = 0;
        for (int i = 0; i < (int)s.size(); ++i) {
            while (q > 0 && pat[q] != s[i]) q = pref[q-1];
            q += pat[q] == s[i];
            matches += q == m;
        }
        return matches;
    }
};