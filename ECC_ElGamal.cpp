#include<iostream>
#include<vector>
using namespace std;
// ECC_ElGame加密算法实现(C++);
// Made by RuanGaofei;
long long quickMi(long long m, int n, int p) {
	// 利用快速幂判断二次剩余m^n%p;
	m = m % p;
	long long sum = 1;
	while (n > 0) {
		if (n % 2 == 1) sum = (sum * m) % p;
		n = n / 2;
		m = (m * m) % p;
	}
	return sum;
}
vector<long long> add(long long Px, long long Py, long long Qx, long long Qy, int a, int p) {
	// 定义交换群Ep上的加法;
	vector<long long> res(2);
	if (Py == p) {
		res[0] = Qx;
		res[1] = Qy;
		return res;
	}
	if (Qy == p) {
		res[0] = Px;
		res[1] = Py;
		return res;
	}
	if (Py + Qy == p && Px == Qx) {
		res[0] = Px;
		res[1] = p; // 无穷远点;
		return res;
	}
	long long lambda;
	if (Px == Qx && Py == Qy) {
		// 同点->切线;
		lambda = ((3 * long long(pow(Px, 2)) + a) % p) * (quickMi(2 * Py, p - 2, p) % p) % p;
	}
	else {
		// 不同点;
		long long fenzi = (Qy - Py) % p, temp = Qx - Px;
		if (fenzi < 0) fenzi += p;
		if (temp < 0) temp += p;
		long long fenmu = quickMi(temp, p - 2, p) % p;
		lambda = (fenzi * fenmu)% p;
	}
	res[0] = (int(pow(lambda, 2)) - Px - Qx) % p;
	res[1] = (lambda*(Px-res[0])-Py) % p;
	if (res[0] < 0) res[0] += p;
	if (res[1] < 0) res[1] += p;
	return res;
}
// 主函数部分;
int main() {
	cout << "--------------------密钥产生----------------------------" << endl<<endl;
	int a,b,p;
	cout << "请输入一个素数p,两个整数a,b（a,b<p)" << endl;
	cin >> p >> a >> b;
	if (4 * pow(a, 3) + 27 * pow(b, 2) == 0) {
		cout << "4a^3+2b^2=0,请重新输入a,b" << endl;
		cin >> a >> b;
	}
	// 计算Ep(a,b);
	vector<int> Ex;
	vector<int> Ey;
	cout << "Abel群Ep点对为：{";
	for (int x = 0; x <= p - 1; x++) {
		long long xx = (int(pow(x, 3)) + a * x+ b) % p;
		if (quickMi(xx, (p - 1) / 2, p)==1) {
			Ex.push_back(x);
			for (int y = 0; y <= p/2; y++) {
				if (int(pow(y, 2)) % p == xx) {
					Ey.push_back(y);
					cout << "(" << x<<","<<y<<"), ";
					Ey.push_back(p - y);
					cout << "(" << x << "," << p-y << "), ";
				}
			}
		}
	}
	cout <<"}" <<endl<<endl;
	// 生成元G;
	long long Gx = Ex[0],Gy = Ey[1];
	cout <<"选取生成元：G=(" << Gx << "," << Gy << ")" << endl;
	int nA;
	cout << "请选取一个正整数作为你的私钥：nA= ";
	cin>>nA;
	// 计算公钥PA;
	vector<long long> PA(2);
	PA[0] = Gx, PA[1] = Gy;
	for (int i = 2; i <= nA; i++)
		PA = add(PA[0], PA[1], Gx, Gy, a, p);
	cout << "你的公钥为：PA=(" << PA[0] << "," << PA[1] << ")" << endl;

	cout << "--------------------消息加密----------------------------" << endl<<endl;
	int m;
	int k1 = 30;
	int m_max = p / k1;
	cout << "输入一个小于"<<m_max<<"的要加密的消息(数字)：m= ";
	cin >> m;
	// 消息嵌入;
	long long Pm_x, Pm_y;
	bool flag_Pm = 0;
	for (int i = 0; i < k1; i++) {
		if (flag_Pm == 1) break;
		int x = m * k1 + i;
		long long xx = (long long(pow(x, 3)) % p + a * x % p + b % p) % p;
		if (quickMi(xx, (p - 1) / 2, p) == 1) {
			// 找到嵌入点;
			for (int y = 0; y <= p / 2; y++) {
				if (int(pow(y, 2)) % p == xx) {
					Pm_x = x;
					Pm_y = y;
					flag_Pm = 1;
					cout << "选择k1=30，消息嵌入点Pm=(" << Pm_x << "," << Pm_y << ")" << endl;
					break;
				}
			}
		}
	}
	// B选择k2，使用A的公钥加密;
	int k2;
	cout << "请选择一个k2（任意正整数即可)用于加密消息：k2=";
	cin >> k2;
	// 计算kG;
	vector<long long> kG(2);
	kG[0] = Gx, kG[1] = Gy;
	for (int i = 2; i <= k2; i++)
		kG= add(kG[0], kG[1], Gx, Gy, a, p);
	// 计算Pm+kPA;
	vector<long long> kPA(2);
	kPA[0] = PA[0], kPA[1] = PA[1];
	for (int i = 2; i <= k2; i++)
		kPA = add(kPA[0], kPA[1], PA[0], PA[1], a, p);
	vector<long long> Pm_kPA(2);
	Pm_kPA = add(Pm_x, Pm_y, kPA[0], kPA[1], a, p);
	cout << "加密结果：{" << "(" << kG[0] << "," << kG[1] << "),(" << Pm_kPA[0] << "," << Pm_kPA[1] << ")}" << endl;

	cout << "--------------------消息解密----------------------------" << endl<<endl;
	// 利用第二个点减去第一个点的nA倍,恢复Pm;
	vector<long long> nA_kG(2);
	nA_kG[0] = kG[0];
	nA_kG[1] = kG[1];
	for (int i = 2; i <= nA; i++)
		nA_kG = add(nA_kG[0], nA_kG[1], kG[0], kG[1], a, p);
	// 求-nA_kG;
	nA_kG[1] = p - nA_kG[1];
	vector<long long> rePm(2);
	rePm = add(nA_kG[0], nA_kG[1], Pm_kPA[0], Pm_kPA[1], a, p);
	cout << "解密的消息嵌入点：rePm=(" << rePm[0] << "," << rePm[1] << ")" << endl;
	int rem = rePm[0] / k1;
	cout << "解密的消息rem：rem=" << rem << endl;
	cout << "--------------------程序结束----------------------------" << endl;
}
