//#include <master.h>

#include <vector>

#include <random>
#include <chrono>

#include <windows.h>

#include <string>
#include <sstream>

#include <fstream>


#define PAU cin.get();

using namespace std;


vector<double> v_time, v_i, v_m, v_s, v_t, v_v, v_mean;
vector<double> d_time, di, dm, ds, dt, dv;
vector<double> w_time, w_i, w_m, w_s, w_t, w_v, ws_s;
double m_s;
vector<double> random_, random_w, random_ws;

vector<vector<double>> v_coeff_w_plus;
vector<vector<double>> v_coeff_w_minus;
vector<double> variance_plus;
vector<double> variance_minus;


class Randouble {
public:
	Randouble(double low = 0.0, double high = 1.0, unsigned s = chrono::system_clock::now().time_since_epoch().count()) :
		re(s),
		dist(low, high) {};
	double operator()() { return dist(re); };
private:
	default_random_engine re;
	uniform_real_distribution<double> dist;
};

double randouble(double low = 0.0, double high = 1.0, unsigned s = chrono::system_clock::now().time_since_epoch().count()) {
	static default_random_engine re(s);
	/*static*/ uniform_real_distribution<double> dist(low, high);
	return dist(re);
};

inline double SQR(double d) { return d*d; };

template<typename T>
double media_v(vector<T> &vec) {
	int N = vec.size();
	double ret = 0.;
	for(T elem : vec) ret += elem;
	ret /= N;
	return ret;
};

template<typename T>
double varianza(vector<T> &vec) {
	int N = vec.size();
	double ret = 0., med = media_v(vec);
	for(T elem : vec) ret += SQR(elem - med);
	ret /= N;
	return ret;
};



void read_random() {
	ifstream ifi_r("random.txt");
	double r;
	while(ifi_r >> r)random_.push_back(r);
}

void wavelet_pass_minus(vector<double> v) {
	int size = v.size();
	if(size <= 2) return;
	vector<double> part, part2;
	for(int i = 0; i < size + 1; i += 2) {
		double m = (v[i] + v[i + 1]) / 2.; 
		double d = (v[i] - v[i + 1]) / 2.;
		part.push_back(m);
		part2.push_back(d);
	};
	v_coeff_w_plus.push_back(part);
	v_coeff_w_minus.push_back(part2);

	wavelet_pass_minus(part);
};

void calcola_wavelet_minus(vector<double> v, string stringa) {
	ostringstream ostring;
	ostring << "wavelet_" << stringa << ".txt";
	int size = v.size(); 

	v_coeff_w_plus.clear();
	v_coeff_w_minus.clear();
	variance_plus.clear();
	variance_minus.clear();

	vector<double> first;
	for(int i = 0; i < size + 1; i += 2) {
		double m = (v[i] + v[i + 1]) / 2.;
		first.push_back(m);
	};

	wavelet_pass_minus(first);


	for(size_t i = 0; i < v_coeff_w_plus.size(); i++)variance_plus.push_back(varianza(v_coeff_w_plus[i]));

	for(size_t i = 0; i < v_coeff_w_minus.size(); i++)variance_minus.push_back(varianza(v_coeff_w_minus[i]));
	
	//ofstream ofi_coeff_p("coeff_plus.txt");

	//for(int i = 0; i < v_coeff_w_plus.size(); i++) {
	//	for(int j = 0; j < v_coeff_w_plus[i].size(); j++)ofi_coeff_p << v_coeff_w_plus[i][j] << " ";
	//	ofi_coeff_p << endl;
	//}

	ostringstream ostring3;
	ostring3 << "wavelet_" << stringa << "coeff.txt";
	ofstream ofi(ostring3.str());
	for(size_t i = 0; i < variance_plus.size(); i++)ofi << v_coeff_w_plus[i].size() << "\t" << variance_plus[i] << "\t" << variance_minus[i] << endl;
	ofi.close();

};

void read_serial_dati() {
	ifstream ifi("serial.TXT");
	double ws;
	while(ifi.good()) {
		ifi >> ws;
		v_s.push_back(ws);
		//cout << ws << endl; PAU;
	}
}

void remove_mean_from_signal() {
	double Med = 0;
	for(size_t i = 0; i < v_s.size(); i++) {
		Med += v_s[i];
	}
	m_s = Med / double(v_s.size());
	for(auto &v : v_s)v -= m_s;
}



int main() {

	read_serial_dati();
	remove_mean_from_signal();

	ofstream ofi("random.txt");
	//for(size_t i = 0; i < 14096; i++)randouble(0, 1.);
	for(size_t i = 0; i < v_s.size(); i++)ofi << randouble(-1., 1.) << endl;
	ofi.close();
	read_random();

	//double somma = 0;
	//for(auto v : random) {
	//	somma += v*v;
	//}

	//cout << somma << endl;

	//somma /= double(random.size());

	//cout << somma << endl;

	//for(auto &v : random) v /= sqrt(somma);

	//somma = 0;
	//for(auto v : random) {
	//	somma += v*v;
	//}
	//cout << somma << endl; //PAU;


	calcola_wavelet_minus(v_s,"metas_");

	calcola_wavelet_minus(random_, "random_");

	return 0;
}