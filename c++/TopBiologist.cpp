#include <string>
#include <set>
#include <vector>
using namespace std;

class TopBiologist {
	public:
		vector <string> generate2tok(string seq) {
			vector <string> res;
			for (int i=0; i<seq.size()-2; i++) {
				string to = seq.substr(i, i+2);
				res.push_back(to);
			}
			return res;
		}
		bool checkTwo(string seq, string &ans) {
			string tokens[16] = {"AA", "AC", "AG", "AT", "CA", "CC", "CG", "CT",
						 	 "GA", "GC", "GG", "GT", "TA", "TC", "TG", "TT"};
			vector <string> tok2 = generate2tok(seq);
			sort(tok2.begin(), tok2.end());
			int ret = false;
			tok2.erase(unique(tok2.begin(), tok2.end()), tok2.end());
			for (int i=0; i<tok2.size(); i++) {
				if (tok2[i] != tokens[i]) {
					ans = tokens[i];
					ret = true;
					break;
				}
			}
			return ret;
		}
		bool checkOne(string seq, string &ans) {
			bool out[4] = {false, false, false, false};
			for (int i=0; i<seq.size(); i++) {
				switch(seq[i]) {
				case 'A':
					out[0] = true;
					break;
				case 'C':
					out[1] = true;
					break;
				case 'G':
					out[2] = true;
					break;
				case 'T':
					out[3] = true;
					break;
				}
			}
			
			for (int i=0; i<4; i++) {
				if (!out[i]) {
				switch(i){
				case 0: ans = "A"; return true;
				case 1: ans = "C"; return true;
				case 2: ans = "G"; return true;
				case 3: ans = "T"; return true;
				}
				}
			}
			return false;
		}
		
		string findShortestNewSequence(string seq) {
			int l1 = 4, l2 = 16, l3 = 64, l4 = 256;
			string ans;
			if (checkOne(seq, ans)) {
				return ans;
			}
			if (checkTwo(seq, ans)) {
				return ans;
			}
			return "AA";
		}
};
