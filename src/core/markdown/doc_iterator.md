u8R"====(## EXPLANATION
An *iterator* is a *variable* that is used to keep track of *iteration* states.
This is commonly used along with *for-loops* and is usually of type *integer*.
In CodeNect, the special *iterator output integer* slot of *For Node* can only
be connected once with a *Variable Node*;
## SAMPLE CODE
Here is a *wrong* C code example:
for (int i = 0; i < 10; i++)
{
  //the 'i' is the iterator.
  //the 'i' is local to this for-loops' scope
  printf("Iteration: %d/%d", i, 10);
}

printf("%d", i); //ERROR, 'i' is only accessible in the code block of the for-loop
***
Here is a *correct* C code example:
//get the sum of 1 to 10
int sum = 0;
for (int i = 1; i <= 10; i++)
{
  //this will be 1 + 2 + 3 + ... + 9 + 10
  sum = sum + i;
}
***
## KEY POINTS
  * The *Variable Node* can only be used for connecting with *iterator*
  * *Iterator* is commonly used with *for-loop* statements
)===="
