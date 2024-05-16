import java.util.Scanner;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicReferenceArray;

class Main {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Input how long you want to do this experiment (in seconds): ");
        int MAX_TIME = scanner.nextInt() * 1000;
        System.out.print("Input call generation average time (in seconds): ");
        int avgCallTime = scanner.nextInt();
        int MAX_FLOOR = 10;

        AtomicBoolean programRun = new AtomicBoolean(true);
        AtomicReferenceArray<BlockingQueue<Call>> building =
                new AtomicReferenceArray<>(MAX_FLOOR + 1);
        for (int index = 1; index <= MAX_FLOOR; ++index) {
            building.set(index, new ArrayBlockingQueue<>(100));
        }

        CallGenerator callGenerator = new CallGenerator(programRun, avgCallTime, MAX_FLOOR, building);
        LiftThread firstLiftThread = new LiftThread(building, programRun, new Lift(1, 1), MAX_FLOOR);
        LiftThread secondLiftThread = new LiftThread(building, programRun, new Lift(2, MAX_FLOOR), MAX_FLOOR);

        long startTime = System.currentTimeMillis();

        callGenerator.start();
        firstLiftThread.start();
        secondLiftThread.start();

        while (System.currentTimeMillis() - startTime < MAX_TIME) {
            System.out.print("\033[H\033[2J");
            System.out.flush();

            int firstLiftFloor = firstLiftThread.lift.currentFloor;
            int secondLiftFloor = secondLiftThread.lift.currentFloor;
            System.out.println("/---\\");

            for (int floor = MAX_FLOOR; floor >= 1; --floor) {
                if (floor == firstLiftFloor && floor == secondLiftFloor) {
                    System.out.print("|#|#|");
                } else if (floor == firstLiftFloor) {
                    System.out.print("|#| |");
                } else if (floor == secondLiftFloor) {
                    System.out.print("| |#|");
                } else {
                    System.out.print("| | |");
                }
                BlockingQueue<Call> people = building.get(floor);
                for (Call current : people) {
                    System.out.print(" (" + current.fromFloor + "->" + current.destinationFloor + ")");
                }
                System.out.println();
            }

            System.out.println("\\---/");
            System.out.println("\nFirst lift:");
            for (Call call : firstLiftThread.lift.people) {
                System.out.print("(" + call.fromFloor + "->" + call.destinationFloor + ") ");
            }
            System.out.println("\n\nSecond lift:");
            for (Call call : secondLiftThread.lift.people) {
                System.out.print("(" + call.fromFloor + "->" + call.destinationFloor + ") ");
            }
            System.out.flush();


            try {
                Thread.sleep(150);
            } catch (InterruptedException exception) {
                System.out.println(exception.getMessage());
                System.exit(-1);
            }
        }

        programRun.set(false);
        System.out.println("Experiment ended!");
        try {
            callGenerator.join();
            firstLiftThread.join();
            secondLiftThread.join();
        } catch (InterruptedException exception) {
            System.out.println("EXCEPTION: " + exception.getMessage());
        }
        scanner.close();
    }
}