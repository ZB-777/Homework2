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
