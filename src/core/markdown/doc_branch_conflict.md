u8R"====(# BRANCH CONFLICT
## EXPLANATION
Think of NodeBranch as a *code block*. A code block is a block of code that
has a separate stack in memory. Code blocks in C/C++, Java, and other programming
languages are denoted by the combination of *curly braces {}*. All code that is inside
the curly braces has its own separate variables and execution. Code blocks can
be nested indefinitely. A code block can access variables outside of it as
long as it its not in another separate code block.  This is called *scopes*.
A files has its own scope and a function has its own scope as well.

Branch conflict happens when a variable that is in another code block tries to
access or refer to a variable that is in another code block. For example, if we
declare a variable inside a code block for the 'if-statement', we can not use
that variable in the 'else-statement' because the code block inside 'else-statement'
has no idea or memory of what is declared the 'if-statement'.
## SAMPLE CODE
Here is a C code example:
```
int a = 0; //since 'a' is in the parent scope, both the if and else statements can access this
if (true)
{
	//this is a code block
	int b = 1; //
	printf("value of a: %d\n", a); //we can access 'a'
	printf("value of b: %d\n", b); //we can access 'b' because it is in the same scope
}
else
{
	//this is another code block
	printf("value of a: %d\n", a); //we can access 'a'
	printf("value of b: %d\n", b); //ERROR! this code block does not know the 'b' variable
}
```
## ILLUSTRATION:
Think of a code block as a house. A house has many rooms (code block inside
another code block). Each room can have different utens (variables). The scope
of a house is everything inside of it. The rooms inside a house has access to
the items in another room. But, a house can not access the rooms or items
inside of another house, that would be trespassing!
)===="
