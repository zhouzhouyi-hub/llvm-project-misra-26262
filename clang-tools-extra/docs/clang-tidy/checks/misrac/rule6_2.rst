.. title:: clang-tidy - misrac-rule6_2

misrac-rule6_2
==============

MISRA C rule 6.2 require Single-bit named bit fields shall not be of a signed type:

```
struct s {
        int b1:1;
};

int main()
{
        struct s S;
        int i;
        i++;
}
```
after we invoke clang-tidy -checks='-*,misrac-rule6_2' -fix 6.2.c, we get:

```
struct s {
        unsigned int b1:1;
};

int main()
{
        struct s S;
        int i;
        i++;
}
```

