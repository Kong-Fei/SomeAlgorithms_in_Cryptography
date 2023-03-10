#include<iostream>
#include<vector>
using namespace std;
// 实现shamir秘密分割门限方案(c++);
// Made by RuanGaofei;

bool find(vector<long long> const sk, int num) {
	// 判断num是否已经是重复系数;
	bool res = false;
	int sk_size = sk.size();
	for (int i = 0; i < sk_size; i++) {
		if (num == sk[i]) {
			res = true;
			break;
		}
	}
	return res;
}

long long quickMi(long long m, int n, int p) {
	// 快速幂算法;
	long long sum = 1;
	m = m % p;
	while (n > 0) {
		if (n % 2 == 1) sum = (sum * m) % p;
		n = n / 2;
		m = (m * m) % p;
	}
	if (sum < 0) sum += p;
	return sum;
}

long long f(long long x,vector<int>const a, int s,int q) {
	// 多项式f(x);
	long long res = s;
	int a_size = a.size();
	for (int i = 0; i < a_size; i++) {
		res = (res + a[i] * quickMi(x, i + 1, q) % q) % q;
	}
	return res;
}

long long oneMultiply(int locat_i, vector<long long>sk,int q,int k) {
	// 计算第locat_i个连乘项;
	long long res;
	long long fenzi=1, fenmu = 1;
	for (int i = 0; i < k; i++) {
		if (i == locat_i) continue;
		fenzi = fenzi * sk[i] % q;
		long long temp = (sk[locat_i] - sk[i]);
		fenmu = fenmu * temp % q;
	}
	res = fenzi * quickMi(fenmu, q - 2, q) % q;
	return res;
}
int main() {
	// 保证命令窗口(cmd)使用utf-8编码格式
    system("chcp 65001");
	cout << "-----------------------------秘密产生-----------------------" << endl<<endl;
	int n, k;
	cout << "请按顺序输入秘密分配参与人数n和门限值k：";
	cin >> n >> k;
	int q;
	cout << "请输入一个大素数q，保证q>=n+1: q= ";
	cin >> q;
	// 产生s和k-1个系数;
	srand(time(NULL));
	int s = rand() % q;
	cout << "秘密s为：s=" << s << endl;
	vector<int> a(k - 1);
	cout << "选取的k-1个系数为：";
	for (int i = 0; i < k - 1; i++) {
		a[i] = rand() % q;
		cout << "a" << i + 1 <<"="<< a[i]<<",";
	}
	cout << endl << endl;

	cout << "-----------------------------子密钥分配-----------------------" << endl << endl;
	// 产生n个不同的子密钥;
	vector<long long> sk;
	vector<long long> fsk;
	cout <<"分配的"<<n<< "个子密钥为：";
	for (int i = 0; i < n; i++) {
		long long num = rand() % q;
		while (find(sk, num)) {
			num = rand() % q;
		}
		sk.push_back(num);
		fsk.push_back(f(num, a, s, q));
		cout << "(" << sk[i] << "," << fsk[i] << "),";
	}
	cout << endl << endl;

	cout << "-----------------------------秘密恢复-----------------------" << endl << endl;
	// 使用前k个子密钥恢复s;
	// 计算每一个插入点的乘积项;
	long long re_s = 0;
	cout << "使用的"<<k<<"个子密钥为：";
	for (int i = 0; i < k; i++) {
		cout << "(" << sk[i] << "," << fsk[i] << "),";
		re_s = (re_s  + (fsk[i]  * oneMultiply(i, sk, q,k))%q )% q;
	}
	cout << endl << endl;
	if ((k - 1) % 2 == 1) re_s = re_s * (long long(q) - 1) % q;
	cout << "恢复出的秘密为：re_s= " << re_s << endl<<endl;
	cout << "-----------------------------程序结束-----------------------" << endl << endl;
}
