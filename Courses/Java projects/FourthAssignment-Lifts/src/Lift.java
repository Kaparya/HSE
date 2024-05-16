import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

public class Lift {
    public Lift(int number, int currentFloor) {
        this.number = number;
        this.currentFloor = currentFloor;
        this.destinationFloor = currentFloor;
        this.people = new ArrayBlockingQueue<>(100);
    }

    int number;
    int currentFloor;
    BlockingQueue<Call> people;
    int destinationFloor;
}
