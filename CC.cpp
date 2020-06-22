#include<numeric>
#include "C:\Users\vhemka\source\repos\usefuls.cpp"


class CCobj
{

public:
	vector<vector<int>> CC_im;
	int numCC;
	vector<vector<int>> PILs;
};
//
//
//
class fp_result
{
public:
	vector<vector<int>> CC1;
	vector<vector<int>> equlist;

};
class sp_result
{
public:
	vector<vector<int>> CC;
	vector<vector<int>> PIL;

};
//
//
//
vector<vector<int>> CCsecond_pass(fp_result fp_res) {
	vector<vector<int>> CC1 = fp_res.CC1;
	vector<vector<int>> equlist = fp_res.equlist;
	vector<vector<int>> CC = CC1;
	for (int i = 0; i < equlist.size(); i++) {

		vector<int> equ_labels = equlist[i];
		for (int il = 0; il < equ_labels.size(); il++)
		{
			int lbl = equ_labels[il];

			for (int ir = 0; ir < CC.size(); ir++) {
				for (int ic = 0; ic < CC[0].size(); ic++) {

					if (CC1[ir][ic] == lbl) {
						CC[ir][ic] = i + 1;
					}

				}
			}



		}


	}
	return CC;


};
sp_result CCsecond_pass_withPIL(fp_result fp_res) {

	vector<vector<int>> CC1 = fp_res.CC1;
	vector<vector<int>> equlist = fp_res.equlist;
	vector<vector<int>> CC = CC1;
	int flat_i;
	int last_lbl;
	for (int i = 0; i < equlist.size(); i++) {

		vector<int> equ_labels = equlist[i];
		for (int il = 0; il < equ_labels.size(); il++)
		{
			int lbl = equ_labels[il];
			
			for (int ir = 0; ir < CC.size(); ir++) {
				for (int ic = 0; ic < CC[0].size(); ic++) {

					if (CC1[ir][ic] == lbl) {
						CC[ir][ic] = i + 1;
						last_lbl = i + 1;
					}

				}
			}



		}


	}

	///PIL part :
	vector<vector<int>> PIL;
	int lbl;
	for (int lbl = 1; lbl <= last_lbl; lbl++) {
		PIL.push_back({});//initialize an empty row in PIL for all labels
	}


	for (int ir = 0; ir < CC.size(); ir++) {
		for (int ic = 0; ic < CC[0].size(); ic++) {
			if (CC[ir][ic] >0) {
				lbl = CC[ir][ic];
				flat_i = sub2ind1(ir, ic, CC[0].size());
				PIL[lbl - 1].push_back(flat_i);
			}
		}
	}
	
	sp_result res;
	res.CC = CC;
	res.PIL = PIL;
	return res;


};
//

//
int scan_rows_of_equlist(vector<vector<int>> equlist, int label) {

	for (int i = 0; i < equlist.size(); i++) {
		vector<int> equrow = equlist[i];
		if (isinvec(equrow, label))
			return i;
	}
	return -1; //label not in equlist yet

};
//
//
////4 neigh cc
fp_result ccfirstpass(vector<vector<int>> bw)
{
	int nr = bw.size();
	int nc = bw[0].size();
	//vector<vector<int>> nbw;
	vector<vector<int>> equlist;
	int w;
	int n;
	int curr_label = 1;
	for (int ir = 0; ir < nr; ir++) {

		for (int ic = 0; ic < nc; ic++) {

			if (bw[ir][ic] == 1) {


				//look at the causal neighbors:

				if (ic > 0)
					w = bw[ir][ic - 1];
				else
					w = 0;

				if (ir > 0)
					n = bw[ir - 1][ic];
				else
					n = 0;
				/////////////////////////////
				if ((n == 0) && (w == 0)) {
					bw[ir][ic] = curr_label;
					equlist.push_back({ curr_label });
					curr_label = curr_label + 1;

					//	cout << "ch1";
				}
				else if ((n == 0) && (w > 0)) {
					bw[ir][ic] = w;
					//	cout << "ch2";
				}
				else if ((w == 0) && (n > 0))
				{
					bw[ir][ic] = n;
					//	cout << "ch3";
				}
				else if (n == w) {
					bw[ir][ic] = n;
					//	cout << "ch4";
				}
				else if ((n > 0) && (w > 0) && (n != w)) {
					int smaller = min(n, w);
					int bigger = max(n, w);
					bw[ir][ic] = bigger;
					//cout << "ch5";
					int equind_s = scan_rows_of_equlist(equlist, smaller);
					int equind_b = scan_rows_of_equlist(equlist, bigger);
					if ((equind_s == -1) && (equind_b == -1)) {
						equlist.push_back({ smaller,bigger });
				//		cout << "to equlist: {" << smaller << ',' << bigger << "}";
					}
					else if ((equind_s > -1) && (equind_b == -1)) {
						equlist[equind_s].push_back(bigger);
				//		cout << "equlist:\n";
				//		display_2dvec(equlist);
				//		cout << "\n";

					}
					else if ((equind_b > -1) && (equind_s == -1)) {
						equlist[equind_b].push_back(smaller);
					//	cout << "equlist:\n";
					//	display_2dvec(equlist);
					//	cout << "\n";
					}
					else if ((equind_b > -1) && (equind_s > -1) && (equind_b != equind_s)) {
						vector<int> b_equs = equlist[equind_b];
						for (int i = 0; i < b_equs.size(); i++) {
							equlist[equind_s].push_back(b_equs[i]);
						}
						// erase the equind_bth element
						equlist.erase(equlist.begin() + equind_b);
					//	cout << "equlist:\n";
					//	display_2dvec(equlist);
					//	cout << "\n";
					}
				}

			}

		}

	}
//	cout << "equlist:\n";
//	display_2dvec(equlist);
	fp_result fp_res;
	fp_res.CC1 = bw;
	fp_res.equlist = equlist;
	return fp_res;

}


fp_result ccfirstpass8(vector<vector<int>> bw)
{
	int nr = bw.size();
	int nc = bw[0].size();
	//vector<vector<int>> nbw;
	vector<vector<int>> equlist;
	int w = 0;
	int n = 0;
	int nw = 0;
	int ne = 0;
	int curr_label;
	curr_label = 1;
	for (int ir = 0; ir < nr; ir++) {

		for (int ic = 0; ic < nc; ic++) {

			if (bw[ir][ic] >0) {


				//look at the causal neighbors:

				if (ic > 0)
					w = bw[ir][ic - 1];
				else
					w = 0;

				if (ir > 0)
					n = bw[ir - 1][ic];
				else
					n = 0;

				if ((ic > 0) && (ir > 0))
					nw = bw[ir - 1][ic - 1];
				else
					nw = 0;

				if ((ic < (nc - 1)) && (ir > 0))
					ne = bw[ir - 1][ic + 1];
				else
					ne = 0;
				///if all neighs zero, give a new label
				if ((n == 0) && (w == 0) && (nw == 0) && (ne == 0)) {
					bw[ir][ic] = curr_label;

					equlist.push_back({ curr_label });
					curr_label = curr_label + 1;
				//	cout << "cond1\n"<<curr_label;
				}
				//if only one of them nonzero, assign that label

				else if ((n == 0) && (w == 0) && (nw == 0) && (ne > 0)) {
					bw[ir][ic] = ne;
					//	cout << "cond2\n";
				}
				else if ((n == 0) && (w == 0) && (nw > 0) && (ne == 0)) {
					bw[ir][ic] = nw;
					//		cout << "cond2\n";
				}
				else if ((n == 0) && (w > 0) && (nw == 0) && (ne == 0)) {
					bw[ir][ic] = w;
					//		cout << "cond2\n";
				}
				else if ((n > 0) && (w == 0) && (nw == 0) && (ne == 0)) {
					bw[ir][ic] = n;
					//	cout << "cond2\n";
				}
				//if more than one non-zeros,
				else {
					
					vector<int> neighs = { n,w,nw,ne };
					vector<int> nonzero_inds;
					vector<int> nonzero_vals;
					int nnonzero = 0;
					for (int i = 0; i < 4; i++) {
						if (neighs[i] > 0){
							nonzero_inds.push_back(i);
							nonzero_vals.push_back(neighs[i]);
							nnonzero += 1;
						}
					}
					bool all_same = 1;
					int prev_val = nonzero_vals[0];
					for (int i = 1; i < nnonzero; i++) {
						int curr_val = nonzero_vals[i];
						if (curr_val != prev_val){
							all_same = 0;
							break;
						}
						prev_val = curr_val;
					}
					//if more than one non-zeros, but all are same, assign that label :
					if (all_same){
						bw[ir][ic] = nonzero_vals[0];
						//	cout << "cond3_1\n";
					}
					else { // if more than one non-zeros and they are not all the same
						//	cout << "cond3_2\n";
						vector<int> ei_nonzeros;
						for (int i = 0; i < nnonzero; i++) {
							int nonzero_val = nonzero_vals[i];
							int ei_nz = scan_rows_of_equlist(equlist, nonzero_val);
							ei_nonzeros.push_back(ei_nz);
							
						}


						//int sum_ei_nz = accumulate(ei_nonzeros.begin(), ei_nonzeros.end(), 0);
						int num_not_in_equ = count_in_vec(ei_nonzeros, -1);
						//	cout << "num_not_in_equ"<< num_not_in_equ <<"\n";
						//	cout << "nnonzero" << nnonzero << "\n";
						if (num_not_in_equ == nnonzero) { //if none of the neighbors in equlist yet,
							equlist.push_back(nonzero_vals);
						}
						else if (num_not_in_equ == (nnonzero - 1)) {
							//one of the neighs already in equlist>>add the others to its line 
							vector<int> inequ_inds = fetch_greater_than(ei_nonzeros, -1);
							int itsline_ind = inequ_inds[0];
							vector<int> others_inds = find_in_vec(ei_nonzeros, -1);
							vector<int> others_vals = vector_indexing(nonzero_vals,others_inds);
							equlist[itsline_ind].insert(equlist[itsline_ind].end(),others_vals.begin(),others_vals.end());
						}
						else if (num_not_in_equ <= (nnonzero - 2)) {
							//		cout << "came to this hell";
					//			cout << "equlist:";
						//		display_2dvec(equlist);
							//two or more of the neighs already in equlist>>add the others to one of these and 
							// move (delete and append) the other equ member neighs lines to the one you added to.
					//			cout << "ei_nonzeros";
							//	display_1dvec(ei_nonzeros);
							vector<int> inequ_line_inds = fetch_greater_than(ei_nonzeros, -1); //has two or more elements
					//			cout << "inequ_line_inds";
						//		display_1dvec(inequ_line_inds);
							vector<int> u_inequ_line_inds = unique_elts(inequ_line_inds);
							//			cout << "u_inequ_line_inds";
						//		display_1dvec(u_inequ_line_inds);
							vector<int> outequ_inds = find_in_vec(ei_nonzeros, -1);
							//			cout << "outequ_inds";
						//		display_1dvec(outequ_inds);
							vector<int> outequ_vals = vector_indexing(nonzero_vals, outequ_inds);
							//		cout << "outequ_vals";
						//		display_1dvec(outequ_vals);
							//cout << "checking2";
							int smallest_equ_i = min_of_vec(u_inequ_line_inds); //this is the one to be kept
							int smallest_i2 = argmin_of_vec(inequ_line_inds);
							//		cout << "smallest_equ_i"<< smallest_equ_i<<"\n";
							//		cout << "nonzero_vals:";
						//		display_1dvec(nonzero_vals);
							bw[ir][ic] = nonzero_vals[smallest_i2];
			
							equlist[smallest_equ_i].insert(equlist[smallest_equ_i].end(), outequ_vals.begin(), outequ_vals.end());
							//			cout << "equlist after insert";
						//		display_2dvec(equlist);
							//cout << "inequ_line_inds";
						//	display_1dvec(u_inequ_line_inds);
							vector<int> other_inequ_lines = vec_diff(u_inequ_line_inds, { smallest_equ_i }); //1 or more elements
						//		cout << "other_inequ_lines";
						//		display_1dvec(other_inequ_lines);
							//cout << "checking3";
							for (int i = 0; i < other_inequ_lines.size(); i++) {
								vector<int> b_equs = equlist[other_inequ_lines[i]];
								for(int j=0;j<b_equs.size();j++){
									equlist[smallest_equ_i].push_back(b_equs[j]);
								}
							}
							//		cout << "equlist after for for pushback";
					//			display_2dvec(equlist);
							//cout << "checking4";
							///now all necessary info added to equlist[smallest_equ_i]; we need to get rid of other labels lines in equlist
							//remove the max first so that indices dont get mixed:
							sort(other_inequ_lines.begin(), other_inequ_lines.end()); //kucukten buyuge 
						//		cout << "other_inequ_lines after sorting";
							//	display_1dvec(other_inequ_lines);
																					  //cout << "checking4.5\n";
							//cout << "equlist:";
							//display_2dvec(equlist);
							//cout << "smallest_equ_i:" << smallest_equ_i << "\n";
							//cout << "other_inequ_lines.size():" << other_inequ_lines.size() << "\n";
							//cout << "other_inequ_lines:";
							//display_1dvec(other_inequ_lines);
							for (int k = (other_inequ_lines.size() - 1); k > -1; k--) {
								
								equlist.erase(equlist.begin() + other_inequ_lines[k]);
							}
							//cout << "checking5";

							//vector<int> b_equs = equlist[equind_b];
							//for (int i = 0; i < b_equs.size(); i++) {
							//	equlist[equind_s].push_back(b_equs[i]);
							//}
							//// erase the equind_bth element
							//equlist.erase(equlist.begin() + equind_b);

						}



					}



				}



			}

		}


	}
	//	cout << "equlist:\n";
	//	display_2dvec(equlist);
	fp_result fp_res;
	fp_res.CC1 = bw;
	fp_res.equlist = equlist;
	return fp_res;

}


CCobj CCA(vector<vector<int>> BW, string el_path, int connectivity)
{
	fp_result fp_res;
	sp_result sp_res;
	vector<vector<int>> CC_im;
	if (connectivity == 4){
		fp_res = ccfirstpass(BW);
		write_csv(fp_res.CC1, "CC1.txt");
		write_csv(fp_res.equlist, el_path);
		sp_res = CCsecond_pass_withPIL(fp_res);
	}
	else if (connectivity == 8) {
		fp_res = ccfirstpass8(BW);
		write_csv(fp_res.CC1, "CC1.txt");
		write_csv(fp_res.equlist, el_path);
		sp_res = CCsecond_pass_withPIL(fp_res);
	}
	int numCC = fp_res.equlist.size();
	CCobj CCo;
	CCo.CC_im = sp_res.CC;
	CCo.numCC = numCC;
	CCo.PILs = sp_res.PIL;
//	cout << "numCC=" << numCC;
	return CCo;
}

int** AllocMatrix(int m, int n)
{
	int** matrix, i, j;
	matrix = (int**)malloc(m * sizeof(int*));
	for (i = 0; i < m; i++)
	{
		matrix[i] = (int*)malloc(n * sizeof(int));
		for (j = 0; j < n; j++)
			matrix[i][j] = 0;
	}
	return matrix;
}

//
//int** C_CCA(int** BW, char el_path[],int w,int h) {
//
//	vector<vector<int>> vvBW;
//	for (int i = 0; i < w; i++) {
//		vvBW.push_back({});
//		for (int j = 0; j < h; j++) {
//			vvBW[i].push_back(BW[i][j]);
//		}
//	}
//
//
//
//	CCobj CCo = CCA(vvBW, "equlist.txt");
//	vector<vector<int>> CC_im = CCo.CC_im;
//	int** CC_imp = AllocMatrix(w, h);
//	for (int i = 0; i < w; i++) {
//		for (int j = 0; j < h; j++) {
//			CC_imp[i][j] = CC_im[i][j];
//		}
//	}
//
//
//
//	return CC_imp;
//}

extern "C" void C_CCA(int**,int,int,int**,int,int*,int**,int*);

void C_CCA(int** BW, int w,int h, int** CC_imp, int connectivity, int* pnum_objs, int** PixelIdxLists,int* PIL_lengths) {

	vector<vector<int>> vvBW;
	for (int i = 0; i < w; i++) {
		vvBW.push_back({});
		for (int j = 0; j < h; j++) {
			vvBW[i].push_back(BW[i][j]);
		}
	}



	CCobj CCo = CCA(vvBW, "equlist.txt",connectivity);

	vector<vector<int>> CC_im = CCo.CC_im;
	vector<vector<int>> PILs = CCo.PILs;
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			CC_imp[i][j] = CC_im[i][j];
		}
	}

	for (int i = 0; i < PILs.size(); i++) {
		PIL_lengths[i] = PILs[i].size();
		for (int j = 0; j < PILs[i].size(); j++) {
			PixelIdxLists[i][j] = PILs[i][j];
		}
	}

	*pnum_objs = CCo.numCC;

	//return CC_imp;
}


	// ...


int main_test() {

	vector<vector<int>> BW = {{0,0,0,0,0,0,0,0,0,0,0,0},
							  {1,1,0,0,0,0,0,0,1,0,0,0},
							  {0,1,0,0,1,1,0,0,1,0,0,0},
							  {0,0,1,0,0,1,0,1,1,0,0,0},
							  {0,0,0,0,0,0,1,0,0,0,1,0},
							  {0,0,0,0,0,0,0,0,1,1,1,0},
							  {0,0,0,0,0,0,0,0,1,0,0,1} };

	CCobj CCo = CCA(BW, "equlist.txt", 8);
	vector<vector<int>> CC_im = CCo.CC_im;
	display_2dvec(CC_im);
	cout << "PILs:";
	display_2dvec(CCo.PILs);

	return 0;
}