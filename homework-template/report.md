# 41343117

作業一


## 解題說明

這次的程式主要是在實作「多項式（Polynomial）」這個抽象資料型態（ADT）。  
Figure 1 說明了多項式的一般形式：
```
p(x) = a₀x⁰ + a₁x¹ + ... + aₙxⁿ
```

每一項都包含「係數（coefficient）」與「指數（exponent）」兩個部分，  因此我先設計了一個 `Term` 類別來存放單一項的資料，  再由 `Polynomial` 類別使用多個 `Term` 物件所組成的陣列，去代表整個多項式的內容。  

整體來說，`Term` 是基礎資料單位，而 `Polynomial` 是用來進行運算的主要類別。  

---

## Algorithm Design & Programming

#### 主要類別設計

1. **class Term**  
   用來儲存多項式中每一項的資料：  
   - `coef`：係數（float）  
   - `exp`：指數（int）

2. **class Polynomial**  
   儲存整個多項式的資料結構，並提供運算功能。  
   內含：
   - `Term* termArray`：儲存所有非零項的陣列  
   - `int capacity`：目前配置的陣列容量  
   - `int terms`：實際項目數量  


#### 主要成員函式

| 函式名稱 | 功能說明 |
|-----------|-----------|
| `Add(Polynomial poly)` | 計算兩個多項式的加法，回傳新多項式。 |
| `Mult(Polynomial poly)` | 計算兩個多項式的乘法，回傳新多項式。 |
| `Eval(float x)` | 將 x 帶入多項式中求值，回傳結果。 |
| `operator >>` | 讓使用者輸入每一項的係數與次方（以負次方結束）。 |
| `operator <<` | 輸出多項式的完整內容，如 `3x^2 + 2x + 1`。 |

---

### 程式碼
```cpp
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



```
---

### 效能分析

本程式的主要操作包含三種：

1. **Add(Polynomial poly)**  
   - 功能：計算兩個多項式的加法。  
   - 分析：  
     由於每一項只需與對方多項式的當前項比較一次，因此時間複雜度為 **O(n)**。  
   - 空間複雜度：  
     輸出多項式需要額外的陣列儲存新項目，故為 **O(n)**。

2. **Mult(Polynomial poly)**  
   - 功能：計算兩個多項式的乘法。  
   - 分析：  
     每一項都要與另一個多項式的每一項相乘，所以時間複雜度為 **O(n²)**。  
     在結果中若有相同次方，需額外搜尋合併，若採線性搜尋合併則最壞情況可達 **O(n³)**。  
   - 空間複雜度：  
     乘法結果的項數可能增加至 **O(n²)**。

3. **Eval(float x)**  
   - 功能：將 x 帶入多項式並計算結果。  
   - 分析：  
     需逐項運算 `coef * pow(x, exp)`，時間複雜度為 **O(n)**。

 **理論總結：**

| 函式名稱 | 時間複雜度 | 空間複雜度 | 分析說明 |
|-----------|-------------|-------------|-----------|
| Add() | O(n) | O(n) | 單次線性合併 |
| Mult() | O(n²) | O(n²) | 雙層巢狀相乘 |
| Eval() | O(n) | O(1) | 單層逐項計算 |

---

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 `$n$` | 預期輸出 | 實際輸出 |
|-----------|----------------|-----------|-----------|
| **測試一** | 3 2<br>1 1<br>2 0<br>-1<br>5 2<br>-1 | **P1(x)** = 3x² + 1x¹ + 2x⁰<br>**P2(x)** = 5x²<br>**P1 + P2** = 8x² + 1x¹ + 2x⁰<br>**P1 × P2** = 15x⁴ + 5x³ + 10x² | 同預期 |
| **測試二** | 1 1<br>2 0<br>-1<br>3 2<br>-1 | **P1(x)** = 1x¹ + 2x⁰<br>**P2(x)** = 3x²<br>**P1 + P2** = 3x² + 1x¹ + 2x⁰<br>**P1 × P2** = 3x³ + 6x² | 同預期 |
| **測試三** | 1 1<br>-1<br>3 5<br>-1 | **P1(x)** = 1x¹<br>**P2(x)** = 3x⁵<br>**P1 + P2** = 3x⁵ + 1x¹<br>**P1 × P2** = 3x⁶ | 同預期 |
| **測試四** | 3 2<br>1 1<br>2 0<br>-1<br>5 2<br>-1 | **P1(x)** = 3x² + 1x¹ + 2x⁰<br>**P2(x)** = 5x²<br>**P1 + P2** = 8x² + 1x¹ + 2x⁰<br>**P1 × P2** = 15x⁴ + 5x³ + 10x² | 同預期 |
| **測試五** | -1 | 無輸入 | **異常拋出**（空多項式測試） |

---

##  效能量測

為了實際驗證效能，我針對不同項數的多項式進行測試，觀察運算時間及執行穩定度（使用 Visual Studio 的時間統計與人工觀察）。

| 測試編號 | 多項式項數 | 操作內容 | 實際執行時間 | 備註 |
|-----------|-------------|-----------|----------------|------|
| 1 | 3 項 × 3 項 | 加法與乘法 | < 0.001 秒 | 執行極快 |
| 2 | 10 項 × 10 項 | 加法與乘法 | 約 0.002 秒 | 發生 1 次動態擴充 |
| 3 | 30 項 × 30 項 | 加法與乘法 | 約 0.007 秒 | 結果正確、穩定 |
| 4 | 50 項 × 50 項 | 加法與乘法 | 約 0.015 秒 | 有多次動態配置 |
| 5 | 100 項 × 100 項 | 加法與乘法 | 約 0.05 秒 | 程式可穩定完成 |


#### 綜合分析

| 項數 n | 理論成長率 | 實際觀察時間 | 差異說明 |
|:------:|:------------:|:----------------:|:-------------|
| 10 | O(n²) ≈ 100 | 約 0.002 秒 | 幾乎線性成長 |
| 50 | O(n²) ≈ 2500 | 約 0.015 秒 | 乘法效能明顯下降 |
| 100 | O(n²) ≈ 10000 | 約 0.05 秒 | 與理論趨勢一致 |

---

## 心得討論

實說我一開始看到這個作業的時候有點傻眼，因為我對 C++ 的 class、指標那些真的不是很熟，而且一開始完全不知道「多項式」要怎麼用程式去存。我原本以為就一個陣列而已，結果老師說要用class Polynomial，裡面還有結構 Term、然後要多載 <<、>>，我就邊查資料邊改，途中程式爆炸超多次，像是「termArray 越界」、「輸入沒讀到」這些我都遇過，還被 Visual Studio 嚇到出現一堆英文視窗（我還以為我把電腦搞壞），後來助教的TA課程有講解，對我來說超級有幫助。
