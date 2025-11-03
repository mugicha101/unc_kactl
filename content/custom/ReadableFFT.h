/**
 * Author: Alexander Yoshida
 * Date: 202X
 * License: idc
 * Description: FFT
 */
#pragma once

#define _USE_MATH_DEFINES
#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef long double ld;
typedef complex<double> cnum;

// get nth root of unity to the pth power
cnum inline getW(int n, double p) {
    return pow(cnum{M_E, 0}, cnum{0, M_PI * 2.0 * p / n});
}

// precondition: n power of 2
vector<cnum> fftHelper(const vector<cnum>& xArr, cnum w, int offset, int spacing) {
    // setup
    int n = (int)xArr.size() >> spacing;
    int h = n >> 1;
    int d = 1 << spacing;

    // base case
    if (n == 1)
        return {xArr[offset]};

    // recurse
    vector<cnum> yEven = fftHelper(xArr, w * w, offset, spacing + 1);
    vector<cnum> yOdd = fftHelper(xArr, w * w, offset + d, spacing + 1);

    // merge
    vector<cnum> yArr(n);
    cnum wk = 1;
    for (int k = 0; k < h; ++k) {
        yArr[k] = yEven[k] + wk * yOdd[k];
        yArr[k + h] = yEven[k] - wk * yOdd[k];
        wk *= w;
    }
    return yArr;
}

// precondition: n power of 2
vector<cnum> fft(const vector<cnum>& xArr) {
    int n = (int)xArr.size();
    if (n ^ (n & (-n)))
        throw exception();
    return fftHelper(xArr, getW((int)xArr.size(), -1), 0, 0);
}

// precondition: n power of 2
vector<cnum> ifft(const vector<cnum>& yArr) {
    int n = (int)yArr.size();
    if (n ^ (n & (-n)))
        throw exception();
    vector<cnum> output = fftHelper(yArr, getW((int)yArr.size(), 1), 0, 0);
    cnum divN = cnum{1.0/yArr.size(), 0};
    for (cnum& y : output)
        y *= divN;
    return output;
}

// ensure arr length is power of 2 (not needed if fft package used)
void pad(vector<cnum>& arr) {
    int s = (int)arr.size();
    while (s ^ (s & (-s))) {
        arr.emplace_back(0,0);
        ++s;
    }
}

// multiplies polynomials represented by the coefficient arrays A and B
// returns a coefficient array
template<typename T>
vector<T> multiplyPolynomials(const vector<T> &A, const vector<T> &B) {
    vector<cnum> cA(A.size());
    vector<cnum> cB(B.size());
    for (int i = 0; i < (int)A.size(); ++i) cA[i] = (cnum)A[i];
    for (int i = 0; i < (int)A.size(); ++i) cB[i] = (cnum)B[i];
    vector<cnum> cC = multiplyPolynomials(cA, cB);
    vector<T> C(cC.size());
    for (int i = 0; i < (int)cC.size(); ++i) {
        if constexpr (std::is_integral_v<T>) C[i] = (T)round(cC[i].real());
        else (T)cC[i].real();
    }
    return C;
}

template<>
vector<cnum> multiplyPolynomials<cnum>(const vector<cnum> &oA, const vector<cnum> &oB) {
    vector<cnum> A = oA;
    vector<cnum> B = oB;

    // pad length of A and B to account for larger output C
    size_t outputSize = A.size() + B.size();
    while (A.size() < outputSize)
        A.emplace_back(0);
    while (B.size() < outputSize)
        B.emplace_back(0);

    // apply dft (O(nlgn))
    pad(A);
    pad(B);
    vector<cnum> dftA = fft(A);
    vector<cnum> dftB = fft(B);

    // point-wise multiply to get dft of A * B (O(n))
    vector<cnum> dftC;
    dftC.reserve(dftA.size());
    for (int i = 0; i < (int)dftA.size(); ++i)
        dftC.push_back(dftA[i] * dftB[i]);

    // inverse dft
    vector<cnum> C = ifft(dftC);

    // trim until matches outputSize
    while (C.size() > outputSize)
        C.pop_back();
    return C;
}

// cartesian sum: treat numbers as exponents
// A + B = multiplyPolynomials(A, B)