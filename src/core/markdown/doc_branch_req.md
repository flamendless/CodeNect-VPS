u8R"====(## EXPLANATION
*Branch requirement error* is a subset of *Need inputs/Lack of inputs* error
specific to the usage of *Branch Node*. This occurs when there is no node
connected to the output of *TRUE*. This is because in *Branch Node*, the *TRUE*
output is equivalent to the *if-statement* while the *FALSE* output is
equivalent to the *else-statement*
## SAMPLE CODE
Here is an *wrong* C code;
int a = 0;
int b = 0;

else //this is the FALSE output
{
  int d = 0;
}
***
Here is a *correct* C code:
int a = 0;
int b = 0;

if (a == b) //this is the TRUE output
{
  int c = 0;
}
else
{
  int d = 0;
}
***
## KEY POINTS
  * A branch needs at least the *TRUE* output connection
  * *TRUE* is equivalent to the *if-statement*
  * *FALSE* is equivalent to the *else-statement*
)===="
