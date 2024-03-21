import org.jetbrains.annotations.NotNull;

public class OurNumber {
    private double re = 0;
    private double im = 0;

    OurNumber() {
    }

    // Real number
    OurNumber(double Re) {
        re = Re;
    }

    // Complex number
    OurNumber(double Re, double Im) {
        re = Re;
        im = Im;
    }

    OurNumber(@NotNull OurNumber rhs) {
        re = rhs.re;
        im = rhs.im;
    }

    void Add(OurNumber rhs) {
        re += rhs.re;
        im += rhs.im;
    }

    static OurNumber Add(@NotNull OurNumber lhs, @NotNull OurNumber rhs) {
        return new OurNumber(lhs.re + rhs.re, lhs.im + rhs.im);
    }

    void Sub(OurNumber rhs) {
        re -= rhs.re;
        im -= rhs.im;
    }

    static OurNumber Sub(@NotNull OurNumber lhs, @NotNull OurNumber rhs) {
        return new OurNumber(lhs.re - rhs.re, lhs.im - rhs.im);
    }

    void Mul(OurNumber rhs) {
        double tmp = re * rhs.re - im * rhs.im;
        im = re * rhs.im + im * rhs.re;
        re = tmp;
    }
    static OurNumber Mul(@NotNull OurNumber lhs, @NotNull OurNumber rhs) {
        return new OurNumber(lhs.re * rhs.re - lhs.im * rhs.im, lhs.re * rhs.im + lhs.im * rhs.re);
    }

    void Mul(double rhs) {
        re *= rhs;
        im *= rhs;
    }

    double Module() {
        return Math.sqrt(re * re + im * im);
    }

    void Print() {
        if (Math.abs(im) < Main.Epsilon) {
            System.out.print(re + " ");
            return;
        } else if (Math.abs(re) < Main.Epsilon) {
            System.out.print(im + "i ");
            return;
        }
        if (im < 0) {
            System.out.print(re + " - " + Math.abs(im) + "i ");
            return;
        }
        System.out.print(re + " + " + im + "i ");
    }

    double GetReal() {
        return re;
    }

    void SetReal(double re) {
        this.re = re;
    }

    double GetImaginary() {
        return im;
    }

    void SetImaginary(double im) {
        this.im = im;
    }
}
