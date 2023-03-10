#include<iostream>
#include<cstdlib>
#include<algorithm>
#include<vector>
#include<string>
#include<math.h>
#include<stdlib.h>
#include<ctime>
#include<sys/timeb.h>
using namespace std;


bool oddJudge(int p) {
    // 素性检验;
    bool res=true;
    if(p<10000) return false;
    for (int i = 2; i <= sqrt(p); i++) {
        if (p % i == 0) {
            res = false;
            break;
        }
    }
    return res;
}
vector<int> creatOddNum() {
    // 产生质数p,q;
	vector<int> pq(2);
    pq[0] = rand();
    pq[1] = rand();

	while (!(oddJudge(pq[0]) && oddJudge(pq[1]))) {
        pq[0] = rand();
        pq[1] = rand();
	}
	return pq;
}
int gcd(int a, int b, int& x, int &y) {
    // 利用欧几里得算法求最大公约数;
    if (a < b) swap(a, b);
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int ans = gcd(b, a % b,x,y);
    int temp = x;
    x = y;
    y = temp - a / b * y;
    return ans;
}

long long quickPow(long long m, int e, int n) {
    // 快速幂算法(m*m)%n=(m%n*m%n)%n;
   long long sum = 1;
    m = m % n;
    while (e > 0) {
        if (e % 2 == 1) sum = (sum * m) % n;
        e = e / 2;
        m=  (m*m) % n; // 合并m,减小m的规模;
    }
    if (sum < 0) sum = sum + n;
    return sum;
}

int main() {
    // 保证命令窗口(cmd)使用utf-8编码格式
    system("chcp 65001");
    cout << "--------------------密钥产生----------------------------" << endl;
    // 随机种子
    timeb time_0,time_creat_key,time_input,time_encode,time_decode;
    ftime(&time_0);
    srand((unsigned int)time(0));
    vector<int> pq(2);
    pq = creatOddNum();
    int p = pq[0],q = pq[1];
    cout << " 质数p=  "<<pq[0] <<"   " <<"质数q= "<< pq[1]<<endl;
    
    
    int n = p * q;
    int phi_n = (p - 1) * (q - 1); // n的欧拉函数值;
    // 获得公钥e;
    int e = rand()%phi_n;
    int x, y;
    while (gcd(phi_n, e,x,y) != 1) e = rand() % phi_n;
    cout << " 公钥e= " << e << " 公钥n= " << n<<endl;
    int d = y % phi_n;
    if (d < 0) d = d + phi_n;
    cout << " 私钥d= " << d << " 私钥n= " << n << endl<<endl;

    ftime(&time_creat_key);
    cout << " 产生公私密钥花费时间为(ms): "<< (time_creat_key.millitm-time_0.millitm) << endl;
    cout << "---------------------加   密--------------------------" << endl;
    string s;
    cout << "请输入明文：";
    getline(cin, s);
    ftime(&time_input);
    int s_len = s.size();
    vector<int> m(s_len);
    vector<long long> encode_m(s_len);
    cout << "密文为：";
    for (int i = 0; i < s_len; i++){
        m[i] = s[i];
        encode_m[i] = quickPow(m[i], e, n);
        // encode_m = m^e % n;
        cout << encode_m[i];
    }
    ftime(&time_encode);
    cout << endl;

    cout << " 加密花费时间为(ms): " << (time_encode.millitm - time_input.millitm) <<endl;


    cout << "--------------------解   密----------------------------" << endl;
    cout << "解密结果为：";
    for (int i = 0; i < s_len; i++) {
        long long outcode_m = quickPow(encode_m[i], d, n);
        cout  << char(outcode_m);
    }
    ftime(&time_decode);
    cout << endl;
    cout << " 解密花费时间为(ms): " << (time_decode.millitm - time_encode.millitm) <<endl;
    cout << "--------------------程序结束--------------------------" << endl << endl;
    cin.get();
}
