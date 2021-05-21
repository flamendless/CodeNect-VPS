u8R"====(## EXPLANATION
*Operation requirements* error is a subset of *Need inputs/Lack of inputs* error specific to the usage of *Operation Node*. This occurs when there is not enough input for operations required. Operations need at least 2 inputs in order for it to operate properly. In CodeNect, it is required that inputs are required to be of the *data type*, that is, it is not allowed to add an `int` with a `string`. This is important because operating on values of different data type leads into buggy code and imprecise results.
## SAMPLE CODE
Here is an *wrong* C code;
int a = 100;
int sum = a + ???; //we do not know what to add with 'a' because we got no other input
int product = ??? * ???; //this one does not have any input

int b = 200;
float c = 300.32;
int = b + c; //warning here because the float c will be casted to be an int
float b + c; //warning here because the int b will be casted to be a float
***
Here is a *correct* C code:
int a = 100;
int b = 101;
int c = 102;
int sum = a + b + c;
int product = a * b * c;
***
## KEY POINTS
  * An operation needs *at least* 2 inputs to operate on
  * Inputs are required to be of the *same data type*
  * All operation results in the same data type as the input
)===="
