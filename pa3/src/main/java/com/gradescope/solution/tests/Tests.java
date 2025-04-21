package com.gradescope.solution.tests;

import org.junit.Test;
import static org.junit.Assert.*;
import com.gradescope.jh61b.grader.GradedTest;

import com.gradescope.solution.BarberShop;
import java.util.List;
import java.util.concurrent.*;

/*
* This isn't the full testcase, nice try :D
*/
public class Tests {
    @Test
    @GradedTest(name="Test solution matches provided output", max_score=1)
    public void testStudentTestcaseLoose() throws InterruptedException {
        final int CHAIRS = 3;
        final int CUSTOMERS = 10;
        final int BARBERS = 2;

        BarberShop shop = new BarberShop(CHAIRS, BARBERS);

        // Start barbers
        ExecutorService barberExecutor = Executors.newFixedThreadPool(BARBERS);
        for (int i = 0; i < BARBERS; i++) {
            final int id = i;
            barberExecutor.submit(() -> shop.barberWork(id));
        }

        // Customers arrive
        ExecutorService customerExecutor = Executors.newFixedThreadPool(CUSTOMERS);
        for (int i = 0; i < CUSTOMERS; i++) {
            final int id = i;
            customerExecutor.submit(() -> shop.customerArrives(id, id%3));
            Thread.sleep(200);
        }

        customerExecutor.shutdown();
        customerExecutor.awaitTermination(15, TimeUnit.SECONDS);

        Thread.sleep(3000);
        barberExecutor.shutdownNow();
        barberExecutor.awaitTermination(5, TimeUnit.SECONDS);

        // Print stats
        shop.printStats();

        assertTrue("Some customers should be served.", shop.getCustomersServed() > 0);
        assertTrue("Rejected customers count should be >= 0.", shop.getCustomersRejected() >= 0);
        assertTrue("Barbers should have slept at least once.", shop.getTotalSleepTimeNanos() > 0);
    }

    // @Test
    // @GradedTest(name="Test appending to a list", max_score=1)
    // public void test_append() {
    //     AbstractIntList test = new IntList(RefIntList.createList(1, 2, 4, 8, 16));
    //     test.append(32);
    //     assertEquals(test, RefIntList.createList(1, 2, 4, 8, 16, 32));
    // }

    // @Test
    // @GradedTest(name="Test converting a list to a string", max_score=1)
    // public void test_to_string() {
    //     AbstractIntList test = new IntList(RefIntList.createList(1, 2, 4, 8, 16));
    //     assertEquals(test.toString(), "1 2 4 8 16\n");
    // }
}
