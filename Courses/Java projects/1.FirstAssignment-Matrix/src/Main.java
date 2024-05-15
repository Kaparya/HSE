
public class Main {

    static double Epsilon = 1e-9;

    static void TestNumbers() {
        System.out.println("NUMBERS TEST\n\n");

        OurNumber first = new OurNumber(0, 2);
        System.out.print("x = ");
        first.Print();
        OurNumber second = new OurNumber(-1, 3);
        System.out.print("\ny = ");
        second.Print();
        OurNumber third = new OurNumber(1);
        System.out.print("\nz = ");
        third.Print();

        OurNumber mul_test = OurNumber.Mul(OurNumber.Mul(first, second), third);
        System.out.print("\n\nx * y * z = ");
        mul_test.Print();
        System.out.print("\nxyz * y = ");
        mul_test.Mul(second);
        mul_test.Print();
        System.out.print("\n|xyz * y| = ");
        System.out.println(mul_test.Module());

        System.out.print("\n\nx + y + z = ");
        OurNumber sum_test = OurNumber.Add(OurNumber.Add(first, second), third);
        sum_test.Print();
        System.out.print("\nx + y + z + y = ");
        sum_test.Add(second);
        sum_test.Print();
        System.out.print("\n|x + y + z + y| = ");
        System.out.println(sum_test.Module());

        System.out.print("\n\nx - y - z = ");
        OurNumber sub_test = OurNumber.Sub(OurNumber.Sub(first, second), third);
        sub_test.Print();
        System.out.print("\nx - y - z - y = ");
        sub_test.Sub(second);
        sub_test.Print();
        System.out.print("\n|x - y - z - y| = ");
        System.out.println(sub_test.Module());

        System.out.println("\n\nChange real or imaginary part:");
        sub_test.Print();
        sub_test.SetReal(10);
        sub_test.SetReal(0);
        System.out.println("\nRe = 0, Im = 10:");
        sub_test.SetImaginary(0);
        sub_test.SetImaginary(10);
        sub_test.Print();
    }

    static void TestMatrix() {
        System.out.println("MATRIX TEST\n\n");

        System.out.println("Empty matrix");
        OurMatrix matrix = new OurMatrix();
        matrix.Print();
        System.out.println("Zero matrix 10 x 10");
        matrix = new OurMatrix(10, 10);
        matrix.Print();

        System.out.println("Ordinary matrix 1 x 2 with same numbers");
        matrix = new OurMatrix(2, 2, new OurNumber(1, 2));
        matrix.Print();

        System.out.println("Ordinary matrix 2 x 2");
        OurNumber[][] help = new OurNumber[2][2];
        help[0][0] = new OurNumber(1, 2);
        help[0][1] = new OurNumber(3, 4);
        help[1][0] = new OurNumber(5, 6);
        help[1][1] = new OurNumber();
        matrix = new OurMatrix(2, 2, help);
        matrix.Print();

        System.out.println("Change elements [1][1] and [0][0]");
        matrix.SetByIndex(1, 1, new OurNumber(7, 8));
        matrix.SetByIndex(0, 0, new OurNumber());
        matrix.Print();
        System.out.print("Element [0][0] = ");
        matrix.GetByIndex(0, 0).Print();
        System.out.print("\nElement [1][1] = ");
        matrix.GetByIndex(1, 1).Print();
    }

    static void TestMatrixAddSub() {
        System.out.println("MATRIX ADDITION AND SUBTRACTION TEST\n\n");

        System.out.println("Addition");
        OurNumber[][] help = new OurNumber[3][3];
        for (int row = 0; row < 3; ++row) {
            for (int column = 0; column < 3; ++column) {
                help[row][column] = new OurNumber(row, column);
            }
        }
        OurMatrix lhs = new OurMatrix(3, 3, help);
        System.out.println("Left matrix:");
        lhs.Print();

        help = new OurNumber[3][3];
        for (int row = 0; row < 3; ++row) {
            for (int column = 0; column < 3; ++column) {
                help[row][column] = new OurNumber(2 - row, 2 - column);
            }
        }
        OurMatrix rhs = new OurMatrix(3, 3, help);
        System.out.println("Right matrix:");
        rhs.Print();

        OurMatrix result = OurMatrix.Add(lhs, rhs);
        System.out.println("Result matrix:");
        result.Print();
        System.out.println("\n");

        // ----------------------
        System.out.println("Addition");
        System.out.println("Left matrix:");
        result.Print();
        System.out.println("Right matrix:");
        lhs.Print();
        result.Add(lhs);
        System.out.println("Result matrix:");
        result.Print();
        System.out.println("\n");

        // ----------------------
        System.out.println("Subtraction");
        System.out.println("Left matrix:");
        result.Print();
        System.out.println("Right matrix:");
        rhs.Print();
        result.Sub(rhs);
        result.Print();
        System.out.println("\n");

        // ----------------------
        System.out.println("Subtraction twice");
        System.out.println("Left matrix:");
        result.Print();
        System.out.println("Right matrix:");
        lhs.Print();
        result = OurMatrix.Sub(result, lhs);
        result.Sub(lhs);
        System.out.println("Result matrix:");
        result.Print();
    }

    static void TestMatrixMul() {
        System.out.println("MATRIX MULTIPLICATION TEST\n\n");

        int height = 2, width = 3;
        OurNumber[][] help = new OurNumber[height][width];
        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                help[row][column] = new OurNumber(row + column, column);
            }
        }
        OurMatrix lhs = new OurMatrix(height, width, help);
        System.out.println("Left matrix:");
        lhs.Print();

        help = new OurNumber[width][height];
        for (int row = 0; row < width; ++row) {
            for (int column = 0; column < height; ++column) {
                help[row][column] = new OurNumber((height + width - 2) - row - column, -row);
            }
        }
        OurMatrix rhs = new OurMatrix(width, height, help);
        System.out.println("Right matrix:");
        rhs.Print();

        OurMatrix result = OurMatrix.Mul(lhs, rhs);
        System.out.println("Result matrix:");
        result.Print();
        System.out.println("\n");

        // ----------------------
        help = new OurNumber[width + 1][height];
        for (int row = 0; row < width + 1; ++row) {
            for (int column = 0; column < height; ++column) {
                help[row][column] = new OurNumber((height + width - 2) - row - column, -row);
            }
        }
        rhs = new OurMatrix(width + 1, height, help);
        System.out.println("Left matrix:");
        lhs.Print();
        System.out.println("Right matrix:");
        rhs.Print();

        result = OurMatrix.Mul(lhs, rhs);
        System.out.println("Result matrix:");
        result.Print();
        System.out.println("\n");
    }

    static void TestDeterminant() {
        System.out.println("DETERMINANT TEST\n\n");

        int height = 2, width = 2;
        OurNumber[][] help = new OurNumber[height][width];
        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                help[row][column] = new OurNumber(row + column, column);
            }
        }
        OurMatrix mat = new OurMatrix(height, width, help);
        System.out.println("Current matrix:");
        mat.Print();

        OurNumber det = mat.Determinant();
        System.out.print("Determinant = ");
        det.Print();
        System.out.println("\n");

        // ----------------------
        height = 5;
        width = 5;
        help = new OurNumber[height][width];
        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                help[row][column] = new OurNumber(height - row + 2, width - column + 2);
            }
        }
        help[1][3].SetReal(64);
        help[3][2].SetReal(41);
        help[3][3].SetReal(34);
        help[3][4].SetReal(54);
        help[4][3].SetReal(39);
        help[4][4].SetImaginary(13);
        mat = new OurMatrix(height, width, help);
        System.out.println("Current matrix:");
        mat.Print();

        det = mat.Determinant();
        System.out.print("Determinant = ");
        det.Print();
        System.out.println("\n");

        // ----------------------
        height = 1;
        width = 4;
        help = new OurNumber[height][width];
        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                help[row][column] = new OurNumber(row + column, column);
            }
        }
        mat = new OurMatrix(height, width, help);
        System.out.println("Current matrix:");
        mat.Print();

        det = mat.Determinant();
        det.Print();
    }

    public static void main(String[] args) {

        // TestNumbers();
        // TestMatrix();
        // TestMatrixAddSub();
        // TestMatrixMul();
        // TestDeterminant();
    }
}