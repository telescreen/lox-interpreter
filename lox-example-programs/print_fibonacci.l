print "Printing fibonacci numbers until 10000";
var a = 0;
var b = 1;
while (a < 10000) {
    print a;
    var t = a;
    a = b;
    b = b + t;
}