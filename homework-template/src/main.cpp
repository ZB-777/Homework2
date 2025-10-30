#include <iostream>
#include <cmath>
using namespace std;

// ======================================
// Term 結構：用來表示多項式的一個項目
// ======================================
struct Term {
    float coef; // 係數 (coefficient)
    int exp;    // 次方 (exponent)
};

// ======================================
// Polynomial 類別：多項式 ADT
// ======================================
class Polynomial {
private:
    Term* termArray; // 指向存放項目的動態陣列
    int capacity;    // 陣列容量（可存幾個項目）
    int terms;       // 實際項目數（目前有幾項）

public:
    // ===== 建構子 =====
    // 預設容量為 10，建立空多項式
    Polynomial(int cap = 10) {
        capacity = cap;
        terms = 0;
        termArray = new Term[capacity];
    }

    // ===== 解構子 =====
    // 程式結束時釋放記憶體
    ~Polynomial() {
        delete[] termArray;
    }

    // ===== 確保容量足夠（防止記憶體爆掉）=====
    void ensureCapacity() {
        if (terms >= capacity) { // 若滿了就擴充
            capacity *= 2; // 擴大一倍容量
            Term* temp = new Term[capacity];
            for (int i = 0; i < terms; i++)
                temp[i] = termArray[i]; // 複製舊資料
            delete[] termArray;         // 釋放舊空間
            termArray = temp;           // 指向新空間
        }
    }

    // ===== 新增一個項目 =====
    // 自動擴充容量 + 加入新項
    void newTerm(float coef, int exp) {
        ensureCapacity(); // 確保空間夠用
        termArray[terms].coef = coef;
        termArray[terms].exp = exp;
        terms++;
    }

    // ===== 多項式加法 =====
    // 回傳目前多項式與 b 的加總
    Polynomial Add(const Polynomial& b) const {
        Polynomial result;
        int aPos = 0, bPos = 0;

        // 同步走訪兩個多項式
        while (aPos < terms && bPos < b.terms) {
            if (termArray[aPos].exp == b.termArray[bPos].exp) {
                // 如果次方相同 → 係數相加
                float c = termArray[aPos].coef + b.termArray[bPos].coef;
                if (fabs(c) > 1e-6) // 避免 0 項
                    result.newTerm(c, termArray[aPos].exp);
                aPos++; 
                bPos++;
            }
            else if (termArray[aPos].exp > b.termArray[bPos].exp) {
                // 如果左邊次方大 → 複製左邊項目
                result.newTerm(termArray[aPos].coef, termArray[aPos].exp);
                aPos++;
            }
            else {
                // 否則複製右邊項目
                result.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
                bPos++;
            }
        }

        // 將剩下的項目補上
        for (; aPos < terms; aPos++)
            result.newTerm(termArray[aPos].coef, termArray[aPos].exp);
        for (; bPos < b.terms; bPos++)
            result.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);

        return result;
    }

    // ===== 多項式乘法 =====
    // 回傳目前多項式與 b 的乘積
    Polynomial Mult(const Polynomial& b) const {
        Polynomial result;

        // 雙層迴圈逐項相乘
        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < b.terms; j++) {
                float c = termArray[i].coef * b.termArray[j].coef; // 係數相乘
                int e = termArray[i].exp + b.termArray[j].exp;     // 次方相加

                // 檢查結果中是否已有相同次方項
                bool found = false;
                for (int k = 0; k < result.terms; k++) {
                    if (result.termArray[k].exp == e) {
                        // 若已有相同次方 → 合併
                        result.termArray[k].coef += c;
                        found = true;
                        break;
                    }
                }

                // 若沒有相同次方 → 新增項目
                if (!found)
                    result.newTerm(c, e);
            }
        }
        return result;
    }

    // ===== 輸入運算子多載 =====
    // 逐行輸入「係數 次方」，當次方 < 0 表示結束
    friend istream& operator>>(istream& in, Polynomial& p) {
        p.terms = 0;
        float c; int e;

        while (true) {
            in >> c >> e;
            if (e < 0) break; // 次方為負 → 結束輸入
            p.newTerm(c, e);
        }

        return in;
    }

    // ===== 輸出運算子多載 =====
    // 以數學格式輸出多項式（例如 3x^2 + 2x + 1）
    friend ostream& operator<<(ostream& out, const Polynomial& p) {
        if (p.terms == 0) {
            out << "0"; // 空多項式輸出 0
            return out;
        }

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
// 主程式：測試多項式的加法與乘法
// ======================================
int main() {
    Polynomial p1, p2;

    cout << "=== 多項式輸入 ===" << endl;
    cout << "請依序輸入係數與次方（每行一組），次方 < 0 結束" << endl;
    cout << "例如：" << endl;
    cout << "3 2  (代表 3x^2)" << endl;
    cout << "1 1  (代表 x)" << endl;
    cout << "2 0  (代表常數 2)" << endl;
    cout << "-1   (結束)" << endl;

    // 輸入第一個多項式
    cout << "\n----- 輸入第一個多項式 -----" << endl;
    cin >> p1;

    // 輸入第二個多項式
    cout << "\n----- 輸入第二個多項式 -----" << endl;
    cin >> p2;

    // 顯示兩個多項式
    cout << "\nP1(x) = " << p1 << endl;
    cout << "P2(x) = " << p2 << endl;

    // 計算加法與乘法結果
    Polynomial sum = p1.Add(p2);
    Polynomial product = p1.Mult(p2);

    // 顯示結果
    cout << "\nP1 + P2 = " << sum << endl;
    cout << "P1 * P2 = " << product << endl;

    return 0;
}


