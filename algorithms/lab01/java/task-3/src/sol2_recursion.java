// SPDX-License-Identifier: BSD-3-Clause

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

public class sol2_recursion {
    static class Task {
        public static final String INPUT_FILE = "in";
        public static final String OUTPUT_FILE = "out";

        int n, x, y;

        public void solve() {
            readInput();
            writeOutput(getValue(n, x, y));
        }

        private void readInput() {
            try {
                Scanner sc = new Scanner(new File(INPUT_FILE));
                n = sc.nextInt();
                x = sc.nextInt();
                y = sc.nextInt();
                sc.close();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }

        private void writeOutput(int result) {
            try {
                PrintWriter pw = new PrintWriter(new File(OUTPUT_FILE));
                pw.print(result);
                pw.close();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }

        /*
         *      0  1
         *  0 |00|01|                | 0| 1|
         *   -------- => cadrane:    --------
         *  1 |10|11|                | 2| 3|
         *
         * Pentru a determina o valoare din celula (x, y) aflată in cadranele 1, 2 sau 3 este
         * suficient să translatăm celula respectivă în cadranul 0, recursiv, scăzând un offset.
         *
         * Cadranul 0: (x, y)
         * Cadranul 1: (x, y - mid)
         * Cadranul 2: (x - mid, y)
         * Cadranul 3: (x - mid, y - mid)
         *
         * off = 2^(2n-2). value = get_value(n-1, x_quadrant_0, y_quadrant_0) + quadrant * off.
         * Cazul de bază: n == 0 => return 1.
         */
        private int getValue(int n, int x, int y) {
            if (n == 0) {
                return 1;
            }

            int mid = (1 << (n - 1));

            // 0 pentru partea stângă, 1 pentru partea dreaptă
            int xHalf = (x <= mid) ? 0 : 1;
            int xQuadrant0 = x - xHalf * mid;

            // 0 pentru partea de sus, 1 pentru partea de jos
            int yHalf = (y <= mid) ? 0 : 1;
            int yQuadrant0 = y - yHalf * mid;

            // 0 - colțul din stânga sus, 1 - dreapta sus, 2 - stânga jos, 3 - dreapta jos
            int quadrant = (xHalf << 1) + yHalf;
            int off = (1 << (2 * n - 2));

            return getValue(n - 1, xQuadrant0, yQuadrant0) + quadrant * off;
        }
    }

    public static void main(String[] args) {
        new Task().solve();
    }
}
