/* C++ program to calculate the initial price of a straddle purchase option using the CRR formula */

#include<iostream>
#include<cmath>

using std::cout;
using std::cin;
using std::endl;

int nCk(int n, int r); // function to calculate number of ways to choose r from n objects
double Binom_CDF(int k, int n, double q); // binomial cumulative distribution function

double CRR_formula_Straddle(double& S0, double& K, int& T, double& u, double& d, double& r);

int main()
{
    double S0, K, u, d, r;
    int T;
    cout << "Please enter the initial stock price" << endl;
    cin >> S0;
    cout << "Please enter the strike price" << endl;
    cin >> K;
    cout << "Please enter the time to maturity" << endl;
    cin >> T;
    cout << "Please enter the upward factor" << endl;
    cin >> u;
    cout << "Please enter the downward factor " << endl;
    cin >> d;
    cout << "Please enter fixed interest rate" << endl;
    cin >> r;
    cout << "The initial price of a plain vanilla straddle purchase is " << CRR_formula_Straddle(S0, K, T, u, d, r) << endl;
    
    return 0;
}

int nCk(int n, int r)
{
    // since nCr is same as nC(n-r)
    // to decrease number of iterations
    if (r > n/2)
        r = n - r;
    
    int var0 = 1;
    for (int i=1; i<=r; i++)
    {
        var0 *= (n - r + i);
        var0 /= i;
    }
    
    return var0;
    
}

double Binom_CDF(int k, int n, double q)
{
    double var1 = 0;
    for (int i=0; i<=k; i++)
    {
        var1 += nCk(n,i)*pow(q,i)*pow(1-q,n-i);
    }
    
    return var1;
}

double CRR_formula_Straddle(double& S0, double& K, int& T, double& u, double& d, double& r)
{
    // check input parameters
    if (S0 <=0 || d<=-1 || d>=u || r<=-1)
    {
        cout << "None." << endl;
        cout << "Invalid input arguments." << endl;
        cout << "Terminating program." << endl;
        return 1;
    }
    // check for arbitrage
    if (!(d<r && r<u))
    {
        cout << "None." << endl;
        cout << "Invalid input arguments." << endl;
        cout << "Terminating program." << endl;
        return 1;
    }
    
    // crr variables
    double U = 1 + u;
    double D = 1 + d;
    double R = 1 + r;
    
    // calculating terms: q, q_dash, var2
    double q = (R - D) / (U - D);
    double q_dash = ((R - D) / (U - D)) * (U / R);
    
    double var2 = log(K / (S0 * pow(D,T)) / log(U / D));
    if (std::ceil(var2) == std::floor(var2))
        var2 = std::floor(var2) - 1;
    
    var2 = std::floor(var2);
    
    double put_price = (K / pow(R,T)) * Binom_CDF(var2, T, q) - S0 * Binom_CDF(var2, T, q_dash);
    
    double call_price = S0 * (1-Binom_CDF(var2, T, q_dash)) - (K / pow(R,T)) * (1-Binom_CDF(var2, T, q));
    
    return put_price + call_price;
}



