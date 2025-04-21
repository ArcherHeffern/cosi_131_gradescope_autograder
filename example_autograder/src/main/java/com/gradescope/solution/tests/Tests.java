package com.gradescope.solution.tests;

import org.junit.Test;
import static org.junit.Assert.*;
import com.gradescope.jh61b.grader.GradedTest;

import com.gradescope.solution.Number;

public class Tests {
    @Test
    @GradedTest(name="Test solution matches provided output", max_score=1)
    public void testStudentTestcaseLoose() throws InterruptedException {
        assertTrue("Number.NUMBER equals 5", Number.NUMBER == 5)
    }
}
