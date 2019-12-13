/* C++ program to calculate the initial price of a Bermudan put option in the CRR method */

#include <iostream>
#include <vector>
#include <cmath>

using std::cout;
using std::cin;
using std::endl;
using std::vector;

double Am_crr_put(double& S0, double& K, int& T, double& u, double& d, double& r);

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

    cout << "The initial price of a plain vanilla put is " << Am_crr_put(S0, K, T, u, d, r) << endl;

    return 0;
}

double Am_crr_put(double& S0, double& K, int& T, double& u, double& d, double& r)
{
    // check input parameters
    if (S0 <=0 || u<=-1 || d<=-1 || d>=u || r<=-1)
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

    // calculate terminal asset prices
    vector <double> ST;
    for (int i=0; i<=T; ++i)
    {
        ST.push_back(S0*pow(1+u,T-i)*pow(1+d,i));
    }

    // calculate terminal put prices
    vector <double> prices;
    for (int i=0; i<=T; ++i)
    {
        prices.push_back(K-ST[i]>0?K-ST[i]:0);
    }

    // calculate risk neutral probabilities
    double q=(r-d)/(u-d);

    // backward induction
    double Zt; // immediate payoff of put at times t=T-1,T-2,..,1,0
    double Pt; // expected discounted value of put at times t=T-1,T-2,..,1,0
    for (int n=T-1; n>=0; --n)
    {
        for (int j=0; j<=n; ++j)
        {
            Zt = (K - S0*pow((1+u),n - j)*pow((d + 1),j))>0 ? (K - S0*pow((1+u),n - j)*pow((d + 1),j)):0;
            Pt = (q*prices[j] + (1-q)*prices[j+1])/(1+r);
            prices[j] = Zt>Pt?Zt:Pt;
        }
    }

    return prices[0];
}
