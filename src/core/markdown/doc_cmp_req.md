u8R"====(## EXPLANATION
*Comparison requirements* error is a subset of *Need Inputs/Lack of Inputs*
error.  This is specific to using *Comparison Node* but without providing any
inputs, or lack of inputs at all. Comparison, by its very definition, needs at
least two values to compare in order to give a result whether the comparison
operator results in *true* or *false*.
## SAMPLE CODE
Here is an *wrong* C code;
int a = 100;

bool is_same = (a == ???); //we don't know what to compare 'a' with
***
Here is a *correct* C code:
int a = 100;
int b = 101;

bool is_same = (a == b); //results in 'false'
***
## KEY POINTS
  * Comparison needs *at least* 2 inputs to compare
  * All comparison results in either *true* or *false*
)===="
