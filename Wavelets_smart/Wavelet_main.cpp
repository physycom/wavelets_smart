//#include <master.h>

#include <vector>

#include <random>
#include <chrono>

#include <windows.h>

#include <string>
#include <sstream>

#include <fstream>


using namespace std;


vector<double> v_tempo, v_i, v_m, v_s, v_t, v_v, v_medio;
vector<double> d_tempo, di, dm, ds, dt, dv;
vector<double> w_tempo, w_i, w_m, w_s, w_t, w_v, ws_s;
double m_s;
vector<double> random_, random_w, random_ws;

vector<vector<double>> v_coeff_w_piu;
vector<vector<double>> v_coeff_w_meno;
vector<double> varianze_piu;
vector<double> varianze_meno;


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

template<typename T>
double varianza(vector<T> &vec) {
	int N = vec.size();
	double ret = 0., med = media_v(vec);
	for(T elem : vec) ret += SQR(elem - med);
	ret /= N;
	return ret;
};


//Daub4 WAVE;

void leggi_random() {
	ifstream ifi_r("random.txt");
	double r;
	while(ifi_r >> r)random_.push_back(r);
}

void wavelet_pass_meno(vector<double> v) {
	int size = v.size();
	if(size <= 2) return;
	vector<double> pezzo, pezzo2;
	for(int i = 0; i < size + 1; i += 2) {
		double m = (v[i] + v[i + 1]) / 2.; 
		double d = (v[i] - v[i + 1]) / 2.;
		pezzo.push_back(m);
		pezzo2.push_back(d);
	};
	v_coeff_w_piu.push_back(pezzo);
	v_coeff_w_meno.push_back(pezzo2);

	wavelet_pass_meno(pezzo);
};

void calcola_wavelet_meno(vector<double> v, string stringa) {
	ostringstream ostringa;
	ostringa << "wavelet_" << stringa << ".txt";
	int size = v.size(); 
	//cout << size << endl; PAU;

	v_coeff_w_piu.clear();
	v_coeff_w_meno.clear();
	varianze_piu.clear();
	varianze_meno.clear();

	vector<double> primo;
	for(int i = 0; i < size + 1; i += 2) {
		double m = (v[i] + v[i + 1]) / 2.;
		primo.push_back(m);
	};

	wavelet_pass_meno(primo);


	for(int i = 0; i < v_coeff_w_piu.size(); i++)varianze_piu.push_back(varianza(v_coeff_w_piu[i]));

	for(int i = 0; i < v_coeff_w_meno.size(); i++)varianze_meno.push_back(varianza(v_coeff_w_meno[i]));
	
	//ofstream ofi_coeff_p("coeff_piu.txt");

	//for(int i = 0; i < v_coeff_w_piu.size(); i++) {
	//	for(int j = 0; j < v_coeff_w_piu[i].size(); j++)ofi_coeff_p << v_coeff_w_piu[i][j] << " ";
	//	ofi_coeff_p << endl;
	//}

	ostringstream ostringa3;
	ostringa3 << "wavelet_" << stringa << "coeff.txt";
	ofstream ofi(ostringa3.str());
	for(int i = 0; i < varianze_piu.size(); i++)ofi << v_coeff_w_piu[i].size() << "\t" << varianze_piu[i] << "\t" << varianze_meno[i] << endl;
	ofi.close();

};

void leggi_smart_s_dati() {
	ifstream ifi("MS_serial.TXT");
	double ws;
	while(ifi.good()) {
		ifi >> ws;
		v_s.push_back(ws);
		//cout << ws << endl; PAU;
	}
}

void tolgo_media_da_segnale() {
	double Med = 0;
	for(int i = 0; i < v_s.size(); i++) {
		Med += v_s[i];
	}
	m_s = Med / double(v_s.size());
	for(auto &v : v_s)v -= m_s;
}



int main() {

	leggi_smart_s_dati();
	tolgo_media_da_segnale();

	ofstream ofi("random.txt");
	//for(int i = 0; i < 14096; i++)randouble(0, 1.);
	for(int i = 0; i < v_s.size(); i++)ofi << randouble(-1., 1.) << endl;
	ofi.close();
	leggi_random();

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


	calcola_wavelet_meno(v_s,"metas_");

	calcola_wavelet_meno(random_, "random_");

	return 0;
}