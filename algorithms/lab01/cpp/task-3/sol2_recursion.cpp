// SPDX-License-Identifier: BSD-3-Clause

#include <iostream> // cin, cout
#include <fstream> // ifstream, ofstream
using namespace std;

class Task {
public:
    void solve() {
        read_input();
        print_output(get_result());
    }

private:
    int n, x, y;

    void read_input() {
        ifstream fin("in");
        fin >> n >> x >> y;
        fin.close();
    }

    /*
          0  1
      0 |00|01|                | 0| 1|
        ------- => cadrane:    -------
      1 |10|11|                | 2| 3|

        Pentru a determina o valoare din celula (x, y) aflată in cadranele 1, 2 sau 3 este
        suficient să translatăm celula respectivă în cadranul 0, recursiv, scăzând un offset.

        Pentru a determina coordonatele de trecere din cadranele 1, 2 sau 3 în cadranul 0,
        trebuie să determinăm dacă x este în jumătatea inferioară sau superioară, respectiv
        dacă y este în jumătatea din stânga sau dreapta. Notăm cu mid jumătate din dimensiunea
        matricei, adică mid = 2^(n-1).

        Astfel, în funcție de cadran vom avea:
        Cadranul 0: (x, y)
        Cadranul 1: (x, y - mid)
        Cadranul 2: (x - mid, y)
        Cadranul 3: (x - mid, y - mid)

        Fie off = 2^(2n-2). Trecerea din cadranul 0 în cadranul 1, 2 sau 3 se face dupa formula
        value * quadrant * off, unde "value" denotă corespondentul lui (x, y) în cadranul 0,
        "quadrant" denotă valoarea cadranului, iar off = 2^(2n-2). Acest offset s-a
        determinat observând că, dacă dimensiunea matricei este dim = 2^n, în matrice avem
        numere de la 1 la dim^2 = 2^2n. Deci, în cadranul 0, care este de 4 ori mai mic,
        vom avea numere de la 1 la 2^(2n-2), notat cu "off".

        | 1 -> 2^(2n-2)                      | 1 + off -> 2^(2n-2) + off          |
        ---------------------------------------------------------------------------
        | 1 + 2 * off -> 2^(2n-2)  + 2 * off | 1 + 3 * off -> 2^(2n-2)  + 3 * off |

        Cazul de bază se obține atunci când n = 0, având o matrice de dimensiune 1 x 1, în care
        valoarea din celula (1, 1) este 1.
    */
    int get_value(int n, int x, int y) {
        if (n == 0) {
            return 1;
        }

        int mid = (1 << (n - 1));

        // 0 pentru partea stângă, 1 pentru partea dreaptă
        int x_half = (x <= mid) ? 0 : 1;
        int x_quadrant_0 = x - x_half * mid;

        // 0 pentru partea de sus, 1 pentru partea de jos
        int y_half = (y <= mid) ? 0 : 1;
        int y_quadrant_0 = y - y_half * mid;

        // 0 - colțul din stânga sus
        // 1 - colțul din dreapta sus
        // 2 - colțul din stânga jos
        // 3 - colțul din dreapta jos
        int quadrant = (x_half << 1) + y_half;
        int off = (1 << (2 * n - 2));

        // valoarea corespondentului în cadranul 0 + offset
        return get_value(n - 1, x_quadrant_0, y_quadrant_0) + quadrant * off;
    }

    int get_result() { return get_value(n, x, y); }

    void print_output(int result) {
        ofstream fout("out");
        fout << result;
        fout.close();
    }
};

// [ATENTIE] NU modifica functia main!
int main() {
    // * se aloca un obiect Task pe heap
    // (se presupune ca e prea mare pentru a fi alocat pe stiva)
    // * se apeleaza metoda solve()
    // (citire, rezolvare, printare)
    // * se distruge obiectul si se elibereaza memoria
    auto* task = new (nothrow) Task(); // hint: cppreference/nothrow
    if (!task) {
        cerr << "new failed: WTF are you doing? Throw your PC!\n";
        return -1;
    }
    task->solve();
    delete task;
    return 0;
}
