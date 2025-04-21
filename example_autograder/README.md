# GradeScope PA3 
[Gradescope Documentation](https://gradescope-autograders.readthedocs.io/en/latest/)  
[Template code on Github](https://github.com/gradescope/autograder_samples/tree/master/java)

## Example Desciption

[Sleeping Barber](https://en.wikipedia.org/wiki/Sleeping_barber_problem) Implementation.  
Input files are `BarberShop.java`, `Customer.java`, and `PriorityCustomerQueue.java`.

## Explanation of Dependencies

- JUnit: Popular Java unit testing framework
- jh61b: Among other things, this provides annotations for JUnit tests that allows setting point values and names, and a test listener that produces output in JSON format
- JDK11: jh61b uses String.join() which is added in JDK8. If you need versions <8, you just need to replace this part of the code

# Files

## setup.sh

Sets up OpenJDK 11.  

Note: Installing JDK11 takes a few minutes, so building the image takes
a while. We may later provide base images to speed this up.

## run_autograder

Copies the student's code to the autograder directory, compiles, and
executes it.

## export.sh

Zips important parts of autogarder into Archive.zip to be uploaded to gradescope as autograder

## src/main/java/com/gradescope/solution/tests/RunTests.java

Runs `src/main/java/com/gradescope/solution/tests/Tests.java`

## src/main/java/com/gradescope/solution/tests/Tests.java

Testcases. 

## src/main/java/com/gradescope/solution/Number.java
Doesn't actually need to be here