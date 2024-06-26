import java.util.Random;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicReferenceArray;

public class CallGenerator extends Thread {
    public CallGenerator(AtomicBoolean programRun,
                         int avgCallTime,
                         int MAX_FLOOR,
                         AtomicReferenceArray<BlockingQueue<Call>> building) {
        this.programRun = programRun;
        this.avgCallTime = avgCallTime;
        this.MAX_FLOOR = MAX_FLOOR;
        this.building = building;
    }

    public void run() {

        Random rnd = new Random(28);

        while (programRun.get()) {
            // Generate new call
            int fromFloor = rnd.nextInt(MAX_FLOOR) + 1;
            int destinationFloor = rnd.nextInt(MAX_FLOOR) + 1;
            while (fromFloor == destinationFloor) {
                destinationFloor = rnd.nextInt(MAX_FLOOR) + 1;
            }
            Call newCall = new Call(fromFloor, destinationFloor);

            // Add new call to the floor
            BlockingQueue<Call> updatedFloor = building.get(fromFloor);
            updatedFloor.add(newCall);
            building.set(fromFloor, updatedFloor);

            // Sleep for randomTime ~ averageCallTime
            int sleepTime = rnd.nextInt(avgCallTime / 4) + avgCallTime;
            try {
                Thread.sleep(sleepTime);
            } catch (InterruptedException e) {
                System.out.println(e.getMessage());
            }
        }
    }

    private AtomicBoolean programRun;
    private final int avgCallTime;
    private final int MAX_FLOOR;
    AtomicReferenceArray<BlockingQueue<Call>> building;
}
