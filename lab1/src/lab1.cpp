#include <iostream>

struct KV{
    std::string key;
    std::string value;
};

class TVect {
    int size;
    int cap;
    KV* vectData;

public:
     TVect() {
        size = 0;
        cap = 2;
        vectData = new KV[2];
    }

    TVect(int i) {
        size = i;
        cap = i;
        vectData = new KV[i];
    }

    KV& operator[] (int k) {
        return vectData[k];
    }

    int VectSize() {
        return size;
    }

    void PushBack(const KV& element) {
        if (size == cap) {
                if (cap == 0) {
                    cap = 1;
                }
                cap *= 2;
                KV* temp = new KV[cap];
                for (int j = 0; j < size; j++) {
                    temp[j] = vectData[j];
                }
                delete [] vectData;
                vectData = temp;
        }
        vectData[size] = element;
        size++;
    }

    ~TVect() {
        delete [] vectData;
    }
};

class TIntVect {
    int size;
    int cap;
    int* vectData;

public:
     TIntVect() {
        size = 0;
        cap = 2;
        vectData = new int[2];
    }

    TIntVect(int i) {
        size = i;
        cap = i;
        vectData = new int[i];
    }

    int& operator[] (int k) {
        return vectData[k];
    }

    int VectSize() {
        return size;
    }

    void PushBack(int element) {
        if (size == cap) {
                if (cap == 0) {
                    cap = 1;
                }
                cap *= 2;
                int* temp = new int[cap];
                for (int j = 0; j < size; j++) {
                    temp[j] = vectData[j];
                }
                delete [] vectData;
                vectData = temp;
        }
        vectData[size] = element;
        size++;
    }

    ~TIntVect() {
        delete [] vectData;
    }
};

void RadixSort(TVect &vect, TIntVect &indices) {
    int charc[26] = {};
    int digc[10] = {};
    TIntVect tempIndices(indices.VectSize());
    for (int i = 8; i != 0; i--) {
        if (vect[indices[0]].key[i-1] == ' ') {
            continue;
        }

        if (vect[indices[0]].key[i-1] >= '0' && vect[indices[0]].key[i-1] <= '9') {
            for (int p = 0; p < indices.VectSize(); p++) {
                int idx = indices[p];
                digc[vect[idx].key[i-1] - '0'] += 1;
            }
            for (int m = 1; m < 10; m++) {
                digc[m] += digc[m - 1];
            }
            for (int p = indices.VectSize() - 1; p >= 0; p--) {
                int idx = indices[p];
                --digc[vect[idx].key[i-1] - '0'];
                tempIndices[digc[vect[idx].key[i-1] - '0']] = idx;
            }
        } else {
            for (int p = 0; p < indices.VectSize(); p++) {
                int idx = indices[p];
                charc[vect[idx].key[i-1] - 'A'] += 1;
            }
            for (int m = 1; m < 26; m++) {
                charc[m] += charc[m - 1];
            }
            for (int p = indices.VectSize() - 1; p >= 0; p--) {
                int idx = indices[p];
                --charc[vect[idx].key[i-1] - 'A'];
                tempIndices[charc[vect[idx].key[i-1] - 'A']] = idx;
            }

        }
		for (int k = 0; k < 10; k++) {
                digc[k] = 0;
            }
		for (int k = 0; k < 26; k++) {
                charc[k] = 0;
        }
        for (int m = 0; m < indices.VectSize(); m++) {
            indices[m] = tempIndices[m];
        }
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    TVect data;
    std::string first;
    std::string number;
    std::string second;
    std::string value;
    while (std::cin >> first >> number >> second >> value)
    {
        KV el;
        std::string key;
        key += first;
        key += " ";
        key += number;
        key += " ";
        key += second;
        el.key = key;
        el.value = value;
        data.PushBack(el);
    }

    int dataSize = data.VectSize();
    TIntVect indices(dataSize);
    for (int i = 0; i < dataSize; ++i) {
        indices[i] = i;
    }

    RadixSort(data, indices);

    for (int j = 0; j < dataSize; ++j) {
        const KV &kv = data[indices[j]];
        std::cout << kv.key << '\t' << kv.value << std::endl;
    }
    return 0;
}
