#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;



void FIFO(int page_frame, string page_schedule, fstream & fout) {
	vector<char> buffer;
	int page_fault = 0;

	for (int i = 0; i < page_frame; i++) {
		char tmp = '-';
		buffer.push_back(tmp);
	}

	fout << "--------------FIFO-----------------------" << endl;
	for (int i = 0; i < page_schedule.size( ); i++) {
		fout << page_schedule[i] << "\t";
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
			fout << buffer[j];
		
		if (fault) {
			fout << "\t";
			fout << "F";
		}
		fout << endl;
	}
	fout << "Page Fault = " << page_fault << "  Page Replaces = " << page_fault - page_frame << "  Page Frames = " << page_frame << endl;
}

void LRU(int page_frame, string page_schedule, fstream & fout) {
	vector<char> buffer;
	int page_fault = 0;

	for (int i = 0; i < page_frame; i++) {
		char tmp = '-';
		buffer.push_back(tmp);
	}

	fout << "--------------LRU--------------------------" << endl;
	for (int i = 0; i < page_schedule.size(); i++) {
		fout << page_schedule[i] << "\t";
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
				fout << buffer[j];

		if (fault) {
			fout << "\t";
			fout << "F";
		}
		fout << endl;
	}
	fout << "Page Fault = " << page_fault << "  Page Replaces = " << page_fault - page_frame << "  Page Frames = " << page_frame << endl;
}

void ARB(int page_frame, string page_schedule, fstream & fout) {
	vector<char> buffer;
	string shift_reg[100];
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

	fout << "--------------Additional Reference Bits-------------------" << endl;
	for (int i = 0; i < page_schedule.size(); i++) {
		fout << page_schedule[i] << "\t";
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
				fout << buffer[j];

		if (fault) {
			fout << "\t";
			fout << "F";
		}
		
		//if (full) {
			//fout << endl;
			//for (int j = 0; j < 6; j++) 
			//	fout <<  "Reg" <<j << "\t" << shift_reg[j] << endl;
		//}

		fout << endl;
	}
	fout << "Page Fault = " << page_fault << "  Page Replaces = " << page_fault - page_frame << "  Page Frames = " << page_frame << endl;
}

void PrintAll(string file_name, string output_name) {
	fstream fin, fout;
	fin.open("output1.txt", ios::in);
	fout.open(output_name, ios::out);
	char tmp;
	while (!fin.eof()) {
		fin.get(tmp);
		fout << tmp;
	}

	fin.close();
	fout.close();
}

void SC(int page_frame, string page_schedule, fstream & fout) {
	vector<char> buffer;
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

	fout << "--------------Second chance Page-------------------------" << endl;
	for (int i = 0; i < page_schedule.size(); i++) {
		fout << page_schedule[i] << "\t";
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
				fout << buffer[j];

		if (fault) {
			fout << "\t";
			fout << "F";
		}

		//if (full) {
		//fout << endl;
		//for (int j = 0; j < 8; j++) 
		//	fout <<  "RB" <<j << "\t" << reference_bit[j] << "\t" << "TS" << j << "\t" << time_stamp[j] << endl;
		//}

		fout << endl;
	}
	fout << "Page Fault = " << page_fault << "  Page Replaces = " << page_fault - page_frame << "  Page Frames = " << page_frame << endl;
}

void LFU(int page_frame, string page_schedule, fstream & fout) {
	vector<char> buffer;
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

	fout << "--------------Least Frequently Used Page Replacement --------------------" << endl;
	for (int i = 0; i < page_schedule.size(); i++) {
		fout << page_schedule[i] << "\t";
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
				fout << buffer[j];

		if (fault) {
			fout << "\t";
			fout << "F";
		}

		//if (full) {
		//fout << endl;
		//for (int j = 0; j < 8; j++) 
		//	fout << "Counter" << j << "\t" << counter[j] << "\tTS" << j << "\t" << time_stamp[j] << endl;
		//}

		fout << endl;
	}
	fout << "Page Fault = " << page_fault << "  Page Replaces = " << page_fault - page_frame << "  Page Frames = " << page_frame << endl;
}

void MFU(int page_frame, string page_schedule, fstream & fout) {
	vector<char> buffer;
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

	fout << "--------------Most Frequently Used Page Replacement ------------------------" << endl;
	for (int i = 0; i < page_schedule.size(); i++) {
		fout << page_schedule[i] << "\t";
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
				fout << buffer[j];

		if (fault) {
			fout << "\t";
			fout << "F";
		}

		//if (full) {
		//fout << endl;
		//for (int j = 0; j < 8; j++) 
		//	fout << "Counter" << j << "\t" << counter[j] << "\tTS" << j << "\t" << time_stamp[j] << endl;
		//}

		fout << endl;
	}
	fout << "Page Fault = " << page_fault << "  Page Replaces = " << page_fault - page_frame << "  Page Frames = " << page_frame << endl;
}


int main() {
	string file_name, page_schedule;
	fstream fin, fout;
	int page_frame;


	while (1) {
		
		cout << "Please input the file name : ";
		cin >> file_name;
		fin.open(file_name, ios::in); 
		
		//file_name = "input2.txt";
		//fin.open(file_name, ios::in);
		if (!fin) 
			cout << "Fail to open the file: " << file_name << endl;

		fin >> page_frame >> page_schedule;
		fin.close();
		string output_name = "";
		for (int i = 0; i < file_name.size(); i++)
			if (file_name[i] == '.')
				break;
			else
				output_name += file_name[i];
		output_name += "_output.txt";
		fout.open(output_name, ios::out);

		FIFO(page_frame, page_schedule, fout);
		fout << endl;
		LRU(page_frame, page_schedule, fout);
		fout << endl;
		ARB(page_frame, page_schedule, fout);
		fout << endl;
		SC(page_frame, page_schedule, fout);
		fout << endl;
		LFU(page_frame, page_schedule, fout);
		fout << endl;
		MFU(page_frame, page_schedule, fout);
		fout << endl;
		fout.close();
		cout << "Done!" << endl;
		//PrintAll(file_name, output_name);
		//system("pause");
		
	}

	return 0;
}