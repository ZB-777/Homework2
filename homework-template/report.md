# 41343117

作業一

## 解題說明

Figure 1.這段程式是在說明多項式類別的抽象資料型態ADT，Figure 2.是在解釋 Polynomial 類別裡面實際儲存資料的結構，多項式的結構：

𝑝(𝑥)=𝑎0𝑥^𝑒0+𝑎1𝑥^𝑒1+⋯+𝑎𝑛𝑥^𝑒n	​

每一項都包含「係數」與「指數」，因此我建立了一個 Term 類別，用來存放一項的資料，再讓 Polynomial 類別使用 Term 陣列來組成整個多項式。

### Algorithm Design & Programming

主要類別

class Term：儲存每一項的 coef（係數）與 exp（指數）。

class Polynomial：包含多項式的資料與運算函式。

主要成員函式

Add(Polynomial poly)：回傳兩個多項式的加法結果。

Mult(Polynomial poly)：回傳兩個多項式的乘法結果。

Eval(float f)：代入 f 的值計算結果。

operator>>：讓使用者輸入多項式的每一項。

operator<<：以數學形式輸出多項式。
```cpp
#include <iostream>
#include <cmath>
using namespace std;

// ======================================
// Term 結構：用來表示一個多項式的單項
// ======================================
struct Term {
    float coef; // 係數
    int exp;    // 次方
};

// ======================================
// Polynomial 類別
// ======================================
class Polynomial {
private:
    Term *termArray; // 指向項目陣列
    int capacity;    // 陣列容量
    int terms;       // 實際項目數

public:
    // ===== 建構與解構 =====
    Polynomial(int cap = 10) {
        capacity = cap;
        terms = 0;
        termArray = new Term[capacity];
    }

    ~Polynomial() {
        delete[] termArray;
    }

    // ===== 新增一個項目 =====
    void newTerm(float coef, int exp) {
        if (terms == capacity) {
            capacity *= 2;
            Term *temp = new Term[capacity];
            for (int i = 0; i < terms; i++)
                temp[i] = termArray[i];
            delete[] termArray;
            termArray = temp;
        }
        termArray[terms].coef = coef;
        termArray[terms].exp = exp;
        terms++;
    }

    // ===== 加法 Add() =====
    Polynomial Add(const Polynomial &b) const {
        Polynomial result;
        int aPos = 0, bPos = 0;

        while (aPos < terms && bPos < b.terms) {
            if (termArray[aPos].exp == b.termArray[bPos].exp) {
                float c = termArray[aPos].coef + b.termArray[bPos].coef;
                if (fabs(c) > 1e-6)
                    result.newTerm(c, termArray[aPos].exp);
                aPos++;
                bPos++;
            } else if (termArray[aPos].exp > b.termArray[bPos].exp) {
                result.newTerm(termArray[aPos].coef, termArray[aPos].exp);
                aPos++;
            } else {
                result.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
                bPos++;
            }
        }

        // 將剩餘項目加入
        for (; aPos < terms; aPos++)
            result.newTerm(termArray[aPos].coef, termArray[aPos].exp);
        for (; bPos < b.terms; bPos++)
            result.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);

        return result;
    }

    // ===== 乘法 Mult() =====
    Polynomial Mult(const Polynomial &b) const {
        Polynomial result;

        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < b.terms; j++) {
                float c = termArray[i].coef * b.termArray[j].coef;
                int e = termArray[i].exp + b.termArray[j].exp;

                // 檢查結果中是否已有相同次方
                bool found = false;
                for (int k = 0; k < result.terms; k++) {
                    if (result.termArray[k].exp == e) {
                        result.termArray[k].coef += c;
                        found = true;
                        break;
                    }
                }
                if (!found)
                    result.newTerm(c, e);
            }
        }
        return result;
    }

    // ===== 求值 Eval() =====
    float Eval(float x) const {
        float sum = 0;
        for (int i = 0; i < terms; i++)
            sum += termArray[i].coef * pow(x, termArray[i].exp);
        return sum;
    }

    // ===== 輸入運算子多載 >> =====
    friend istream &operator>>(istream &in, Polynomial &p) {
        cout << "請輸入項數: ";
        in >> p.terms;
        for (int i = 0; i < p.terms; i++) {
            cout << "輸入第 " << i + 1 << " 項的係數與次方: ";
            in >> p.termArray[i].coef >> p.termArray[i].exp;
        }
        return in;
    }

    // ===== 輸出運算子多載 << =====
    friend ostream &operator<<(ostream &out, const Polynomial &p) {
        for (int i = 0; i < p.terms; i++) {
            if (i > 0 && p.termArray[i].coef >= 0)
                out << " + ";
            out << p.termArray[i].coef;
            if (p.termArray[i].exp > 0)
                out << "x^" << p.termArray[i].exp;
        }
        return out;
    }
};

// ======================================
// 主程式：測試所有功能
// ======================================
int main() {
    Polynomial p1, p2;

    cout << "=== 多項式輸入 ===" << endl;
    cin >> p1;
    cin >> p2;

    cout << "\nP1(x) = " << p1 << endl;
    cout << "P2(x) = " << p2 << endl;

    Polynomial sum = p1.Add(p2);
    Polynomial product = p1.Mult(p2);

    cout << "\nP1 + P2 = " << sum << endl;
    cout << "P1 * P2 = " << product << endl;

    float x;
    cout << "\n請輸入要代入的 x 值: ";
    cin >> x;
    cout << "P1(" << x << ") = " << p1.Eval(x) << endl;
    cout << "P2(" << x << ") = " << p2.Eval(x) << endl;

    return 0;
}


```
## 效能分析

1. 時間複雜度：程式的時間複雜度為 $O(n^2)$。
2. 空間複雜度：空間複雜度為 $O(n+m)$。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $p1(x)=2x^2+3x+1;p2(x) = x + 1$      | p1 + p2 = 2x² + 4x + 2 ;p1 * p2 = 2x³ + 5x² + 4x + 1      | p1 + p2 = 2x² + 4x + 2 ;p1 * p2 = 2x³ + 5x² + 4x + 1        |
| 測試二   | $n = 1$      | 1        | 1        |
| 測試三   | $n = 3$      | 6        | 6        |
| 測試四   | $n = 5$      | 15       | 15       |
| 測試五   | $n = -1$     | 異常拋出 | 異常拋出 |

### 效能量測

```shell
$ g++ -std=c++17 -o sigma sigma.cpp
$ ./sigma
6
```

### 心得討論

如何設計一個結構化的 C++ 類別。

抽象資料型態（ADT）的概念不只是理論，它可以轉化成實際程式架構。

雖然我對程式還不太熟，但透過這份作業，我理解了「資料結構」與「程式邏輯」之間的關聯。
