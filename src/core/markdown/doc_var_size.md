u8R"====(## EXPLANATION
*Variable size error* is a special error implemented by CodeNect. This occurs during transpilation stage when a *Variable Node* that is *not* of type *string* is connected to a *Size Node* In programming languages, the size of a variable depends on the data type of the variable but they are measured in bytes which is not beginner-friendly (though understanding this fundamental concept is important!). *Variable Node* of type *string* however, can be connected to a *Size Node* because usually, programmers want to determine at runtime the length of a string. A length of a string is its number of characters.
## SAMPLE CODE
Here is a *pseudo-code* example:
***
a = 100;
b = 100000;
c = 10000000;
//you might think that a, b, and c vary in size because they hold different
values. BUT they are still of type INTEGER, so their size is the same
size(a); // 8 bytes (depending on the OS and programming language)
size(b); // 8 bytes (depending on the OS and programming language)
size(c); // 8 bytes (depending on the OS and programming language)
//here's the usage for strings
d = "Hello, World!";
e = "Foo Bar Baz";
f = "Antidisestablishmentarianism";
size(d); //13 characters (whitespace and exclamation mark are counted)
size(e); //11 characters
size(f); //28 characters
***
## KEY POINTS
  * You can get the *size* or *character length* of a *Variable Node* if it is of type *string*
  * You can get the *size* or *number of elements stored* of a *Array Node*
)===="
