#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::to_string;
using std::setw;
using std::left;

class Person{
	private:
		int status;
	public:
		Person(){ // default constructor
			status = 0;
		}

		string status_string(){
			if (status == 0){
				return "susceptible.";
			} else if (status > 0){
				return "sick (" + to_string(status) + " to go).";
			} else {
				return "recovered.";
			}
		}

		void infect(int n){
			status = n;
		}

		bool is_stable(){
			if (status == -1 || status == -2){
				return true;
			} else {
				return false;
			}
		}

		void update(){
			if (status > 0){
				status--;
				if (status == 0){
					status = -1;
				}
			}
		}
		
		void inoculate(){
			status = -2;
		}

		int get_status(){
			return status;
		}
};

class Population {
	private:
		float transfer_prob;
		int num_interactions;
		vector<Person> population;
	public:
		Population(int n, int interactions){
			for (int i = 0; i < n; i++){
				population.push_back(Person());
			}

			num_interactions = interactions;
		}

		void infect(){
			int val = rand() % population.size();
			population.at(val).infect(5);
		}

		void spread(){
			int temp = num_interactions;		
			for (int i = 0; i < population.size(); i++){
				if (population.at(i).get_status() != 0){
					continue;
				}
				while (temp > 0){
					int p = rand() % population.size();
					if (i == p){
						continue;
					}
					if (population.at(p).get_status() > 0){
						float r = ((rand() % 100) + 1) / 100.;
						if (r <= transfer_prob){
							population.at(i).infect(5);
							break;
						}
					}
					temp--;
				}
				temp = num_interactions;
			}
		}

		void update(){
			for (int i = 0; i < population.size(); i++){
				population.at(i).update();
			}
		}

		void set_transfer_prob(float p){
			transfer_prob = p;
		}

		void inoculate(float p){
			int n = population.size() * p/100;
			for (int i = 0; i < n; i++){
				float r = rand() % population.size();
				population.at(r).inoculate();
			}
		}
		
		void get_status(int &s, int &r, int &l, int &o){
			for (int i = 0; i < population.size(); i++){
				int c = population.at(i).get_status();
				if (c == 0){
					l++;
				} else if (c > 0){
					s++;
				} else if (c == -1){
					r++;
				} else {
					o++;
				}
			}
		}
};

int main(){
	int pop_size;
	float prob;	
	float inoc_rate;
	int interactions;

	cout << "Population size: ";
	cin >> pop_size;
	cout << "Probability of transfer: ";
	cin >> prob;
	cout << "Inoculation rate: ";
	cin >> inoc_rate;
	cout << "Daily interactions: ";
	cin >> interactions;
	cout << endl;
	
	int days = 1;

	Population p = Population(pop_size, interactions);		
	p.set_transfer_prob(prob);
	p.infect();	

	int c = 1;

	while (c != 0){
		p.spread();
		p.inoculate(inoc_rate);
		p.update();
		int s = 0, r = 0, l = 0, o = 0;
		p.get_status(s, r, l, o);
		cout << setw(4) << "Day " << setw(5) << left << days;
		cout << setw(13) << "Number sick: " << setw(8) << left << s;
		cout << setw(18) << "Number recovered: " << setw(8) << left << r;
		cout << setw(18) << "Number susceptible: " << setw(8) << left << l;
		cout << setw(18) << "Number inoculated: " << setw(8) << left << o << endl;
		days++;
		c = s;
	}		
	
	// cout << "\nThe virus ran its course in " << days - 1 << " days." << endl;

	return 0;
}

