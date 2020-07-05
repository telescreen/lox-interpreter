fun fact(n) {
    if (n>=1)
        return n*fact(n-1);
    else
        return 1;
}

print fact(6);