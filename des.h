//
// Created by Jimmy on 2023/3/23.
//

#ifndef DES_DES_H

#include "string"
#include "iostream"

#define DES_DES_H
using namespace std;

class key {
public:
    bool bit_key[64];     //原始密钥
    bool round_key[16][48]; //保留16轮的子密钥，每个密钥48位
    explicit key(string input_key);
};

string input_information(); //输入
void char2bit(bool *output, string &input);  //转换为二进制函数
void transform(bool *output, bool *input, const int *table, int tablelen);  //置换函数
void shift_left(bool *input, int len);   //左移
void des_encrypt(string &text, key des_key); //总体des加密函数
void s_func(bool *output, const bool *input); //S盒置换加密
void cipher_func(bool *target, bool *key);  //根据子密钥和部分明文进行S盒、P盒置换加密
void xor_bit(bool *input1, const bool *input2, int len);  //异或运算，结果保留在第一个input参数中
void show_in_hex(const bool input[64]);     //以十六进制展示

#endif //DES_DES_H

