u8R"====(## EXPLANATION
A *Mathematical function requirements* error is a subset of the *Need
inputs/Lack of inputs* error specific to the usage of *Math Node*.  This occurs
when there is a *Math Node* that has no or is lacking a required number of
inputs. This warning and error occurs because most Mathematical functions
require values from inputs in order for it to calculate.
## SAMPLE CODE
Here is an *wrong* C code;
float a = math.sin(); //sine needs an input
float b = math.cos(); //cosine needs an input
float c = math.tan(); //tangent needs an input
float d = math.sqrt(); //square root needs an input
float e = math.pow(1); //power needs two inputs
float f = math.sin("hello"); //needs numerical input, not a string
***
Here is a *correct* C code:
float a = math.sin(1); //sine of 1
float b = math.cos(2); //cosing of 2
float c = math.tan(3); //tangent of 3
float d = math.sqrt(4); //square root of 4
float e = math.pow(5, 2); //5 raise to the 2nd power
***
## KEY POINTS
  * Most *Math Nodes* require input(s)
  * Most *Math Nodes* require numerical values
  * You can use other kind of *Node* as input to *Math Node* such as:
    * Variable Node
    * Math node
    * Prompt node
    * Cast node
    * Operation node
    * ArrayAccess node
    * Size node
)===="

