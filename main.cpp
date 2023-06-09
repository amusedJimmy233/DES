#include "des.h"
#include "cmath"

using namespace std;

//置换IP表
const static int IP_Table[64] = {
        58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};
//逆置换IP-1表
const static int IPR_Table[64] = {
        40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
};
//E位选择表(扩展置换表)
static const int E_Table[48] = {
        32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
        8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
};
//P换位表(单纯换位表)
const static int P_Table[32] = {
        16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
        2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
};
//PC1选位表(密钥生成置换表1)
const static int PC1_Table[56] = {
        57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
};
//PC2选位表(密钥生成置换表2)
const static int PC2_Table[48] = {
        14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};
//左移位数表
const static int Shift_Table[16] = {
        1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

// S盒
const static int S_Box[8][4][16] = {
        // S1
        14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
        0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
        4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
        15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13,
        //S2
        15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
        3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
        0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
        13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9,
        //S3
        10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
        13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
        13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
        1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,
        //S4
        7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
        13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
        10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
        3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,
        //S5
        2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
        14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
        4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
        11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,
        //S6
        12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
        10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
        9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
        4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,
        //S7
        4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
        13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
        1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
        6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,
        //S8
        13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
        1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
        7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
        2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
};


int main() {
    cout << "text:" << endl;
    string input_text = input_information();
    cout << "key:" << endl;
    string input_key = input_information();
    if (input_key.size() > 8) {
        cout << "the length of key must less than 8" << endl;
        return 0;
    }

    //初始化密钥
    key des_key(input_key);

    //加密
    int encrypt_cnt = ceil(double(input_text.length()) / 8);
    for (int cnt = 0; cnt < encrypt_cnt; cnt++) {
        string encrypt_word;
        if (cnt == encrypt_cnt) {
            encrypt_word = input_text.substr(cnt * 8);
        } else {
            encrypt_word = input_text.substr(cnt * 8, 8);
        }
        cout << "Text：" << encrypt_word << endl;
        des_encrypt(encrypt_word, des_key);
    }

    return 0;
}

string input_information() {
    string word;
    getline(cin, word);
    return word;
}

key::key(string k0) {
    char2bit(bit_key, k0);

    print("Hexadecimal key", bit_key);
    cout << endl;

    //PC1置换
    transform(bit_key, bit_key, PC1_Table, 56);
    bool *l = &bit_key[0];
    bool *r = &bit_key[28];
    for (int i = 0; i < 16; i++) {
        shift_left(l, Shift_Table[i]);
        shift_left(r, Shift_Table[i]);
        //PC2置换
        transform(round_key[i], bit_key, PC2_Table, 48);
    }
}

void char2bit(bool *output, string &input) {
    for (unsigned int i = 0; i < input.length() * 8; i++) {
        output[i] = (input[i / 8] << (i % 8)) & 0x80;
    }
    for (unsigned int i = input.length() * 8; i < 64; i++) {
        output[i] = false;
    }
}

void transform(bool *output, bool *input, const int *table, int tablelen) {
    bool temp[1024];
    for (int i = 0; i < tablelen; i++) {
        temp[i] = input[table[i] - 1];
    }
    memcpy(output, temp, tablelen);
}

void shift_left(bool *input, int len) {
    bool temp[28];
    memcpy(temp, &input[len], 28 - len);
    memcpy(&temp[28 - len], input, len);
    memcpy(input, temp, 28);
}

void des_encrypt(string &text, key des_key) {
    bool text_bit[64];
    char2bit(text_bit, text);

    print("Hexadecimal text", text_bit);

    //IP置换
    transform(text_bit, text_bit, IP_Table, 64);

    print("Initial permutation", text_bit);

    bool *xor1 = &text_bit[0];
    bool *xor2 = &text_bit[32];
    for (int i = 0; i < 16; i++) {
        bool temp[32];
        memcpy(temp, xor2, 32);
        cipher_func(xor2, des_key.round_key[i]);
        xor_bit(xor2, xor1, 32);
        memcpy(xor1, temp, 32);

        cout << "Round " << i << ":";
        show_in_hex(text_bit);
    }
    bool temp[32];
    memcpy(temp, xor2, 32);
    memcpy(xor2, xor1, 32);
    memcpy(xor1, temp, 32);

    //最终置换
    transform(text_bit, text_bit, IPR_Table, 64);

    print("Inverse initial permutation", text_bit);
    cout << endl;
}

void cipher_func(bool *target, bool *key) {
    bool temp[48];
    //扩展置换
    transform(temp, target, E_Table, 48);
    xor_bit(temp, key, 48);
    s_func(target, temp);
    //P盒置换
    transform(target, target, P_Table, 32);
}

void s_func(bool *output, const bool *input) {
    for (int i = 0; i < 8; i++) {
        int row = 2 * input[6 * i] + 1 * input[6 * i + 5];
        int col = 8 * input[6 * i + 1] + 4 * input[6 * i + 2] + 2 * input[6 * i + 3] + 1 * input[6 * i + 4];
        int s_result = S_Box[i][row][col];
        for (int j = 0; j < 4; j++) {
            output[4 * i + j] = (s_result << j) & 0x8; //转化二进制
        }
    }
}

void xor_bit(bool *input1, const bool *input2, int len) {
    for (int i = 0; i < len; i++) {
        input1[i] = input1[i] ^ input2[i];
    }
}

void show_in_hex(const bool input[64]) {
    unsigned int Hex[16] = {0};
    for (int i = 0; i < 16; i++) {
        Hex[i] = 8 * input[0 + i * 4] + 4 * input[1 + i * 4] + 2 * input[2 + i * 4] + 1 * input[3 + i * 4];
    }
    for (unsigned int i: Hex) {
        cout << hex << i;
    }
    cout << endl;
}

void print(const string &name, bool input[64]) {
    cout << name << ":";
    show_in_hex(input);
}