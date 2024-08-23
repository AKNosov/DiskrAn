#include <iostream>

struct KV{
    char key[9];
	char value[65];
};

class TVect {
    int size;
	int cap;
    KV* vectData;

public:
 	TVect() {
        size = 0;
        cap = 0;
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

    void PushBack(KV element) {
        if (size == cap) {
                if (cap == 0) {
            		cap = 1;
        		}
        		cap *=2;
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

void RadixSort(TVect &vect){
	char charc[36] = {};
	char digc[10] = {};
	TVect temp(vect.VectSize());
	for (int i = 8; i != 0; i--){
		if (vect[0].key[i-1] == ' ') {
            continue;
        } 
        if (vect[0].key[i-1] >= '0' && vect[0].key[i-1] <= '9') {
            int p = 0;
            int m = 0;
            for (p = 0; p < vect.VectSize(); p++) {
                digc[vect[p].key[i-1] - '0'] += 1;
            }
            for (m = 1; m < 10; m++) {
                digc[m] += digc[m - 1];
            }
            for (p = vect.VectSize(); p != 0; p--) {
                --digc[vect[p - 1].key[i-1] - '0'];
                temp[digc[vect[p - 1].key[i-1] - '0']] = vect[p - 1];
            }
        } 
		
		else {
            int p = 0;
            int m = 0;
            for (p = 0; p < vect.VectSize(); p++) {
                charc[vect[p].key[i-1] - 'A'] += 1;
            }
            for (m = 1; m < 26; m++) {
                charc[m] += charc[m - 1];
            }
            for (p = vect.VectSize(); p != 0; p--) {
                --charc[vect[p - 1].key[i-1] - 'A'];
                temp[charc[vect[p - 1].key[i-1] - 'A']] = vect[p - 1];
            }
        }
		for (int k = 0; k < 26; k++) {
            charc[k] = 0;
        }
		for (int k = 0; k < 10; k++) {
            digc[k] = 0;
        }
        for (int m = 0; m < vect.VectSize(); m++) {
            vect[m] = temp[m];
        }

	}
}

int main()
{
	KV el;
	TVect data;
	char str[74];
	while (true)
	{	

		str[0] = '\0';
        std::cin.getline(str, 74);
	    int k = 0;
        int j = 0;
		if (std::cin.eof()) {
            break;
        }
        for (k = 0; k < 8; k++) {
            el.key[k] = str[k];
        }
        el.key[8] = '\0';
        k += 1;
		while (k < 74) {
            el.value[j++] = str[k++];
        }
		data.PushBack(el);
		for (int i = 0; i < 74; i ++){
			str[i] = '\0';
		}
		for (int i = 0; i < 64; i ++){
			el.value[i] = '\0';
		}
	}


	RadixSort(data);
	
    for (int j = 0; j < data.VectSize(); j++) {
        std::cout << data[j].key << '\t' << data[j].value << std::endl;
    }
    return 0;
}