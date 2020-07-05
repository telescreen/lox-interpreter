fun fact(n) {
    if (n>=1)
        return n*fact(n-1);
    else
        return 1;
}

print "factorial of 6 is: ";
print fact(6);