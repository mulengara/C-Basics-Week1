#include<stdio.h>
int main()
{
printf("My name is Mediate Moonga,I am 23 years old and my hobby is cooking\n");
char name[50];
int age;
printf("Enter name:\n");
scanf("%s", name);
printf("Enter age:\n");
scanf("%d", &age);
printf("Hello %s,you are %d years old", name, age);
return 0;
}
