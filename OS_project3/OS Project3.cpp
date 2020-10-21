#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

string int2str(int x) {
	string str = "";
	if (x == 0 ) str = "0" ;
	else if ( x == 1 ) str = "1" ;
	else if ( x == 2 ) str = "2" ;
	else if ( x == 3 ) str = "3" ;
	else if ( x == 4 ) str = "4" ;
	else if ( x == 5 ) str = "5" ;
	else if ( x == 6 ) str = "6" ;
	else if ( x == 7 ) str = "7" ;
	else if ( x == 8 ) str = "8" ;
	else if ( x == 9 ) str = "9" ;	
	else if ( x == 10 ) str = "10" ;
	else if ( x == 11 ) str = "11" ;
	else if ( x == 12 ) str = "12" ;
	else if ( x == 13 ) str = "13" ;
	else if ( x == 14 ) str = "14" ;
	else if ( x == 15 ) str = "15" ;
	else if ( x == 16 ) str = "16" ;
	else if ( x == 17 ) str = "17" ;
	else if ( x == 18 ) str = "18" ;
	else if ( x == 19 ) str = "19" ;
	else if ( x == 20 ) str = "20" ;
	else if ( x == 21 ) str = "21" ;
	else if ( x == 22 ) str = "22" ;
	else if ( x == 23 ) str = "23" ;
	else if ( x == 24 ) str = "24" ;
	else if ( x == 25 ) str = "25" ;
			

	return str;
	
}

string FIFO(int page_frame, string page_schedule) {  //先進先出 
	vector<char> buffer;
	string str = "";
	string sen = "";
	int page_fault = 0;
	//cout << "in FIFO" ;

	for (int i = 0; i < page_frame; i++) {
		char tmp = '-';
		buffer.push_back(tmp);
	}

	str +=  "--------------FIFO-----------------------";
	str += "\n";
	for (int i = 0; i < page_schedule.size( ); i++) {
		str += page_schedule[i] ;
		str += "\t";
		int stored_idx;
		bool fault = false, stored = false;
		for (int j = 0; j < page_frame;j++)
			if (buffer[j] == page_schedule[i]) {
				stored_idx = j;
				stored = true;
			}
		/*
		if (stored) {
			for (int j = stored_idx ; j > 0; j--) {
				buffer[j] = buffer[j - 1];
			}
			buffer[0] = page_schedule[i];
		}
		*/
		if (!stored) {
			fault = true;
			page_fault++;
			for (int j = page_frame-1; j > 0; j--) {
				buffer[j] = buffer[j - 1];
			}
			buffer[0] = page_schedule[i];
		}
		for (int j = 0; j < page_frame; j++)
			if (buffer[j] != '-')
			str += buffer[j];
		
		if (fault) {
			str += "\t";
			str += "F";
		}
		str += "\n";
	}
	string pf = "";
	string pr = "";
	string pfr = "";
	int PageRe = page_fault - page_frame;
	pf = int2str(page_fault) ;
	pr = int2str(PageRe) ;
	pfr = int2str(page_frame) ;

	sen = "Page Fault = " + pf;
	sen += "  Page Replaces = " ;
	sen += pr ;
	sen += "  Page Frames = ";
	sen += pfr ;
	sen += "\n";
	str += sen;
	return str;
}

string LRU(int page_frame, string page_schedule) { //刷新 
	vector<char> buffer;
	int page_fault = 0;
	string str = "";
	string sen = "";
	for (int i = 0; i < page_frame; i++) {
		char tmp = '-';
		buffer.push_back(tmp);
	}

	str = "--------------LRU--------------------------" ;
	str += "\n";
	for (int i = 0; i < page_schedule.size(); i++) {
		str += page_schedule[i] ;
		str += "\t";
		int stored_idx;
		bool fault = false, stored = false;
		for (int j = 0; j < page_frame;j++)
			if (buffer[j] == page_schedule[i]) {
				stored_idx = j;
				stored = true;
			}
		
		if (stored) {
		for (int j = stored_idx ; j > 0; j--) {
		buffer[j] = buffer[j - 1];
		}
		buffer[0] = page_schedule[i];
		}
		
		else {
			fault = true;
			page_fault++;
			for (int j = page_frame - 1; j > 0; j--) {
				buffer[j] = buffer[j - 1];
			}
			buffer[0] = page_schedule[i];
		}
		for (int j = 0; j < page_frame; j++)
			if (buffer[j] != '-')
				str += buffer[j];

		if (fault) {
			str += "\t";
			str += "F";
		}
		str += "\n";
	}
	string pf = "";
	string pr = "";
	string pfr = "";
	int PageRe = page_fault - page_frame;
	pf = int2str(page_fault) ;
	pr = int2str(PageRe) ;
	pfr = int2str(page_frame) ;

	sen = "Page Fault = " + pf;
	sen += "  Page Replaces = " ;
	sen += pr ;
	sen += "  Page Frames = ";
	sen += pfr ;
	sen += "\n";
	str += sen;
	return str;

}

string ARB(int page_frame, string page_schedule) { // 8bits 
	vector<char> buffer;
	string shift_reg[100];
	string str = "";
	string sen = "";
	bool inBuffer[100];

	int page_fault = 0;

	for (int i = 0; i < page_frame; i++) {
		char tmp = '-';
		buffer.push_back(tmp);
	}

	for (int i = 0; i < 100; i++) {
		shift_reg[i] = "00000000";
		inBuffer[i] = false;
	}

	str = "--------------Additional Reference Bits-------------------" ;
	str += "\n";
	for (int i = 0; i < page_schedule.size(); i++) {
		str += page_schedule[i] ;
		str += "\t";
		bool fault = false, stored = false, full = true;;
		for (int j = 0; j < page_frame;j++)
			if (buffer[j] == page_schedule[i]) {
				stored = true;
			}
		
		if (stored) {
			// reg shift
			for (int k = 0; k < 100; k++) {
				for (int j = 7; j > 0; j--) {
					shift_reg[k][j] = shift_reg[k][j - 1];
				}
				if ( k == (int)page_schedule[i]-48)
					shift_reg[k][0] = '1';
				else
					shift_reg[k][0] = '0';
			}
		}


		else {
			fault = true;
			page_fault++;

			for (int j = 0; j < page_frame; j++) 
				if (buffer[j] == '-') {
					full = false;
					break;
				}

			if (full) {
				int min_idx = 0;
				string min = "~" ;

				for (int j = 0; j < 100; j++) {
					if (inBuffer[j]) {
						if (min > shift_reg[j]) {
							
							min = shift_reg[j];
							min_idx = j;
						}
					}
				}
				
				shift_reg[min_idx] = "00000000";
				inBuffer[min_idx] = false;
				
				// Replace
				for (int j = 0; j < page_frame; j++)
					if ((int)buffer[j] - 48 == min_idx)
						min_idx = j;

				for (int j = min_idx; j > 0; j--) {
					buffer[j] = buffer[j - 1];
				}
				buffer[0] = page_schedule[i];

				
				inBuffer[(int)page_schedule[i] - 48] = true;
			}
			else {
				inBuffer[(int)page_schedule[i] - 48] = true;
				
				for (int j = page_frame - 1; j > 0; j--) {
					buffer[j] = buffer[j - 1];
				}
				buffer[0] = page_schedule[i];
			}
			// reg shift
			for (int k = 0; k < 100; k++) {
				for (int j = 7; j > 0; j--) {
					shift_reg[k][j] = shift_reg[k][j - 1];
				}
				if (k == (int)page_schedule[i] - 48)
					shift_reg[k][0] = '1';
				else
					shift_reg[k][0] = '0';
			}

		}

		for (int j = 0; j < page_frame; j++)
			if (buffer[j] != '-')
				str += buffer[j];

		if (fault) {
			str += "\t";
			str += "F";
		}
		
		//if (full) {
			//fout << endl;
			//for (int j = 0; j < 6; j++) 
			//	fout <<  "Reg" <<j << "\t" << shift_reg[j] << endl;
		//}

		str += "\n";
	}
	//cout << "Page Fault = " << page_fault << "  Page Replaces = " << page_fault - page_frame << "  Page Frames = " << page_frame << endl;
	string pf = "";
	string pr = "";
	string pfr = "";
	int PageRe = page_fault - page_frame;
	pf = int2str(page_fault) ;
	pr = int2str(PageRe) ;
	pfr = int2str(page_frame) ;

	sen = "Page Fault = " + pf;
	sen += "  Page Replaces = " ;
	sen += pr ;
	sen += "  Page Frames = ";
	sen += pfr ;
	sen += "\n";
	str += sen;
	return str;	
}

void PrintAll(string file_name, string output_name) {
	fstream fin;
	file_name = "output1.txt";
	ofstream fout(output_name.c_str());
	fin.open( file_name.c_str() );
	char tmp;
	while (!fin.eof()) {
		fin.get(tmp);
		fout << tmp;
	}

	fin.close();
	fout.close();
}

string SC(int page_frame, string page_schedule) { // 二次機會 
	vector<char> buffer;
	string str = "";
	string sen = "";
	int reference_bit[100], time_stamp[100];
	bool inBuffer[100];

	int page_fault = 0;

	for (int i = 0; i < page_frame; i++) {
		char tmp = '-';
		buffer.push_back(tmp);
	}

	for (int i = 0; i < 100; i++) {
		reference_bit[i] = 0;
		time_stamp[i] = 0;
		inBuffer[i] = false;
	}

	str =  "--------------Second chance Page-------------------------" ;
	str += "\n" ;
	for (int i = 0; i < page_schedule.size(); i++) {
		str += page_schedule[i] ;
		str += "\t";
		bool fault = false, stored = false, full = true;;
		for (int j = 0; j < page_frame;j++)
			if (buffer[j] == page_schedule[i]) {
				stored = true;
			}

		if (stored) {
			reference_bit[(int)page_schedule[i] - 48] = 1;
			//time_stamp[(int)page_schedule[i] - 48] = i+1;
		}


		else {
			fault = true;
			page_fault++;

			for (int j = 0; j < page_frame; j++)
				if (buffer[j] == '-') {
					full = false;
					break;
				}

			if (full) {
				int min_idx = 0, cnt = 0;

				for (int j = 0; j < 100; j++)
					if (inBuffer[j])
						if (reference_bit[j] == 1)
							cnt++;

				if (cnt == page_frame) {
					inBuffer[(int)buffer[page_frame - 1] - 48] = false;
					time_stamp[(int)buffer[page_frame - 1] - 48] = 0;

					for (int j = 0; j < page_frame; j++)
						reference_bit[(int)buffer[j] - 48] = 0;

					inBuffer[(int)page_schedule[i] - 48] = true;
					reference_bit[(int)page_schedule[i] - 48] = 1;
					

					for (int j = page_frame - 1; j > 0; j--) {
						buffer[j] = buffer[j - 1];
					}
					buffer[0] = page_schedule[i];

					

					for (int j = 0; j < page_frame; j++)
						time_stamp[(int)buffer[j] - 48] = i+1;
					for (int j = 0; j < page_frame; j++) {
						for (int k = j + 1; k < page_frame; k++)
							if (time_stamp[(int)buffer[j] - 48] < time_stamp[(int)buffer[k] - 48])
								swap(buffer[j], buffer[k]);
					}

				}
				else {
					inBuffer[(int)page_schedule[i] - 48] = true;
					reference_bit[(int)page_schedule[i] - 48] = 1;
					time_stamp[(int)page_schedule[i] - 48] = i+1;

					int replace_idx = 0, replace_key = 0;
					for (int j = page_frame - 1; j >= 0; j--) {
						if (reference_bit[(int)buffer[j] - 48] == 1) 
							time_stamp[(int)buffer[j] - 48] = i + 1;
						if (reference_bit[(int)buffer[j] - 48] == 0) {
							replace_idx = j;
							replace_key = (int)buffer[j] - 48;
							break;
						}
					}

					inBuffer[replace_key] = false;
					time_stamp[replace_key] = 0;

					for (int j = page_frame - 1; j >= 0; j--) {
						if (reference_bit[(int)buffer[j] - 48] == 1) {
							if (replace_idx <j)
								reference_bit[(int)buffer[j] - 48] = 0;
							break;
						}
					}


					for (int j = replace_idx; j > 0; j--) {
						buffer[j] = buffer[j - 1];
					}
					buffer[0] = page_schedule[i];

					for (int j = 0; j < page_frame; j++) {
						for (int k = j + 1; k < page_frame; k++)
							if (time_stamp[(int)buffer[j] - 48] < time_stamp[(int)buffer[k] - 48])
								swap(buffer[j], buffer[k]);
					}
				}

				
			}
			else {
				inBuffer[(int)page_schedule[i] - 48] = true;
				reference_bit[(int)page_schedule[i] - 48] = 1;
				time_stamp[(int)page_schedule[i] - 48] = i+1;

				for (int j = page_frame - 1; j > 0; j--) {
					buffer[j] = buffer[j - 1];
				}
				buffer[0] = page_schedule[i];
			}


		}
		

		for (int j = 0; j < page_frame; j++)
			if (buffer[j] != '-')
				str += buffer[j];

		if (fault) {
			str += "\t";
			str += "F";
		}

		//if (full) {
		//fout << endl;
		//for (int j = 0; j < 8; j++) 
		//	fout <<  "RB" <<j << "\t" << reference_bit[j] << "\t" << "TS" << j << "\t" << time_stamp[j] << endl;
		//}

		str += "\n";
	}
	string pf = "";
	string pr = "";
	string pfr = "";
	int PageRe = page_fault - page_frame;
	pf = int2str(page_fault) ;
	pr = int2str(PageRe) ;
	pfr = int2str(page_frame) ;

	sen = "Page Fault = " + pf;
	sen += "  Page Replaces = " ;
	sen += pr ;
	sen += "  Page Frames = ";
	sen += pfr ;
	sen += "\n";
	str += sen;
	return str;	

}

string LFU(int page_frame, string page_schedule) { //最少使用丟掉 
	vector<char> buffer;
	string sen = "";
	string str = "";
	int time_stamp[100], counter[100];

	bool inBuffer[100];

	int page_fault = 0;

	for (int i = 0; i < page_frame; i++) {
		char tmp = '-';
		buffer.push_back(tmp);
	}

	for (int i = 0; i < 100; i++) {
		time_stamp[i] = 0;
		counter[i] = 0;
		inBuffer[i] = false;
	}

	str = "--------------Least Frequently Used Page Replacement --------------------" ;
	str += "\n" ;
	for (int i = 0; i < page_schedule.size(); i++) {
		str += page_schedule[i] ;
		str += "\t";
		bool fault = false, stored = false, full = true;;
		for (int j = 0; j < page_frame;j++)
			if (buffer[j] == page_schedule[i]) {
				stored = true;
			}

		if (stored) {
			counter[(int)page_schedule[i] - 48] += 1;
			time_stamp[(int)page_schedule[i] - 48] = i + 1;
		}


		else {
			fault = true;
			page_fault++;

			for (int j = 0; j < page_frame; j++)
				if (buffer[j] == '-') {
					full = false;
					break;
				}

			if (full) {
				int min_idx = 0, min_key = 0, min_counter = 999999999, min_timestamp ;
				// check counter smallest
				for (int j = page_frame-1; j >= 0 ; j--)
					if (min_counter > counter[(int)buffer[j] - 48]) {
						min_counter = counter[(int)buffer[j] - 48];
						min_idx = j;
						min_key = (int)buffer[j] - 48;
					}

				min_timestamp = time_stamp[min_key];
				// if counter as the same , check time stamp smallest
				for (int j = 0; j < page_frame; j++)
					if (counter[min_key] == counter[(int)buffer[j] - 48] && time_stamp[(int)buffer[j] - 48] < min_timestamp && j != min_idx) {
						min_timestamp = time_stamp[(int)buffer[j] - 48];
						min_idx = j;
						min_key = (int)buffer[j] - 48;
					}
				
				time_stamp[min_key] = 0;
				counter[min_key] = 0;
				inBuffer[min_key] = false;

				counter[(int)page_schedule[i] - 48] += 1;
				time_stamp[(int)page_schedule[i] - 48] = i+1;
				inBuffer[(int)page_schedule[i] - 48] = true;


				for (int j = min_idx; j > 0; j--) {
					buffer[j] = buffer[j - 1];
				}
				buffer[0] = page_schedule[i];

			}
			else {
				inBuffer[(int)page_schedule[i] - 48] = true;
				counter[(int)page_schedule[i] - 48] += 1;
				time_stamp[(int)page_schedule[i] - 48] = i+1;

				for (int j = page_frame - 1; j > 0; j--) {
					buffer[j] = buffer[j - 1];
				}
				buffer[0] = page_schedule[i];
			}


		}


		for (int j = 0; j < page_frame; j++)
			if (buffer[j] != '-')
				str += buffer[j];

		if (fault) {
			str += "\t";
			str += "F";
		}

		//if (full) {
		//fout << endl;
		//for (int j = 0; j < 8; j++) 
		//	fout << "Counter" << j << "\t" << counter[j] << "\tTS" << j << "\t" << time_stamp[j] << endl;
		//}

		str += "\n";
	}
	string pf = "";
	string pr = "";
	string pfr = "";
	int PageRe = page_fault - page_frame;
	pf = int2str(page_fault) ;
	pr = int2str(PageRe) ;
	pfr = int2str(page_frame) ;

	sen = "Page Fault = " + pf;
	sen += "  Page Replaces = " ;
	sen += pr ;
	sen += "  Page Frames = ";
	sen += pfr ;
	sen += "\n";
	str += sen;
	return str;	
}

string MFU(int page_frame, string page_schedule) { 
	vector<char> buffer;
	string sen = "";
	string str = "";
	int time_stamp[100], counter[100];
	bool inBuffer[100];

	int page_fault = 0;

	for (int i = 0; i < page_frame; i++) {
		char tmp = '-';
		buffer.push_back(tmp);
	}

	for (int i = 0; i < 100; i++) {
		time_stamp[i] = 0;
		counter[i] = 0;
		inBuffer[i] = false;
	}

	str = "--------------Most Frequently Used Page Replacement ------------------------" ;
	str += "\n";
	for (int i = 0; i < page_schedule.size(); i++) {
		str += page_schedule[i] ;
		str += "\t";
		bool fault = false, stored = false, full = true;;
		for (int j = 0; j < page_frame;j++)
			if (buffer[j] == page_schedule[i]) {
				stored = true;
			}

		if (stored) {
			counter[(int)page_schedule[i] - 48] += 1;
			time_stamp[(int)page_schedule[i] - 48] = i + 1;
		}


		else {
			fault = true;
			page_fault++;

			for (int j = 0; j < page_frame; j++)
				if (buffer[j] == '-') {
					full = false;
					break;
				}

			if (full) {
				int max_idx = 0, max_key = 0, max_counter = -999999999, max_timestamp;
				// check counter smallest
				for (int j = page_frame - 1; j >= 0; j--)
					if (max_counter < counter[(int)buffer[j] - 48]) {
						max_counter = counter[(int)buffer[j] - 48];
						max_idx = j;
						max_key = (int)buffer[j] - 48;
					}

				max_timestamp = time_stamp[max_key];
				// if counter as the same , check time stamp smallest
				for (int j = 0; j < page_frame; j++)
					if (counter[max_key] == counter[(int)buffer[j] - 48] && time_stamp[(int)buffer[j] - 48] < max_timestamp && j != max_idx) {
						max_timestamp = time_stamp[(int)buffer[j] - 48];
						max_idx = j;
						max_key = (int)buffer[j] - 48;
					}

				time_stamp[max_key] = 0;
				counter[max_key] = 0;
				inBuffer[max_key] = false;

				counter[(int)page_schedule[i] - 48] += 1;
				time_stamp[(int)page_schedule[i] - 48] = i + 1;
				inBuffer[(int)page_schedule[i] - 48] = true;


				for (int j = max_idx; j > 0; j--) {
					buffer[j] = buffer[j - 1];
				}
				buffer[0] = page_schedule[i];

			}
			else {
				inBuffer[(int)page_schedule[i] - 48] = true;
				counter[(int)page_schedule[i] - 48] += 1;
				time_stamp[(int)page_schedule[i] - 48] = i + 1;

				for (int j = page_frame - 1; j > 0; j--) {
					buffer[j] = buffer[j - 1];
				}
				buffer[0] = page_schedule[i];
			}


		}


		for (int j = 0; j < page_frame; j++)
			if (buffer[j] != '-')
				str += buffer[j];

		if (fault) {
			str += "\t";
			str += "F";
		}

		//if (full) {
		//fout << endl;
		//for (int j = 0; j < 8; j++) 
		//	fout << "Counter" << j << "\t" << counter[j] << "\tTS" << j << "\t" << time_stamp[j] << endl;
		//}

		str += "\n";
	}
	string pf = "";
	string pr = "";
	string pfr = "";
	int PageRe = page_fault - page_frame;
	pf = int2str(page_fault) ;
	pr = int2str(PageRe) ;
	pfr = int2str(page_frame) ;

	sen = "Page Fault = " + pf;
	sen += "  Page Replaces = " ;
	sen += pr ;
	sen += "  Page Frames = ";
	sen += pfr ;
	sen += "\n";
	str += sen;
	return str;	
}


int main() {
	string file_name, page_schedule, name;
	string output_name = "";
	fstream fin;
	string str = "";
	int Method = 0;
	int page_frame;

		cout << endl << "***** OS PROJECT 3      *****";
        cout << endl << "* 1. 基本題                 *";
        cout << endl << "* 1. LFU + FIFO             *";
        cout << endl << "* 2. MFU + LIFO             *";
        cout << endl << "*************************************";
        cout << endl << "Input a choice(1, 2, 3): ";
		cin >> Method ; 
		cout << "Please input the file name : ";
		cin >> name;
		name = name + ".txt";
		fin.open(name.c_str()); 
		while ( !fin ) 
		{ 
	    cout << "### " << name << " does not exist! ###" << endl;
		cout << "Input a file number:" ;
		cin >> name ;
		name = name + ".txt" ;

		fin.open( name.c_str() );	
		}	

		fin >> page_frame >> page_schedule;
		//cout << page_frame << ", " << page_schedule << endl;
		fin.close();

		if (Method == 1) {
			name[0] = 'u' ;
			name[1] = 't' ;
			output_name = "10627149_o" + name ;

			ofstream fout(output_name.c_str());
			str = "";
			str = FIFO(page_frame, page_schedule);
			fout << str ;
			fout << endl ;
			str = "";
			str = LRU(page_frame, page_schedule);
			fout << str ;
			fout << endl;
			str = "";
			str = ARB(page_frame, page_schedule);
			fout << str ;
			fout << endl;
			str = "";
			str = SC(page_frame, page_schedule);
			fout << str ;
			fout << endl;
			str = "";		
			str = LFU(page_frame, page_schedule);
			fout << str ;
			fout << endl;
			str = "";
			str = MFU(page_frame, page_schedule);
			fout << str ;
			fout << endl;
			fout.close();
			}
		else if (Method == 2) {
			name[0] = 'u' ;
			name[1] = 't' ;
			output_name = "TestA_o" + name ;
		}		
		else if (Method == 3) {
			name[0] = 'u' ;
			name[1] = 't' ;
			output_name = "TestB_o" + name ;			
		}
		cout << output_name << " 已建檔 !" << endl;
		//PrintAll(file_name, output_name);



	return 0;
}
