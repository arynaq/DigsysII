double bisectionMethod(double, double, int, double (*f)(double));
double newtonsMethod(double a, double b, double x0, int maxIter, double (*f)(double), double (*df)(double));
static double bisectionMethodActual(double, double, int, int, double(*f)(double));
static double newtonsMethodActual(double a, double b, double x0, int currIter, int maxIter, 
									double (*f)(double), double (*df)(double));
