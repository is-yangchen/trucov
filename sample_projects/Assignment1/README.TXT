William Reinhardt
Cpt S 422
Assignment: Homework #1

INSTRUCTIONS
============
Compile the program using either the Makefile or an IDE and run the executable. Use the test driver menu to either test one or more operations' test cases, or alternatively run all test cases sequentially.

Observe the output from the print functions, when applicable to a particular operation.

COVERAGE OUTPUT
===============
Coverage information for each function is included in the form of the attached PDFs. The PDFs were generated after all test cases were ran. Green blocks and arcs indicate coverage, while red arcs and blocks indicate that the block or arc was never traversed. Yellow diamonds indicate a branch which did not have all of its outgoing paths taken.

ERRORS FOUND
============
Test cases 2.4-2.7 would result in segfaults.
The severity of this defect was high.
The location of the defect was anywhere within the remove() function where the delete operator was used.
The defect was corrected by setting the pointer to a value of NULL after the delete operator.
