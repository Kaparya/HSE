
public class OurMatrix {

    OurMatrix() {
        data = new OurNumber[0][0];
        height = 0;
        width = 0;
    }

    OurMatrix(int height, int width) {
        data = new OurNumber[height][width];
        this.height = height;
        this.width = width;

        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                data[row][column] = new OurNumber();
            }
        }
    }

    OurMatrix(int height, int width, OurNumber value) {
        data = new OurNumber[height][width];
        this.height = height;
        this.width = width;

        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                data[row][column] = value;
            }
        }
    }

    OurMatrix(int height, int width, OurNumber[][] value) {
        data = new OurNumber[height][width];
        this.height = height;
        this.width = width;

        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                data[row][column] = (value[row][column] != null ? value[row][column] : new OurNumber());
            }
        }
    }

    void ScalarMul(double value) {
        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                data[row][column].Mul(value);
            }
        }
    }

    static OurMatrix Mul(OurMatrix lhs, OurMatrix rhs) {
        if (lhs.width != rhs.height) {
            throw new ArithmeticException("Matrices' sizes aren't comparable!");
        }

        OurMatrix result = new OurMatrix(lhs.height, rhs.width);
        for (int row = 0; row < lhs.height; ++row) {
            for (int column = 0; column < rhs.width; ++column) {

                for (int index = 0; index < lhs.width; ++index) {
                    result.SetByIndex(row, column, OurNumber.Add(result.GetByIndex(row, column),
                            OurNumber.Mul(lhs.data[row][index], rhs.data[index][column])));
                }

            }
        }

        return result;
    }

    void Add(OurMatrix rhs) {
        if (rhs.width != width || rhs.height != height) {
            throw new ArithmeticException("Matrices' sizes aren't comparable!");
        }

        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                data[row][column].Add(rhs.data[row][column]);
            }
        }
    }

    static OurMatrix Add(OurMatrix lhs, OurMatrix rhs) {
        if (lhs.width != rhs.width || lhs.height != rhs.height) {
            throw new ArithmeticException("Matrices' sizes aren't comparable!");
        }

        OurMatrix result = new OurMatrix(lhs.height, lhs.width);

        for (int row = 0; row < lhs.height; ++row) {
            for (int column = 0; column < lhs.width; ++column) {
                result.SetByIndex(row, column, OurNumber.Add(lhs.data[row][column], rhs.data[row][column]));
            }
        }

        return result;
    }

    void Sub(OurMatrix rhs) {
        if (rhs.width != width || rhs.height != height) {
            throw new ArithmeticException("Matrices' sizes aren't comparable!");
        }

        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                data[row][column].Sub(rhs.data[row][column]);
            }
        }
    }

    static OurMatrix Sub(OurMatrix lhs, OurMatrix rhs) {
        if (lhs.width != rhs.width || lhs.height != rhs.height) {
            throw new ArithmeticException("Matrices' sizes aren't comparable!");
        }

        OurMatrix result = new OurMatrix(lhs.height, lhs.width);

        for (int row = 0; row < lhs.height; ++row) {
            for (int column = 0; column < lhs.width; ++column) {
                result.SetByIndex(row, column, OurNumber.Sub(lhs.data[row][column], rhs.data[row][column]));
            }
        }

        return result;
    }

    OurNumber GetByIndex(int row, int column) {
        if (row >= height || row < 0 || column >= width || column < 0) {
            throw new NullPointerException("Ex");
        }

        return data[row][column];
    }

    void SetByIndex(int row, int column, OurNumber value) {
        if (row >= height || row < 0 || column >= width || column < 0) {
            throw new NullPointerException("Ex");
        }

        data[row][column] = value;
    }

    public void Print() {
        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                data[row][column].Print();
            }
            System.out.println();
        }
        System.out.println("----------------------");
    }

    private OurMatrix FindMinor(OurMatrix matrix, int row_del, int col_del) {

        OurMatrix result = new OurMatrix(matrix.height - 1, matrix.width - 1);

        for (int row = 0; row < row_del; ++row) {
            for (int col = 0; col < col_del; ++col) {
                result.SetByIndex(row, col, matrix.GetByIndex(row, col));
            }
            for (int col = col_del + 1; col < matrix.width; ++col) {
                result.SetByIndex(row, col - 1, matrix.GetByIndex(row, col));
            }
        }
        for (int row = row_del + 1; row < matrix.height; ++row) {
            for (int col = 0; col < col_del; ++col) {
                result.SetByIndex(row - 1, col, matrix.GetByIndex(row, col));
            }
            for (int col = col_del + 1; col < matrix.width; ++col) {
                result.SetByIndex(row - 1, col - 1, matrix.GetByIndex(row, col));
            }
        }

        return result;
    }

    private OurNumber CountDeterminant(OurMatrix current) {
        if (current.height == 1) {
            return current.GetByIndex(0 , 0);
        }

        OurNumber result = new OurNumber();
        for (int row = 0; row < current.height; ++row) {
            OurMatrix help = FindMinor(current, row, 0);
            OurNumber help_det = CountDeterminant(help);
            if (row % 2 == 0) {
                result.Add(OurNumber.Mul(current.GetByIndex(row, 0), help_det));
            } else {
                result.Sub(OurNumber.Mul(current.GetByIndex(row, 0), help_det));
            }
        }

        return result;
    }

    public OurNumber Determinant() {
        if (height != width) {
            throw new ArithmeticException("Matrix is not quadratic, determinant is not defined!");
        } else if (height == 0) {
            return new OurNumber();
        }

        return CountDeterminant(this);
    }


    private OurNumber[][] data;
    private int height, width;
}
