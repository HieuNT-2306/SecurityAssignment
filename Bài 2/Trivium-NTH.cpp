#include <bits/stdc++.h>
#include <dirent.h>
using namespace std;

class Trivium {
	//su dung 1 state bang 288 de bieu hien cho cac thanh ghi.
	//thanh ghi 1 tuong trung voi state 0-92.
	//thanh ghi 2 tuong trung voi state 93-176
	//thanh ghi 3 tuong trung voi state 177-287
    bitset<288> state;

public:
    Trivium(const bitset<80>& key, const bitset<80>& iv) {
    	//gan cac bit cua key va iv vao trong trang thai ban dau.	
    	//gan iv vao thanh ghi 1
    	//gan key vao thanh ghi 2
        for (int i = 0; i < 80; ++i) {
        	state[i] = iv[i];
	        state[i + 93] = key[i];
    	}
    	//khoi toan 3 bit cuoi cua thanh ghi 3 thanh 1.
    	state[285] = state[286] = state[287] = 1;
    }

    bitset<1> generate() {
    	//tinh output bang ba thanh ghi
        bitset<1> output = (state[65] ^ state[92]) ^ (state[161] ^ state[176]) ^ (state[242] ^ state[287]);
		//phan hoi cua 3 thanh ghi 1, 2, 3
        bitset<1> feedback1 = (state[65] ^ state[92]) ^ (state[90] & state[91]) ^ state[170];
        bitset<1> feedback2 = (state[161] ^ state[176])^ (state[174] & state[175]) ^ state[263];
        bitset<1> feedback3 = (state[242] ^ state[287]) ^ (state[285] & state[286]) ^ state[68];
		//dich chuyen ca 3 thanh ghi 1 bit
        state >>= 1;
        //dua cac bit phan hoi vao thanh ghi theo mo ta
        state[93] = feedback1[0];
        state[177] = feedback2[0];
        state[0] = feedback3[0];

        return output;
    }
};

void encryptFile(const string& filename, const bitset<80>& key) {
	//Doc cac file tu tep tin input, va cac file ma hoa cho vao tep tin output.
    ifstream input("./input/" + filename, ios::binary);
    ofstream output("./output/encrypted-" + filename , ios::binary);
    
    //tao gia tri ban dau (iv) su dung thuat toan Mersenne Twister
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 1);
    bitset<80> iv;
    for (int i = 0; i < 80; ++i) {
        iv[i] = dis(gen);
    }

    Trivium trivium(key, iv);
	//bam gio thoi gian
    auto start = chrono::high_resolution_clock::now();

    char c;
    //sau do, ma hoa tung ki tu trong file input va dua vao file output
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
	//Tra ve ten va thoi gian
    cout << "File: " << filename << '\n';
    cout << "IV: " << iv << '\n';
    cout << "Thoi gian thuc hien: " << duration << "ms" << '\n';
	cout << "--------------------------------------------------" << '\n';
    input.close();
    output.close();
}

int main() {
	//Doc khoa trong file key.txt
    ifstream keyFile("key.txt");
    string key_str;
    getline(keyFile, key_str);
    keyFile.close();
    cout << "Khoa trong file key.txt: " << key_str << '\n'; 
    bitset<80> key(key_str);
    
    //Doc cac file tu tap input, va dua ten vao tap tin
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
