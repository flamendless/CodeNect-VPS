u8R"====(## EXPLANATION
A *For Loop* is a programming statement that all programming languages have.
A *For Loop* statement is executed multiple times, each execution is called
*iteration*, depending on its constructs. A *For Loop* has the following parts:
  * *iterator* - this is set to be starting value
  * *conditon* - this is checked after each iteration to determine if the loop
  	should continue
  * *increment* - this is executed after the *conditon* resulsts to *true* and
  	before the next *iteration* happens
## SAMPLE CODE
Here is a sample C code:
for (int i = 0; i < 10; i++)
{
  printf("%d", i); //run this every iteration
}
//this will print: 0123456789
***
Here is the breakdown of the syntax:
  * *for* is the syntax to mean we are declaring a for-loop statement
  * *int i = 0* is the *iterator* where the *iterator variable* is *i* which is
initialized to *0*
  * *i < 10* is the conditon. This is checked when we reach the end of the for-loops' code block
  * *i++* is the *increment*. If the *condition* is true, it will be executed.
## KEY POINTS
  * *For Loops* are powerful, they simplify reptitive tasks
  * Each *part* of a *For Loop* can be omitted.
  * A *For Loop* can be used as an alternative to a *While Loop*
  * A *For Loop* is safer to use than a *While Loop* because it can prevent *infinite loop*
  * A *For Loop* is a *controlled loop*
)===="
