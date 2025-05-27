/*
 *  Author: a7mddra
 */
#include "bits/stdc++.h"
using namespace std;
using ll = long long;
using ld = long double;

ll to_ll(string __str)
  {
    ll __val=0;
    for (char c:__str) {
        __val = __val*10+(c-'0');
    }
    return __val;
  }

string DEC(string num, ll sys)
{
    ll n=to_ll(num);
    if (n==0) return "0";
    string res;
    while (n>0) {
        ll digit = n%sys;
        res += digit<10? digit+'0':digit-10+'A';
        n/=sys;
    }
    reverse(res.begin(), res.end());
    return res;
}

string CNV(string num, ll sys1, ll sys2)
{
    ll bits=num.size(), group=log(sys2)/log(sys1);
    ld step = log(sys2)/log(sys1);
    group = step==group?group:bits;

    while(bits%group>0) {
        num.insert(num.begin(), '0');
        bits++;
    }

    string res;
    ll bit=0;
    for (ll i=0; i<bits; i++) {
        ll digit = (num[i] >= 'A' ? num[i] - 'A' + 10 : num[i] - '0');
        bit += digit * pow(sys1, (bits - i - 1) % group);
        if ((bits-i-1)%group==0) {
            res += DEC(to_string(bit), sys2);
            bit=0;
        }
    }
    return res;
}

bool OR  (ll a, ll b) { return a+b>0; }
bool AND (ll a, ll b) { return a*b>0; }
bool NAND(ll a, ll b) { return !AND(a, b); }
bool XOR (ll a, ll b) { return AND(NAND(a, b), OR(a, b)); }

string HADDER(char a, char b)
{
    string out="00";
    out[0] = AND(a-'0', b-'0')+'0';
    out[1] = XOR(a-'0', b-'0')+'0';
    return out;
}

string FADDER(char ci, char a, char b)
{
    string out ="00", h1=HADDER(a, b), h2=HADDER(h1[1], ci);
    out[0] = OR(h1[0]-'0', h2[0]-'0')+'0';
    out[1] = h2[1];
    return out;
}

string SUM(string num1, string num2)
{
    ll bits1 = num1.size(), bits2 = num2.size();
    while (bits1 != bits2) {
        if (bits1 < bits2) num1.insert(num1.begin(), '0'), bits1++;
        else num2.insert(num2.begin(), '0'), bits2++;
    }
    string res = "";
    string f0 = FADDER('0', num1[bits1-1], num2[bits1-1]);
    for (ll i = 0; i < bits1; i++) res += '0';
    res[bits1-1] = f0[1];
    char ci = f0[0];
    for (ll i = bits1-2; i >= 0; i--) {
        string fi = FADDER(ci, num1[i], num2[i]);
        res[i] = fi[1];
        ci = fi[0];
    }
    if (ci == '1') res = '1' + res;
    return res;
}

string CMP(string num, ll order, ll src) 
{
    string tmp="";
    for (ll i=0; i<num.size(); i++) {
        if (num[i]=='1'||tmp!="") {
            tmp+=num[i];
        }
    }
    if (tmp == "") tmp = "0";
    while (tmp.size() < src) {
        tmp.insert(tmp.begin(), '0');
    }
    for (char&c:tmp) c=(c=='0')?'1':'0';
    if (order > 1) tmp = SUM(tmp, "1");
    return tmp;
}

void solve()
{
    cout << "Choose operation:\n1.CNV\n2.SUM\n3.SUB\n";
    ll op, sys; cin >> op;
    string num, num1, num2, res;
    switch (op)
    {
    case 1: 
        cout << "1.D2B  2.D2O  3.D2H\n4.B2D  5.B2O  6.B2H\n7.O2D  8.O2B  9.O2H\n10.H2D 11.H2B  12.H2O\n13.1st CMP  14.2nd CMP\n";
        ll cnv; cin >> cnv;
        cout << "Enter number: ";
            cin >> num;
            switch (cnv) {
                case 1: res = DEC(num, 2); break;
                case 2: res = DEC(num, 8); break;
                case 3: res = DEC(num, 16); break;
                case 4: res = CNV(num, 2, 10); break;
                case 5: res = CNV(num, 2, 8); break;
                case 6: res = CNV(num, 2, 16); break;
                case 7: res = CNV(num, 8, 10); break;
                case 8: res = CNV(num, 8, 2); break;
                case 9: res = CNV(num, 8, 16); break;
                case 10: res = CNV(num, 16, 10); break;
                case 11: res = CNV(num, 16, 2); break;
                case 12: res = CNV(num, 16, 8); break;
                case 13: res = CMP(num, 1, num.size()); break;
                case 14: res = CMP(num, 2, num.size()); break;
                default: cout << "404 not found\n"; break;
            }
            break;
        
        case 2:
            cout << "1.B\n2.O\n3.H\n";
            cin >> sys;
            cout << "Enter number1: "; cin >> num1;
            cout << "Enter number2: "; cin >> num2;
            switch (sys) {
                case 1: res = SUM(num1, num2); break;
                case 2: res = CNV(SUM(CNV(num1,8,2), CNV(num2,8,2)), 2,8); break;
                case 3: res = CNV(SUM(CNV(num1,16,2), CNV(num2,16,2)), 2,16); break;
                default: break;
            }
            break;
        
        case 3: 
            cout << "1.B\n2.O\n3.H\n";
            cin >> sys;
            cout << "Enter number1: "; cin >> num1;
            cout << "Enter number2: "; cin >> num2;
            switch (sys) {
                case 1: res = SUM(num1, CMP(num2,2, num1.size())).substr(1);
                if (to_ll(CNV(num1, 2, 10))<to_ll(CNV(num2, 2, 10))) res = CMP(res, 2, num2.size()); break;
                case 2: res = SUM(CNV(num1,8,2), CMP(CNV(num2,8,2),2, num1.size())).substr(1); 
                if (to_ll(CNV(num1, 8, 10))<to_ll(CNV(num2, 8, 10))) res = CNV(CMP(res, 2, CNV(num2,8,2).size()), 2, 8); break;
                case 3: res = SUM(CNV(num1,16,2), CMP(CNV(num2,16,2),2, num1.size())).substr(1);
                if (to_ll(CNV(num1, 16, 10))<to_ll(CNV(num2, 16, 10))) res = CNV(CMP(res, 2, CNV(num2,16,2).size()), 2, 16); break;
                default: break;
            }
            break;

        default: cout << "404 not found\n"; break;
    }
    cout << res << "\n";
}

int main()
{
    ll T = 1;
    while (T--) solve();
    return 0; 
}
