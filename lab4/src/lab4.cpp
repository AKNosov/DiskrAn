#include <iostream>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <sstream>

using namespace std;

unordered_map<uint32_t, int> BCTable(const vector<uint32_t>& pat) {
    unordered_map<uint32_t, int> badc;
    for (int i = 0; i < pat.size(); ++i) {
        badc[pat[i]] = i; 
    }
    return badc;
}

void Pobeda(const vector<uint32_t>& pat, vector<uint32_t>& suff) {
    int m = pat.size();
    suff.resize(m);
    suff[m - 1] = m;
    int g = m - 1;
    int f = 0;
    for (int i = m - 2; i >= 0; --i) {
        if (i > g && suff[i + m - 1 - f] < i - g) {
            suff[i] = suff[i + m - 1 - f];
        }
        else {
            if (i < g) {
                g = i;
            }
            f = i;
            while (g >= 0 && pat[g] == pat[g + m - 1 - f]) {
                --g;
            }
            suff[i] = f - g;
        }
    }
}

vector<uint32_t> GSTable(const vector<uint32_t>& pat) {
    vector<uint32_t> gs;
    int m = pat.size();
    vector<uint32_t> suff;
    Pobeda(pat, suff);
    gs.resize(m, m);
    int j = 0;
    for (int i = m - 1; i >= 0; --i) {
        if (suff[i] == i + 1) {
            for (; j < m - 1 - i; ++j) {
                if (gs[j] == m) {
                    gs[j] = m - 1 - i;
                }
            }
        }
    }
    for (int i = 0; i <= m - 2; ++i) {
        gs[m - 1 - suff[i]] = m - 1 - i; 
    }
    return gs;
}

vector<int> BoyerMoore(const vector<uint32_t>& text, const vector<uint32_t>& pat, 
const unordered_map<uint32_t, int>& badc, const vector<uint32_t>& gs){
    int m = pat.size();
    int n = text.size();
    vector<int> ocur;
    int s = 0; 
    while (s <= n - m) {
        int j = m - 1; 
        while (j >= 0 && pat[j] == text[s + j]) {
            --j;
        }
        if (j < 0) {
            ocur.emplace_back(s);
            s += gs[0];  
        }
        else {
            auto it = badc.find(text[s + j]);
            int badc_shift = (it != badc.end()) ? it->second : -1;
            uint32_t bc_shift = j - badc_shift;
            uint32_t i  = 1;
            s += max(i, max(bc_shift, gs[j])); 
        }
    }
    return ocur;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    string line;
    vector<uint32_t> pat;
    uint32_t num;
    while (getline(cin, line)) {
        istringstream iss(line);
        while (iss >> num) {
            pat.emplace_back(num);
        }
        if (!pat.empty()) {
            break; 
        }
    }
    if (pat.empty()) {
        return 0;  
    }
    unordered_map<uint32_t, int> badc = BCTable(pat);
    vector<uint32_t> gs = GSTable(pat);
    vector<pair<unsigned int, unsigned int>> pos;
    vector<uint32_t> text;
    unsigned int lcount = 1;
    while (getline(cin, line)) {
        istringstream iss(line);
        for (unsigned int wcount = 1; iss >> num; ++wcount) {
            text.emplace_back(num);
            pos.emplace_back(make_pair(lcount, wcount));
        }
        ++lcount;
    }
    if (text.empty()) {
        return 0;  
    }
    if (text.size() < pat.size()){
        return 0;
    }
    vector<int> ocur = BoyerMoore(text, pat, badc, gs);
        for (int s : ocur) {
            cout << pos[s].first << ", " << pos[s].second << endl;
        }
    return 0;
}