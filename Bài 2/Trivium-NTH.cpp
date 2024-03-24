#include <bits/stdc++.h>
#include <dirent.h>
using namespace std;

//vector<string>inputs = {"alice29.txt",
//                    "asyoulik.txt",
//                    "bible.txt",
//                    "cp.html",
//                    "E.coli",
//                    "fields.c",
//                    "grammar.lsp",
//                    "kennedy.xls",
//                    "lcet10.txt",
//                    "plrabn12.txt",
//                    "ptt5",
//                    "sum",
//                    "world192.txt",
//                    "xargs.1"};

class Trivium {
    bitset<288> state;

public:
    Trivium(const bitset<80>& key, const bitset<80>& iv) {
        for (int i = 0; i < 80; ++i) {
	        state[i + 93] = key[i];
	        state[i + 173] = iv[i];
    	}
    state[285] = state[286] = state[287] = 1;
    }

    bitset<1> generate() {
        bitset<1> output = (state[65] ^ state[92]) ^ (state[161] ^ state[176]) ^ (state[242] ^ state[287]);

        bitset<1> feedback1 = (state[65] ^ state[92]) ^ (state[90] & state[91]) ^ state[170];
        bitset<1> feedback2 = (state[161] ^ state[176])^ (state[174] & state[175]) ^ state[263];
        bitset<1> feedback3 = (state[242] ^ state[287]) ^ (state[285] & state[286]) ^ state[68];

        state >>= 1;
        state[93] = feedback1[0];
        state[177] = feedback2[0];
        state[288] = feedback3[0];

        return output;
    }
};

void encryptFile(const string& filename, const bitset<80>& key) {
    ifstream input("./input/" + filename, ios::binary);
    ofstream output("./output/encrypted-" + filename , ios::binary);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 1);
    bitset<80> iv;
    for (int i = 0; i < 80; ++i) {
        iv[i] = dis(gen);
    }

    Trivium trivium(key, iv);

    auto start = chrono::high_resolution_clock::now();

    char c;
    while (input.get(c) ) {
        bitset<8> plaintext(c);
        bitset<8> ciphertext;
        for (int i = 0; i < 8; ++i) {
            ciphertext[i] = plaintext[i] ^ trivium.generate()[0];
        }
        output.put(static_cast<char>(ciphertext.to_ulong()));
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    cout << "File: " << filename << '\n';
    cout << "IV: " << iv << '\n';
    cout << "Thoi gian thuc hien: " << duration << "ms" << '\n';
	cout << "--------------------------------------------------" << '\n';
    input.close();
    output.close();
}

int main() {
    ifstream keyFile("key.txt");
    string key_str;
    getline(keyFile, key_str);
    keyFile.close();
    cout << "Key in key.txt: " << key_str << '\n'; 
    bitset<80> key(key_str);
    DIR* dirp = opendir("./input");
    vector<string> inputs;
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        if (string(dp->d_name) != "." && string(dp->d_name) != "..") {
            inputs.push_back(dp->d_name);
        }
    }

    closedir(dirp);
	cout << "--------------------------------------------------" << '\n';
	for (auto filename : inputs) {
		encryptFile(filename, key);
	} 
	cout << "Hoan thanh chuong trinh" << '\n';
    return 0;
}
