/**
 * Author: Alexander Yoshida
 * Date: 202X
 * License: idc
 * Description: a bunch segment trees
 */
#pragma once

template<typename A, typename B> ostream& operator<<(ostream &os, const pair<A, B> &p) { return os << '(' << p.first << ", " << p.second << ')'; }

// STANDARD SEG TREE
// array based
// inclusive queries
class SegTree {
    // config
    using T = int; // type
    const T ZRV = 0; // value to return if range is 0
    const T IV = 0; // initial value of elements
    inline T op(T l, T r) const { return l + r; } // range query operator
    // config end

    vector<T> nodes;

    inline void asn(int i) { nodes[i] = op(nodes[left(i)], nodes[right(i)]); }
    inline int left(int i) const { return (i << 1) + 1; }
    inline int right(int i) const { return (i << 1) + 2; }
    inline int parent(int i) const { return (i - 1) >> 1; }
    inline int elem(int i) const { return (nodes.size() >> 1) + i; }
    inline bool leaf(int i) const { return i >= elem(0); }

    T query_helper(int index, int size, int first, int last) const {
        if (first >= size || last < 0) return ZRV;
        if (first <= 0 && last >= size-1) return nodes[index];
        size >>= 1;
        return op(query_helper(left(index), size, first, last), query_helper(right(index), size, first - size, last - size));
    }

    void dump_helper(int index, int indent) const {
        cout << string(indent * 2, ' ') << "(" <<  nodes[index] << ")" << endl;
        if (leaf(index)) return;
        dump_helper(left(index), indent+1);
        dump_helper(right(index), indent+1);
    }

public:

    SegTree(int size) {
        --size;
        int n = 1;
        while (size) {
            n <<= 1;
            size >>= 1;
        }
        nodes.resize(n * 2 - 1, IV);
        for (int i = n-2; i >= 0; --i) asn(i);
    }

    T get(int index) const { return nodes[elem(index)]; }
    T query(int first, int last) const { return query_helper(0, (nodes.size() + 1) >> 1, first, last); }
    void dump() const { dump_helper(0, 0); }

    void assign(int index, T value) {
        index = elem(index);
        nodes[index] = value;
        while (index) {
            index = parent(index);
            asn(index);
        }
    }
};

// LAZY SEG TREE
// array based
// inclusive queries
class LazySegTree {
    // config
    using T = ll; // type
    using L = ll; // lazy type (must be numeric)
    const T ZRV = 0; // value to return if range is 0
    const T IV = 0; // initial value of elements
    inline T op(T l, T r) const { return l + r; } // range query operator
    inline T lop(T v, L l, int s) const { return v + l * s; } // lazy update operator
    // config end

    vector<L> lazy;
    vector<T> nodes;

    inline void asn(int i, int size) { nodes[i] = op(at(left(i), size >> 1), at(right(i), size >> 1)); }
    inline T at(int i, int size) const { return lop(nodes[i], lazy[i], size); }
    inline int left(int i) const { return (i << 1) + 1; }
    inline int right(int i) const { return (i << 1) + 2; }
    inline int parent(int i) const { return (i - 1) >> 1; }
    inline int elem(int i) const { return (nodes.size() >> 1) + i; }
    inline bool leaf(int i) const { return i >= elem(0); }

    T update_helper(int index, int size, int first, int last, L value) {
        if (first >= size || last < 0) return ZRV;
        if (first <= 0 && last >= size-1) {
            lazy[index] += value;
            return at(index, size);
        }
        update_helper(left(index), size >> 1, first, last, value);
        update_helper(right(index), size >> 1, first - (size >> 1), last - (size >> 1), value);
        asn(index, size);
        return at(index, size);
    }

    T query_helper(int index, int size, int first, int last) const {
        if (first >= size || last < 0) return ZRV;
        if (first <= 0 && last >= size-1) return at(index, size);
        return lop(op(query_helper(left(index), size >> 1, first, last), query_helper(right(index), size >> 1, first - (size >> 1), last - (size >> 1))), lazy[index], min(last, size-1) + 1 - max(first, 0));
    }

    void dump_helper(int index, int indent, int size) const {
        cout << string(indent * 2, ' ') << index << ":l(" <<  nodes[index] << "," << lazy[index] << "," << size << ")=" << lop(nodes[index], lazy[index], size) << endl;
        if (leaf(index)) return;
        dump_helper(left(index), indent+1, size >> 1);
        dump_helper(right(index), indent+1, size >> 1);
    }

public:
    
    LazySegTree(int size) {
        --size;
        int n = 1;
        while (size) { n <<= 1; size >>= 1; }
        nodes.resize(n * 2 - 1, IV);
        lazy.resize(n * 2 - 1, 0);
        for (int i = n-2; i >= 0; --i) asn(i, 1);
    }

    void assign(int index, L value) { update(index, index, value - query(index, index)); }
    void update(int first, int last, L value) { update_helper(0, (nodes.size() + 1) >> 1, first, last, value); }
    T query(int first, int last) const { return query_helper(0, (nodes.size() + 1) >> 1, first, last); }
    void dump() const { dump_helper(0, 0, (nodes.size() + 1) >> 1); }
};

// LAZY BOUNDS SEG TREE
// array based
// inclusive queries
class LazySegTree {
    // config
    using T = pair<int,int>; // type
    using L = int; // lazy type (must be numeric)
    const T ZRV = make_pair(INT_MAX, INT_MIN); // value to return if range is 0
    const T IV = make_pair(INT_MAX, INT_MIN); // initial value of elements
    inline T op(T l, T r) const { return make_pair(min(l.first, r.first), max(l.second, r.second)); } // range query operator
    inline T lop(T v, L l) const { return make_pair(v.first + l, v.second + l); } // lazy update operator
    // config end

    vector<L> lazy;
    vector<T> nodes;

    inline void asn(int i) { nodes[i] = op(at(left(i)), at(right(i))); }
    inline T at(int i) const { return lop(nodes[i], lazy[i]); }
    inline int left(int i) const { return (i << 1) + 1; }
    inline int right(int i) const { return (i << 1) + 2; }
    inline int parent(int i) const { return (i - 1) >> 1; }
    inline int elem(int i) const { return (nodes.size() >> 1) + i; }
    inline bool leaf(int i) const { return i >= elem(0); }

    T update_helper(int index, int size, int first, int last, L value) {
        if (first >= size || last < 0) return ZRV;
        if (first <= 0 && last >= size-1) {
            lazy[index] += value;
            return at(index);
        }
        size >>= 1;
        update_helper(left(index), size, first, last, value);
        update_helper(right(index), size, first - size, last - size, value);
        asn(index);
        return at(index);
    }

    T query_helper(int index, int size, int first, int last) const {
        if (first >= size || last < 0) return ZRV;
        if (first <= 0 && last >= size-1) return at(index);
        size >>= 1;
        return lop(op(query_helper(left(index), size, first, last), query_helper(right(index), size, first - size, last - size)), lazy[index]);
    }

    void dump_helper(int index, int indent) const {
        cout << string(indent * 2, ' ') << "(" <<  nodes[index] << " + " << lazy[index] << ")" << endl;
        if (leaf(index)) return;
        dump_helper(left(index), indent+1);
        dump_helper(right(index), indent+1);
    }

public:
    
    LazySegTree(int size) {
        --size;
        int n = 1;
        while (size) { n <<= 1; size >>= 1; }
        nodes.resize(n * 2 - 1, IV);
        lazy.resize(n * 2 - 1, 0);
        for (int i = n-2; i >= 0; --i) asn(i);
    }

    T get(int index) const { return nodes[elem(index)]; }
    void update(int first, int last, L value) { update_helper(0, (nodes.size() + 1) >> 1, first, last, value); }
    T query(int first, int last) const { return query_helper(0, (nodes.size() + 1) >> 1, first, last); }
    void dump() const { dump_helper(0, 0); }

    void assign(int index, T value) {
        index = elem(index);
        nodes[index] = value;
        lazy[index] = 0;
        while (index) {
            index = parent(index);
            asn(index);
        }
    }
};

// keeps track of length of covered range of intervals
// lazy prop segtree
struct ItvTree {
    ItvTree *left = nullptr;
    ItvTree *right = nullptr;
    int lx; int rx; // covered range
    int count = 0; // number of times completely covered by an interval
    int subCoverLen  = 0; // length of covered range of children (if count > 0, the true cover is just rx - lx)

    // note: points must be sorted
    ItvTree(vector<int> &points, int begin = 0, int end = -1) {
        if (end == -1) end = points.size() - 1;
        int itvs = end - begin;
        lx = points[begin];
        rx = points[end];
        if (itvs >= 2) {
            int mid = begin + (itvs >> 1);
            left = new ItvTree(points, begin, mid);
            right = new ItvTree(points, mid, end);
        }
    }

    // note: if input values are not valid points, will fail assertion

    // gets cover length of range [l, r]
    int query(int l, int r) {
        if (r <= lx || l >= rx) return 0;
        if (count > 0) return min(r, rx) - max(l, lx);
        if (l <= lx && r >= rx) return subCoverLen;
        assertNonleaf(l, r);
        return left->query(l, r) + right->query(l, r);
    }

    // inserts an interval
    // returns true iff coverLen changed
    bool insert(int l, int r) {
        if (r <= lx || l >= rx) return false;
        if (l <= lx && r >= rx) return ++count == 1;
        assertNonleaf(l, r);
        if (!left->insert(l, r) & !right->insert(l, r)) return false;
        subCoverLen = left->coverLen() + right->coverLen();
        return count <= 0;
    }

    // removes an interval
    // returns true iff coverLen changed
    bool erase(int l, int r) {
        if (r <= lx || l >= rx) return false;
        if (l <= lx && r >= rx) return --count == 0;
        assertNonleaf(l, r);
        if (!left->erase(l, r) & !right->erase(l, r)) return false;
        subCoverLen = left->coverLen() + right->coverLen();
        return count <= 0;
    }

    inline int len() const { return rx - lx; }
    inline bool leaf() const { return !left; }
    inline int coverLen() const { return count > 0 ? len() : subCoverLen; }
    inline void assertNonleaf(int opL, int opR) const {
        if (leaf()) {
            cerr << "is a leaf: [" << lx << "," << rx << "] op range: [" << opL << "," << opR << "]" << endl;
            exit(-1);
        }
    }
};

// lis tree
// st[i] = length of lis at rank i (need to rerank before this)
class SegTree {
    // config
    using T = pair<int,int>; // type
    const T ZRV = {0,0}; // value to return if range is 0
    const T IV = {0,0}; // initial value of elements
    inline T op(T l, T r) const { return l.first == r.first ? make_pair(l.first, (l.second + r.second) % MOD) : max(l, r); } // range query operator
    // config end

    vector<T> nodes;

    inline void asn(int i) { nodes[i] = op(nodes[left(i)], nodes[right(i)]); }
    inline int left(int i) const { return (i << 1) + 1; }
    inline int right(int i) const { return (i << 1) + 2; }
    inline int parent(int i) const { return (i - 1) >> 1; }
    inline int elem(int i) const { return (nodes.size() >> 1) + i; }
    inline bool leaf(int i) const { return i >= elem(0); }

    T query_helper(int index, int size, int first, int last) const {
        if (first >= size || last < 0) return ZRV;
        if (first <= 0 && last >= size-1) return nodes[index];
        size >>= 1;
        return op(query_helper(left(index), size, first, last), query_helper(right(index), size, first - size, last - size));
    }

    void dump_helper(int index, int indent) const {
        cout << string(indent * 2, ' ') << "(" <<  nodes[index] << ")" << endl;
        if (leaf(index)) return;
        dump_helper(left(index), indent+1);
        dump_helper(right(index), indent+1);
    }

public:

    SegTree(int size) {
        --size;
        int n = 1;
        while (size) {
            n <<= 1;
            size >>= 1;
        }
        nodes.resize(n * 2 - 1, IV);
        for (int i = n-2; i >= 0; --i) asn(i);
    }

    T get(int index) const { return nodes[elem(index)]; }
    T query(int first, int last) const { return query_helper(0, (nodes.size() + 1) >> 1, first, last); }
    void dump() const { dump_helper(0, 0); }

    void assign(int index, T value) {
        index = elem(index);
        nodes[index] = value;
        while (index) {
            index = parent(index);
            asn(index);
        }
    }
};