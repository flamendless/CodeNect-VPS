u8R"====(## EXPLANATION
*Need inputs or lack of inputs* is a special error implemented by CodeNect. This occurs whenever user declares a *Node* that requires a number of inputs but were not provided one. The inputs are necessary in order to transpile the node into actual code. Instead of transpiling the code and then the compiler will throw a *syntax error*, it is better to catch it during transpilation stage to narrow down the potential issue.
## SAMPLE CODE
Here is a *pseudo-code* example:
//let's say that user has NodeOperation declared WITH inputs,
//the result will be:
int a = 1;
int b = 2;
int result = a + b;

//WITHOUT inputs it will be:
int a = 1;
int b = 2;
int result = +; //syntax error because CodeNect does not know what to put there because there is no given input
***
## KEY POINTS
  * Check carefully the warnings in the *Terminal* to catch these issues before transpiling
  * Make sure you provide enough data or input to *Nodes* that need it
)===="
