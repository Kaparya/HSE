import java.util.concurrent.BlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicReferenceArray;

import static java.lang.Math.*;

public class LiftThread extends Thread {
    public LiftThread(AtomicReferenceArray<BlockingQueue<Call>> building,
                      AtomicBoolean programRun,
                      Lift lift,
                      int MAX_FLOOR) {
        this.building = building;
        this.programRun = programRun;
        this.lift = lift;
        this.MAX_FLOOR = MAX_FLOOR;
    }

    public void run() {
        while (programRun.get()) {

            moveLift();

            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                System.out.println(e.getMessage());
                System.exit(-3);
            }
        }
    }

    private void letPeopleOutOnCurrentFloor() {
        BlockingQueue<Call> people = lift.people;

        for (Call call : lift.people) {
            if (call.destinationFloor == lift.currentFloor) {
                try {
                    people.remove(call);
                } catch (Exception exception) {
                    System.out.println(exception.getMessage());
                    System.exit(-7);
                }
            }
        }

        lift.people = people;
    }

    private void takeNewPeopleOnCurrentFloor() {
        BlockingQueue<Call> current = building.get(lift.currentFloor);
        if (current.isEmpty()) {
            return;
        }

        boolean up;
        if (lift.people.isEmpty()) {
            Call inside = null;
            try {
                inside = current.take();
            } catch (InterruptedException e) {
                System.out.println(e.getMessage());
                System.exit(-4);
            }

            lift.people.add(inside);
            lift.destinationFloor = inside.destinationFloor;
            up = inside.up;
        } else {
            up = lift.people.peek().up;
        }

        for (Call call : current) {
            if (call.up == up) {
                lift.people.add(call);
                if (up) {
                    lift.destinationFloor = max(lift.destinationFloor, call.destinationFloor);
                } else {
                    lift.destinationFloor = min(lift.destinationFloor, call.destinationFloor);
                }
            }
        }

        for (Call call : lift.people) {
            try {
                current.remove(call);
            } catch (Exception exception) {
                System.out.println(exception.getMessage());
                System.exit(-7);
            }
        }
        building.set(lift.currentFloor, current);
    }

    // To find the next floor (if the lift is empty), we count coefficients, than take the maximum one
    int[] countCoefficients() {
        int[] result = new int[MAX_FLOOR + 1];

        for (int floor = 1; floor <= MAX_FLOOR; ++floor) {
            BlockingQueue<Call> currentPeople = building.get(floor);
            if (floor == lift.currentFloor || currentPeople.isEmpty() || currentPeople.peek().taken) {
                result[floor] = -1;
                continue;
            }
            result[floor] = MAX_FLOOR - abs(lift.currentFloor - floor);
            result[floor] += currentPeople.size() * 4;
        }

        return result;
    }

    public synchronized void findNewDestination() {
        int[] coefficients = countCoefficients();

        int maxCoefficient = -1;
        int maxFloor = lift.currentFloor;
        for (int floor = 1; floor <= MAX_FLOOR; ++floor) {
            if (coefficients[floor] > maxCoefficient) {
                maxCoefficient = coefficients[floor];
                maxFloor = floor;
            }
        }
        lift.destinationFloor = maxFloor;
        if (maxCoefficient == -1) {
            return;
        }
        BlockingQueue<Call> tmp = building.get(maxFloor);
        if (!tmp.isEmpty()) {
            tmp.peek().taken = true;
        }
        building.set(maxFloor, tmp);
    }

    private void moveLift() {

        if (goingToNewFloor) {
            if (lift.currentFloor == lift.destinationFloor) {
                goingToNewFloor = false;
            } else {
                BlockingQueue<Call> destinationFloor = building.get(lift.destinationFloor);
                if (destinationFloor.isEmpty() || !destinationFloor.peek().taken) {
                    goingToNewFloor = false;
                }
            }
        }

        if (!goingToNewFloor) {
            letPeopleOutOnCurrentFloor();
            takeNewPeopleOnCurrentFloor();
        }

        if (lift.destinationFloor < lift.currentFloor) {
            --lift.currentFloor;
            return;
        }
        if (lift.destinationFloor > lift.currentFloor) {
            ++lift.currentFloor;
            return;
        }

        goingToNewFloor = true;
        findNewDestination();
    }

    public boolean goingToNewFloor = false;
    private AtomicReferenceArray<BlockingQueue<Call>> building;
    private AtomicBoolean programRun;
    public Lift lift;
    private final int MAX_FLOOR;
}
