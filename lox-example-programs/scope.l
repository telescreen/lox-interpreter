var a = "global a";
var b = "global b";

{
    var a = "scope1 a";
    {
        var a = "scope2 a";
        var b = "scope2 b";
        print a;   // --> scope2 a
        print b;   // --> scope2 b
    }
    print a;      // --> scope1 b
    print b;      // --> global b
}

print a;   // --> global a
print b;   // --> global b
