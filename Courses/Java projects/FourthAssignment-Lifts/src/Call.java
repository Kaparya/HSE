
public class Call {
    public Call(int fromFloor, int destinationFloor) {
        this.fromFloor = fromFloor;
        this.destinationFloor = destinationFloor;

        up = fromFloor < destinationFloor;
    }

    public int fromFloor;
    public int destinationFloor;
    public boolean up;
    public boolean taken = false;
}
