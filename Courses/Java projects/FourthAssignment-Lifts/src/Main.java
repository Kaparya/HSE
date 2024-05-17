import javax.swing.*;
import java.awt.*;
import java.util.Scanner;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicReferenceArray;

class Main {
    private static void printToConsole(
            LiftThread firstLiftThread,
            LiftThread secondLiftThread,
            LiftThread thirdLiftThread,
            AtomicReferenceArray<BlockingQueue<Call>> building,
            int MAX_FLOOR) {
        System.out.print("\033[H\033[2J");
        System.out.flush();

        int firstLiftFloor = firstLiftThread.lift.currentFloor;
        int secondLiftFloor = secondLiftThread.lift.currentFloor;
        int thirdLiftFloor = thirdLiftThread.lift.currentFloor;
        System.out.println("  /-------\\");

        for (int floor = MAX_FLOOR; floor >= 1; --floor) {
            if ((floor - 1) % 3 == 0) {
                System.out.printf("%2d", floor);
            } else {
                System.out.print("  ");
            }
            if (floor == firstLiftFloor) {
                System.out.print("|#|");
            } else {
                System.out.print("| |");
            }
            if (floor == secondLiftFloor) {
                System.out.print("|#|");
            } else {
                System.out.print("| |");
            }
            if (floor == thirdLiftFloor) {
                System.out.print("|#|");
            } else {
                System.out.print("| |");
            }


            for (Call current : building.get(floor)) {
                System.out.print(" (" + current.fromFloor + "->" + current.destinationFloor + ")");
            }
            System.out.println();
        }

        System.out.println("  \\-------/");
        System.out.print("\nFirst lift:");
        if (firstLiftThread.goingToNewFloor) {
            System.out.print(firstLiftThread.lift.destinationFloor);
        }
        System.out.println();
        for (Call call : firstLiftThread.lift.people) {
            System.out.print("(" + call.fromFloor + "->" + call.destinationFloor + ") ");
        }

        System.out.print("\n\nSecond lift:");
        if (secondLiftThread.goingToNewFloor) {
            System.out.print(secondLiftThread.lift.destinationFloor);
        }
        System.out.println();
        for (Call call : secondLiftThread.lift.people) {
            System.out.print("(" + call.fromFloor + "->" + call.destinationFloor + ") ");
        }

        System.out.print("\n\nThird lift:");
        if (thirdLiftThread.goingToNewFloor) {
            System.out.print(thirdLiftThread.lift.destinationFloor);
        }
        System.out.println();
        for (Call call : thirdLiftThread.lift.people) {
            System.out.print("(" + call.fromFloor + "->" + call.destinationFloor + ") ");
        }
        System.out.flush();
    }

    public static void main(String[] args) {

        int MAX_FLOOR = 10;

        Scanner scanner = new Scanner(System.in);
        System.out.print("Input how long you want to do this experiment (in seconds): ");
        int MAX_TIME = scanner.nextInt() * 1000;
        System.out.print("Input call generation average time (in milliseconds): ");
        int avgCallTime = scanner.nextInt();
        scanner.close();

        AtomicBoolean programRun = new AtomicBoolean(true);
        AtomicReferenceArray<BlockingQueue<Call>> building =
                new AtomicReferenceArray<>(MAX_FLOOR + 1);
        for (int index = 1; index <= MAX_FLOOR; ++index) {
            building.set(index, new ArrayBlockingQueue<>(100));
        }

        CallGenerator callGenerator = new CallGenerator(programRun, avgCallTime, MAX_FLOOR, building);
        LiftThread firstLiftThread = new LiftThread(building, programRun, new Lift(1, 1), MAX_FLOOR);
        LiftThread secondLiftThread = new LiftThread(building, programRun, new Lift(2, MAX_FLOOR), MAX_FLOOR);
        LiftThread thirdLiftThread = new LiftThread(building, programRun, new Lift(2, MAX_FLOOR / 2), MAX_FLOOR);


        JFrame window = new JFrame();
        window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        window.setResizable(false);
        window.setTitle("Async Elevators");

        LiftsPanel liftsPanel = new LiftsPanel(MAX_FLOOR);
        window.add(liftsPanel);
        window.pack();
        window.setLocationRelativeTo(null);
        window.setVisible(true);
        int[] peopleOnFloor = new int[MAX_FLOOR + 1];
        int[] peopleInLift = new int[3];
        int[] peopleOut = new int[3];
        liftsPanel.repaint(
                firstLiftThread.lift.currentFloor,
                secondLiftThread.lift.currentFloor,
                thirdLiftThread.lift.currentFloor,
                peopleOnFloor,
                peopleInLift,
                peopleOut
        );

        long startTime = System.currentTimeMillis();

        callGenerator.start();
        firstLiftThread.start();
        secondLiftThread.start();
        thirdLiftThread.start();

        while (System.currentTimeMillis() - startTime < MAX_TIME) {

            printToConsole(
                    firstLiftThread,
                    secondLiftThread,
                    thirdLiftThread,
                    building,
                    MAX_FLOOR
            );

            for (int floor = 1; floor <= MAX_FLOOR; ++floor) {
                peopleOnFloor[floor] = building.get(floor).size();
            }

            peopleOut = new int[]{
                    peopleInLift[0] - firstLiftThread.lift.people.size(),
                    peopleInLift[1] - secondLiftThread.lift.people.size(),
                    peopleInLift[2] - thirdLiftThread.lift.people.size()
            };
            peopleInLift = new int[]{
                    firstLiftThread.lift.people.size(),
                    secondLiftThread.lift.people.size(),
                    thirdLiftThread.lift.people.size()
            };

            liftsPanel.repaint(
                    firstLiftThread.lift.currentFloor,
                    secondLiftThread.lift.currentFloor,
                    thirdLiftThread.lift.currentFloor,
                    peopleOnFloor,
                    peopleInLift,
                    peopleOut
            );

            try {
                Thread.sleep(499);
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
    }
}