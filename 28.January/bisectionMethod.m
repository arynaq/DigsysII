function [c,currIter] = bisectionMethod(a,b,currIter, maxIter, f)

tol = 1E-4;

c = (a+b)/2;

if(abs(f(c)) < tol || currIter==maxIter)
    return;  
end

if(f(c)*f(b) > 0)
    b = c;
else
    a = c;
end
bisectionMethod(a,b,currIter+1, maxIter,f);

end