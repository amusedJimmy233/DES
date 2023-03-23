# DES加密算法
### 用C++语言实现经典密码算法DES

1）明文输入为字符串，密文输出以16进制表示；

2）程序应显示出中间结果，均以16进制显示，包括：每一明文分组、密钥、初始置换IP结果、16轮迭代中每一轮的结果、逆初始置换IP-1结果。

### 结果验证：

在网站https://paginas.fe.up.pt/~ei10109/ca/des.html 中验证正确。

### 特别鸣谢：

> 代码大部分来自https://blog.csdn.net/Joliph/article/details/80754661 

本仓库做出的修改有：

- 实现整行输入，自动将大于8个字节的明文分组进行加密
- 对加密的中间过程做输出进行可视化
- 进行一些客制化修改

