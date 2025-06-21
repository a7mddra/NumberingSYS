#include "bits/stdc++.h"
using namespace std;
using ll = long long;
string el = "\n";


string toBinary(ll n, ll vars) {
    string res;
    for (ll i = vars - 1; i >= 0; i--) {
        ll bit = (n >> i) & 1;
        res += bit ? '1' : '0';
    }
    return res;
}


void extract(string line, vector<ll>& mins, vector<ll>& dcs) {
    stringstream ss(line);
    string token;
    while (ss >> token) {
        if (token.empty()) continue;
        try {
            size_t pos;
            ll num = stoll(token, &pos, 10);
            if (pos == token.size()) mins.push_back(num);
            else if (pos == token.size() - 1 && token[pos] == 'd') {
                mins.push_back(num);
                dcs.push_back(num);
            }
        } catch (const invalid_argument& e) { continue; }
        catch (const out_of_range& e) { continue; }
    }
}


ll check(string s, string t) {
    ll diff = 0;
    ll pos = -1;
    for (ll i = 0; i < s.size(); i++) {
        if (s[i] != t[i]) {
            diff++;
            pos = i;
        }
    }
    return (diff == 1) ? pos : -1;
}

void srch(const set<ll>& a, const vector<ll>& b, set<ll>& c) {
    unordered_map<ll, ll> counts;
    for (ll x : b) counts[x]++;
    for (ll x : a) counts[x]--;
    for (const auto& pair : counts)
    if (pair.second > 0) c.insert(pair.first);
}

void fltr(const set<ll>& nums, const vector<ll>& mins, const vector<pair<pair<ll, ll>, string>>& in, vector<pair<pair<ll, ll>, string>>& out) {
    for (const auto& p : in)
        if (nums.count(mins[p.first.first]) > 0 || nums.count(mins[p.first.second]) > 0)
            out.push_back({{p.first}, p.second});
}

ll fnd(vector<ll>& mins, ll n) {
    ll pos=-1;
    for (ll i=0; i<mins.size(); i++) 
        if (mins[i]==n) {pos=i; break;}
    return pos;
}

bool isEmpty(vector<vector<string>> table) {
    for (auto&v:table)
        for (auto&s:v)
            if (s=="X") {return false;}
    return true;
}

void print(vector<vector<string>> table, ll r, ll c, vector<pair<pair<ll, ll>, string>> pr, vector<pair<vector<ll>, string>> mrgs){
    cout <<el;
    for (ll i=0; i<r; i++) { 
        for (ll j=0; j<c; j++) {
            cout << table[i][j] << " ";
        }
        cout << " " << (i<pr.size()? pr[i].second:mrgs[i-pr.size()].second) << el;
    }
}

string decode(string s)
{
    string res;
    for (ll i=0; i<s.size(); i++) {
        if (s[i]!='_') res+= 'A'+i;
        if (s[i]=='0') res+="'"; 
    }
    return res;
}

int main() {
    ll vars;
    string input;

    cout << "Quine-McCluskey Prime Implicant Solver" << el << el;
    cout << "Enter number of variables: ";
    cin >> vars;
    cin.ignore();
    cout << "Enter space-separated terms. Use `d` as a suffix for don't-care terms (e.g., 2d 5 6d):" << el;
    cout << "Example: 0 1 2 5 6 7 8 9 10 14d" << el << el;

    cout << "F(";
    for (ll i = 0; i < vars; i++) {
        cout << char('A' + i);
        if (i < vars - 1) cout << ", ";
    }
    cout << ") = Σ: ";
    
    getline(cin, input);
    vector<ll> mins, dcs;
    extract(input, mins, dcs);
    
    set<ll> allMintermsSet(mins.begin(), mins.end());
    set<ll> dontCareSet(dcs.begin(), dcs.end());
    set<ll> essentialMinterms;
    set_difference(allMintermsSet.begin(), allMintermsSet.end(), 
                  dontCareSet.begin(), dontCareSet.end(),
                  inserter(essentialMinterms, essentialMinterms.begin()));
    
    vector<string> bin;
    for (ll& i : mins) bin.push_back(toBinary(i, vars));

    map<string, set<ll>> implicants;
    for (ll i = 0; i < mins.size(); i++) {
        implicants[bin[i]] = {mins[i]};
    }

    bool changed = true;
    while (changed) {
        changed = false;
        map<string, set<ll>> nextImplicants;
        set<string> merged;

        for (auto it1 = implicants.begin(); it1 != implicants.end(); ++it1) {
            auto it2 = it1;
            ++it2;
            for (; it2 != implicants.end(); ++it2) {
                string s1 = it1->first, s2 = it2->first;
                ll diffPos = check(s1, s2);
                if (diffPos != -1) {
                    string mergedStr = s1;
                    mergedStr[diffPos] = '_';
                    set<ll> combined = it1->second;
                    combined.insert(it2->second.begin(), it2->second.end());
                    nextImplicants[mergedStr] = combined;
                    merged.insert(s1);
                    merged.insert(s2);
                    changed = true;
                }
            }
        }

        for (const auto& imp : implicants) {
            if (merged.find(imp.first) == merged.end()) {
                nextImplicants[imp.first] = imp.second;
            }
        }
        implicants = nextImplicants;
    }

    vector<pair<set<ll>, string>> primeImplicants;
    for (const auto& imp : implicants) {
        primeImplicants.push_back({imp.second, imp.first});
    }

    vector<vector<string>> table;
    ll r = primeImplicants.size();
    ll c = mins.size();
    table.assign(r, vector<string>(c, "O"));

    for (ll i = 0; i < r; i++) {
        for (ll j = 0; j < c; j++) {
            if (primeImplicants[i].first.find(mins[j]) != primeImplicants[i].first.end()) {
                table[i][j] = "X";
            }
        }
    }

    for (ll i = 0; i < dcs.size(); i++) {
        ll idx = fnd(mins, dcs[i]);
        if (idx != -1) {
            for (ll j = 0; j < r; j++) {
                table[j][idx] = "O";
            }
        }
    }

    cout << "Prime Implicant Table:" << el;
    for (ll i = 0; i < r; i++) {
        for (ll j = 0; j < c; j++) {
            cout << table[i][j] << " ";
        }
        cout << " " << primeImplicants[i].second << el;
    }

    vector<string> ans;
    set<ll> covered;
    vector<bool> rowUsed(r, false);
    vector<bool> colCovered(c, false);

    for (ll j = 0; j < c; j++) {
        if (essentialMinterms.find(mins[j]) == essentialMinterms.end()) continue;
        ll count = 0;
        ll essentialRow = -1;
        for (ll i = 0; i < r; i++) {
            if (table[i][j] == "X") {
                count++;
                essentialRow = i;
            }
        }
        if (count == 1) {
            ans.push_back(primeImplicants[essentialRow].second);
            rowUsed[essentialRow] = true;
            for (ll k = 0; k < c; k++) {
                if (table[essentialRow][k] == "X") {
                    colCovered[k] = true;
                }
            }
        }
    }

    while (true) {
        bool allCovered = true;
        for (ll j = 0; j < c; j++) {
            if (essentialMinterms.find(mins[j]) == essentialMinterms.end()) continue;
            if (!colCovered[j]) {
                allCovered = false;
                break;
            }
        }
        if (allCovered) break;

        ll maxCover = -1, selectedRow = -1;
        for (ll i = 0; i < r; i++) {
            if (rowUsed[i]) continue;
            ll coverCount = 0;
            for (ll j = 0; j < c; j++) {
                if (!colCovered[j] && table[i][j] == "X") {
                    coverCount++;
                }
            }
            if (coverCount > maxCover) {
                maxCover = coverCount;
                selectedRow = i;
            }
        }

        if (selectedRow != -1) {
            ans.push_back(primeImplicants[selectedRow].second);
            rowUsed[selectedRow] = true;
            for (ll j = 0; j < c; j++) {
                if (table[selectedRow][j] == "X") {
                    colCovered[j] = true;
                }
            }
        } else {
            break;
        }
    }

    cout << el;
    for (string s : ans) cout << s << ", ";
    cout << el << el << "FINAL ANSWER: ";
    for (ll i = 0; i < ans.size(); i++) cout << decode(ans[i]) << (i < ans.size() - 1 ? " + " : el);
    return 0;
}
