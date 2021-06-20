#include<iostream>
#include<functional>
#include<string>

using namespace std;
//
//
long long quickMi(long long m, long long n, long long p) {
	// 快速幂算法;
	long long sum = 1;
	m = m % p;
	while (n > 0) {
		if (n % 2 == 1) sum = sum * m % p;
		n = n / 2;
		m = m * m % p;
	}
	if (sum < 0) sum += p;
	return sum;
}

bool judgeOriginal(long long num, long long p) {
	// 判断 num是否为p的本原根;
	bool res = true;
	for (long long i = 2; i <= p - 2; i++) {
		if (quickMi(num, i, p) == 1) {
			res = false;
			break;
		}
	}
	return res;
}
long long gcd(long long a, long long b, long long& x, long long& y) {
	// 扩展欧几里得算法求最大公约数;
	if (a < b) swap(a, b);
	if (b == 0) {
		x = 1;
		y = 0;
		return a;
	}
	else {
		long long ans = gcd(b, a % b,x,y);
		long long temp = x;
		x = y;
		y = temp - a / b * y;
		return ans;
	}
}

int main() {
	cout << "-----------------------------参数&密钥产生-----------------------" << endl << endl;
	long long p;
	cout << "请输入一个素数(>=7)：p= ";
	cin >> p;
	// 产生生成元g;
	long long g=-1;
	for (long long i = 2; i < p; i++) {
		if (judgeOriginal(i, p)) {
			g = i;
			break;
		}
	}
	cout << "(mod p)的乘法群生成元：g= " << g << endl;
	// 密钥和私钥产生;
	srand(time(NULL));
	long long sk = rand() % p;
	long long pk = quickMi(g, sk, p);
	cout << "你的私钥为：SK= " << sk << "; 你的公钥为：PK= " << pk << endl << endl;;

	cout << "-----------------------------消息签名-----------------------" << endl << endl;
	// 消息Hash映射;
	cout << "请入任意一则消息m：";
	// 清空cin输入流;
	string temp = "\n";
	getline(cin, temp);
	// 输入消息;
	string m;
	getline(cin, m);
	hash<string> m_hash;
	long long Hm = m_hash(m);
	cout << "消息m的哈希值为：H(m)=" << Hm << endl;
	// 产生z_(p-1)上的随机数;
	long long k=rand()%(p-1);
	long long x, y;
	while (gcd(p - 1, k,x,y) != 1) k = rand() % (p - 1);
	long long inv_k = y % p;
	cout << "在(mod p-1)的乘法群上选取的随机数k为：k= " << k << endl;
	// 产生签名;
	long long r = quickMi(g, k, p);
	long long s = (Hm - sk * r) * inv_k % (p - 1);
	if (s < 0) s += (p-1);
	cout << "你在消息m上的数字签名为：(" << r << "," << s << ")" << endl<<endl;

	cout << "-----------------------------签名验证-----------------------" << endl << endl;
	if (quickMi(pk, r, p) * quickMi(r, s, p) % p == quickMi(g, Hm, p)) {
		cout << "消息m是由你产生的，验证成功" << endl << endl;
	}
	else cout << "验证失败" << endl<<endl;
	cout << "-----------------------------程序结束-----------------------" << endl << endl;
}
