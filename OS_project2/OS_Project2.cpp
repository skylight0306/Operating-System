#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib> 
#include <queue>
#include<algorithm>


using namespace std;

struct plist {     //Process list 
    int Id;        // Process ID 
    int Burst;     // CPU Brust
    int Arr;       // Arrival Time
    int Priority;  // Process Proirity 
    int Ttime;     // Turnaround Time
    int Wtime;     // Waiting Time
    bool isExe;    // is execution before
};
	
struct CPU {
	plist Current;
	queue<plist> WaitList;
};
plist temp ;
vector<plist> list ;
vector<plist> waitlist ;
vector<plist> FClist ;
vector<plist> RRlist ;
vector<plist> PSlist ;
vector<plist> NSlist ;
vector<plist> PPlist ;


void ReadFile(int & Method, int & Tslice, string & name);    
string FCFS(int Tslice);
string RR(int Tslice);
string PSJF(int Tslice);
string NSJF(int Tslice);
string PP(int Tslice);
string Allstr[5];
void ALLM(int Tslice); // All Method     
void SaveFile(string str, string &name, int Method);
bool SaveSort(plist &x, plist &y);    
bool Sort1(plist &x, plist &y);    
bool Sort2(plist &x, plist &y);
bool Sort3(plist &x, plist &y);
bool Sort4(plist &x, plist &y);
string Merge(int num, string str);     
void UpdateList(int current, CPU &cpu); 
void UpdateVec(int current);
void UpdateVecPS(plist &Current,int current);
void UpdateVecPP(plist &Current,int current);
void Initialtemp();

int main()
{
	int Method = 0, Tslice = 0;
	string name = "";
	string str = "";
	Initialtemp();
	ReadFile(Method,Tslice,name);  // Read the input and save the data into vector
	if (Method == 1) str = FCFS(Tslice);
	else if (Method == 2) str = RR(Tslice);
	else if (Method == 3) str = PSJF(Tslice);
	else if (Method == 4) str = NSJF(Tslice);
	else if (Method == 5) str = PP(Tslice);
	else if (Method == 6) ALLM(Tslice);
	//for (int i = 0; i < list.size(); i++ ) cout << list[i].Id << " " << list[i].Wtime << " " << list[i].Ttime << endl;
	if ( Method > 0 && Method < 7 ) SaveFile(str, name, Method) ;
	else cout << "The Method does not exist!" << endl;
	cout << name << "¤w«ØÀÉ" ; 

}    

void Initialtemp()
{	
	temp.Arr = 0;
	temp.Burst = 0; 
	temp.Id = 0;
	temp.isExe = false;
	temp.Priority = 0;
	temp.Ttime = 0; 
	temp.Wtime = 0;
	
}
void ALLM(int Tslice)
{
	//for (int i = 0; i < list.size(); i++ ) cout << list[i].Id << " " << list[i].Wtime << " " << list[i].Ttime << endl;
	vector<plist> initialList ;	
	initialList = list;
	for(int i = 0; i < 5; i++)
	{

		list = initialList;
		if ( i == 0 ) Allstr[i] = FCFS(Tslice);
		else if ( i == 1 ) Allstr[i] = RR(Tslice);
		else if ( i == 2 ) Allstr[i] = PSJF(Tslice);
		else if ( i == 3 ) Allstr[i] = NSJF(Tslice);
		else if ( i == 4 ) Allstr[i] = PP(Tslice);
	}

	sort(FClist.begin(),FClist.end(),SaveSort);
	sort(RRlist.begin(),RRlist.end(),SaveSort);
	sort(PSlist.begin(),PSlist.end(),SaveSort);
	sort(NSlist.begin(),NSlist.end(),SaveSort);
	sort(PPlist.begin(),PPlist.end(),SaveSort);
} 
 
bool Sort4(plist &x, plist &y)  // for PSJF
{
	if (x.Priority < y.Priority) // x Arrival Time smaller than y
	{
		return true;
	}
	else if (x.Priority == y.Priority )
	{
		if(!x.isExe && y.isExe)
		{
			return true;
		}
		else if ( x.isExe == y.isExe )
		{
			if ( x.Arr < y.Arr )
			{
				return true;
			}
			else if ( x.Arr == y.Arr )
			{
				if( x.Id < y.Id )
					return true;
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;

	}

	else 
		return false;	
}   
void UpdateVecPP(plist &Current, int current)
{
	
	for(int i = 0;i < list.size(); i++) 
	{
		if ( current == list[i].Arr ) // if the process arrival
		{	
			//cout << endl << "arr :-" << list[i].Id << "-" ; 
			if (list[i].Priority < Current.Priority )  // if the new process CPU burst is smaller than current
			{
				//cout << "t" ;
				for(int j = 0;j < list.size();j++) // find the current id location 
				{
					if( Current.Id == list[j].Id )
					{
						list[i].isExe = true;
						//cout << "in" << list[j].Id << endl << "waitsize" << waitlist.size();
						
						waitlist.push_back(temp);
						waitlist[waitlist.size()-1] = list[j];
						//cout << waitlist[0].Id << list[i].Id;
						Current = list[i];
						//cout << Current.Id;
						break;
					}

					
				}

			}
			else
			{
				//cout << "f";
				waitlist.push_back(temp);
				waitlist[waitlist.size()-1] = list[i];
			}

		}
	}
	//cout << endl;
	sort(waitlist.begin(),waitlist.end(),Sort4);
	
	
} 

string PP(int Tslice)
{
	string str = "-";
	plist Current;
	plist temp;
	int current = 1; // current time
	sort(list.begin(),list.end(),Sort4); // PSJF Serve
	PPlist = list ;
	
	int AllBurst = 1;
	bool first = true;
	for (int i = 0;i < list.size(); i++) AllBurst = AllBurst + list[i].Burst ;
	//for (int i = 0;i < list.size(); i++) cout << list[i].Id << endl;
	while(current < AllBurst)
	{
		//cout << "1";
		
		//UpdateList(current,cpu); // update the waitting list
		if (first) //  first come 
		{
			UpdateVecPP(Current,current); // update the waitting list
			//cout << "2";
			first = false;
			Current = waitlist[0];
			waitlist.erase(waitlist.begin());
		}
		

			//cout << "3";
			for(int j = 0; j < list.size(); j++)
			{
				if ( Current.Id == list[j].Id ) 
				{
					list[j].isExe = true;
					list[j].Burst = list[j].Burst - 1;
					Current = list[j];
					str = Merge(Current.Id,str);
					if ( list[j].Burst == 0 )
					{
						list[j].Ttime = current - list[j].Arr + 1;
						if( !waitlist.empty() )
						{
							Current = waitlist[0];
							waitlist.erase(waitlist.begin());
						}
					
					}
					
					break;
				}
			}
			current++;
			UpdateVecPP(Current,current); // update the waitting list
		



		//cout << "5" << endl;
	    //cout << "str: "<< str << endl << "waitting list : ";
 		//for(int i = 0; i < waitlist.size();i++)cout << waitlist[i].Id ;
 		//cout << endl;

	}
	//cout << str << endl;
	for(int i = 0; i < list.size(); i++) list[i].Wtime = list[i].Ttime - PPlist[i].Burst;
	//for (int i = 0; i < list.size(); i++ ) cout << list[i].Id << " " << list[i].Wtime << " " << list[i].Ttime << endl;

	PPlist = list;
	//for (int i = 0; i < PPlist.size(); i++ ) cout << i <<PPlist[i].Id << " " << PPlist[i].Wtime << " " << PPlist[i].Ttime << endl;
	return str;

}


string NSJF(int Tslice)
{
	string str = "-";
	plist temp;
	plist Current; 
	int current = 1; // current time
	sort(list.begin(),list.end(),Sort2); // NSJF Serve
	int AllBurst = 1;
	bool first = true;
	for (int i = 0;i < list.size(); i++) AllBurst = AllBurst + list[i].Burst ;
	//for (int i = 0;i < list.size(); i++) cout << list[i].Id << endl;
	while(current < AllBurst)
	{
		//cout << "1";
		
		//UpdateList(current,cpu); // update the waitting list
		if (first) //  first come 
		{
			UpdateVec(current); // update the waitting list
			//cout << "2";
			first = false;
			Current = waitlist[0];
			waitlist.erase(waitlist.begin());
		}
		
		for(int i = 0; i < Current.Burst; i++)
		{ 
			//cout << "3";
			str = Merge(Current.Id,str);
			current++;
			UpdateVec(current); // update the waitting list
		} 
		for(int i = 0;i < list.size(); i++)
		{
			if (Current.Id == list[i].Id)
			{
			list[i].Ttime = current - list[i].Arr;
			break;
			}
		}
		if( !waitlist.empty())
		{
			Current = waitlist[0];
			waitlist.erase(waitlist.begin());
		}
		

		//cout << "5" << endl;
	    //cout << "str: "<< str << endl << "waitting list : ";
	    //temp = cpu.WaitList.front();
 
	    //cout << temp.Id << endl << "current : " << cpu.Current.Id;
	    //cout << endl;
	}
	
	//cout << str << endl;
	for(int i = 0; i < list.size(); i++) list[i].Wtime = list[i].Ttime - list[i].Burst;
	NSlist = list;
	//for (int i = 0; i < list.size(); i++ ) cout << list[i].Id << " " << list[i].Wtime << " " << list[i].Ttime << endl;
	return str;

}

bool Sort2(plist &x, plist &y)  // for NSJF
{
	if (x.Burst < y.Burst) // x Burst Time smaller than y
	{
		return true;
	}
	else if (x.Burst == y.Burst )
	{
		if ( x.Arr < y.Arr )
		{
			return true;
		}
		else if ( x.Arr == y.Arr )
		{
			if( x.Id < y.Id )
				return true; 
			else 
				return false;
		}
		else 	
			return false;
	}

	else 
		return false;	
}   


void UpdateVecPS(plist &Current, int current)
{
	
	for(int i = 0;i < list.size(); i++) 
	{
		if ( current == list[i].Arr ) // if the process arrival
		{	
			//cout << endl << "arr :-" << list[i].Id << "-" ; 
			if (list[i].Burst < Current.Burst )  // if the new process CPU burst is smaller than current
			{
				//cout << "t" ;
				for(int j = 0;j < list.size();j++) // find the current id location 
				{
					if( Current.Id == list[j].Id )
					{
						list[i].isExe = true;
						//cout << "in" << list[j].Id << endl << "waitsize" << waitlist.size();
						
						waitlist.push_back(temp);
						waitlist[waitlist.size()-1] = list[j];
						//cout << waitlist[0].Id << list[i].Id;
						Current = list[i];
						//cout << Current.Id;
						break;
					}

					
				}

			}
			else
			{
				//cout << "f";
				waitlist.push_back(temp);
				waitlist[waitlist.size()-1] = list[i];
			}

		}
	}
	//cout << endl;
	sort(waitlist.begin(),waitlist.end(),Sort3);
	

	
	
} 

string PSJF(int Tslice)
{
	string str = "-";
	plist Current;
	plist temp;
	int current = 1; // current time
	sort(list.begin(),list.end(),Sort3); // PSJF Serve
	PSlist = list;
	int AllBurst = 1;
	bool first = true;
	for (int i = 0;i < list.size(); i++) AllBurst = AllBurst + list[i].Burst ;
	//for (int i = 0;i < list.size(); i++) cout << list[i].Id << endl;
	while(current < AllBurst)
	{
		//cout << "1";
		
		//UpdateList(current,cpu); // update the waitting list
		if (first) //  first come 
		{
			UpdateVecPS(Current,current); // update the waitting list
			//cout << "2";
			first = false;
			Current = waitlist[0];
			waitlist.erase(waitlist.begin());
		}
		

			//cout << "3";
			for(int j = 0; j < list.size(); j++)
			{
				if ( Current.Id == list[j].Id ) 
				{
					list[j].isExe = true;
					list[j].Burst = list[j].Burst - 1;
					Current = list[j];
					str = Merge(Current.Id,str);
					if ( list[j].Burst == 0 )
					{
						list[j].Ttime = current - list[j].Arr + 1;
						if( !waitlist.empty() )
						{
							Current = waitlist[0];
							waitlist.erase(waitlist.begin());
						}
					
					}
					
					break;
				}
			}
			current++;
			UpdateVecPS(Current,current); // update the waitting list
		



		//cout << "5" << endl;
	    //cout << "str: "<< str << endl << "waitting list : ";
 		//for(int i = 0; i < waitlist.size();i++)cout << waitlist[i].Id ;
 		//cout << endl;

	}
	//cout << str << endl;
	for(int i = 0; i < list.size(); i++) list[i].Wtime = list[i].Ttime - PSlist[i].Burst;
	//for (int i = 0; i < list.size(); i++ ) cout << list[i].Id << " " << list[i].Wtime << " " << list[i].Ttime << endl;
	PSlist = list ;
	return str;


}

bool Sort3(plist &x, plist &y)  // for PSJF
{
	if (x.Burst < y.Burst) // x Arrival Time smaller than y
	{
		return true;
	}
	else if (x.Burst == y.Burst )
	{
		if(!x.isExe && y.isExe)
		{
			return true;
		}
		else if ( x.isExe == y.isExe )
		{
			if ( x.Arr < y.Arr )
			{
				return true;
			}
			else if ( x.Arr == y.Arr )
			{
				if( x.Id < y.Id )
					return true;
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;

	}

	else 
		return false;	
}   

void UpdateVec(int current)
{
	
	for(int i = 0;i < list.size(); i++) 
	{
		if ( current == list[i].Arr ) // if the process arrival
		{	
			waitlist.push_back(temp);
			waitlist[waitlist.size()-1] = list[i];
		}
	}
	
	sort(waitlist.begin(),waitlist.end(),Sort2);
	
	
}

void UpdateList(int current, CPU &cpu)
{
	for(int i = 0;i < list.size(); i++) 
		if ( current == list[i].Arr ) // if the process arrival
		{
			cpu.WaitList.push(list[i]) ; // save the process into the waitting list	
			//cout << "update : " << list[i].Id << list[i].Burst << endl; 
		}
}	

string RR(int Tslice)
{
	string str = "-";
	int current = 1; // current time
	//for (int i = 0;i < list.size(); i++) cout << list[i].Id << list[i].Burst << endl;
	sort(list.begin(),list.end(),Sort1); // RR Serve
	CPU cpu;
	RRlist = list ;
	int AllBurst = 1;
	bool first = true;
	for (int i = 0;i < list.size(); i++) AllBurst = AllBurst + list[i].Burst ;
	//for (int i = 0;i < list.size(); i++) cout << list[i].Id;
	while(current < AllBurst)
	{
		//cout << "1";
		
		//UpdateList(current,cpu); // update the waitting list
		if (first) //  first come 
		{
			UpdateList(current,cpu); // update the waitting list
			//cout << "2";
			first = false;
			cpu.Current = cpu.WaitList.front() ; // let the front of the waitting process be the current process
			cpu.WaitList.pop();	// delete the front of the waitting list
		}
		
		if (Tslice >= cpu.Current.Burst) // the Time slice is smaller than CPU burst time
		{
			//cout << "in" << cpu.Current.Id << "BURST:" << cpu.Current.Burst << endl;
			for(int i = 0; i < cpu.Current.Burst; i++)
			{ 
				//cout << "3";
				str = Merge(cpu.Current.Id,str);
				current++;
				UpdateList(current,cpu); // update the waitting list
			} 
			for(int i = 0;i < list.size(); i++)
			{
				if (cpu.Current.Id == list[i].Id)
				{
					list[i].Ttime = current - list[i].Arr;
					break;
				}
			}
			cpu.Current = cpu.WaitList.front() ; // let the front of the waitting process be the current process
			cpu.WaitList.pop();	// delete the front of the waitting list
		}
		
		else // the current CPU burst bigger than Time Slice
		{
			for(int i = 0; i < Tslice; i++)
			{
				//cout << "4";
				str = Merge(cpu.Current.Id,str); 	
				current++;
				UpdateList(current,cpu); // update the waitting list
			}
			
			for(int i = 0; i < list.size(); i++)
			{
				if ( cpu.Current.Id == list[i].Id ) 
				{
					
					list[i].Burst = list[i].Burst - Tslice;
					cpu.WaitList.push(list[i]) ; // save the process into the waitting list
					//cout << "in" << list[i].Id << list[i].Burst << endl;
					cpu.Current = cpu.WaitList.front() ; // let the front of the waitting process be the current process
					//cout << "current" << cpu.Current.Id << endl;
					cpu.WaitList.pop();	// delete the front of the waitting list
					break;
				}
				
			}
			
		}
		//cout << "5" << endl;
	    //cout << "str: "<< str << endl << "waitting list : ";
	    //temp = cpu.WaitList.front();
	    // cout << temp.Id << endl << "current : " << cpu.Current.Id;
	    //cout << endl;
	}
	
	//cout << str << endl;
	for(int i = 0; i < list.size(); i++) list[i].Wtime = list[i].Ttime - RRlist[i].Burst;
	//for (int i = 0; i < list.size(); i++ ) cout << list[i].Id << " " << list[i].Wtime << " " << list[i].Ttime << endl;
	RRlist = list;
	return str;
}

bool Sort1(plist &x, plist &y)  // for First Come First Serve Sort and RR
{
	if (x.Arr < y.Arr) // x Arrival Time smaller than y
		return true;
	else if (x.Arr == y.Arr && x.Id < y.Id)
		return true;
	else 
		return false;	
}   

string Merge(int num, string str)
{
	if ( num == 0 ) str = str + "0";
	else if ( num == 1 ) str = str + "1";
	else if ( num == 2 ) str = str + "2";
	else if ( num == 3 ) str = str + "3";
	else if ( num == 4 ) str = str + "4";
	else if ( num == 5 ) str = str + "5";
	else if ( num == 6 ) str = str + "6";
	else if ( num == 7 ) str = str + "7";
	else if ( num == 8 ) str = str + "8";
	else if ( num == 9 ) str = str + "9";
	else if ( num == 10 ) str = str + "A";
	else if ( num == 11 ) str = str + "B";
	else if ( num == 12 ) str = str + "C";
	else if ( num == 13 ) str = str + "D";
	else if ( num == 14 ) str = str + "E";
	else if ( num == 15 ) str = str + "F";
	else if ( num == 16 ) str = str + "G";
	else if ( num == 17 ) str = str + "H";
	else if ( num == 18 ) str = str + "I";
	else if ( num == 19 ) str = str + "J";
	else if ( num == 20 ) str = str + "K";
	else if ( num == 21 ) str = str + "L";
	else if ( num == 22 ) str = str + "M";
	else if ( num == 23 ) str = str + "N";
	else if ( num == 24 ) str = str + "O";
	else if ( num == 25 ) str = str + "P";
	else if ( num == 26 ) str = str + "Q";
	else if ( num == 27 ) str = str + "R";
	else if ( num == 28 ) str = str + "S";
	else if ( num == 29 ) str = str + "T";
	else if ( num == 30 ) str = str + "U";
	else if ( num == 31 ) str = str + "V";
	else if ( num == 32 ) str = str + "W";
	else if ( num == 33 ) str = str + "X";
	else if ( num == 34 ) str = str + "Y";
	else if ( num == 35 ) str = str + "Z";
	return str;
}
   
string FCFS(int Tslice)
{
	string str = "-";
	sort(list.begin(),list.end(),Sort1); // First Come First Serve 
	int current = 1; // current time
	
	//for (int i = 0; i < list.size(); i++ ) cout << list[i].Id << " " << list[i].Burst << " " << list[i].Arr << " " << list[i].Priority << endl;
	
	for(int i = 0; i < list.size(); i++)
	{
		for(int j = 0; j < list[i].Burst;j++)
		{
			str = Merge(list[i].Id,str);
			current++;
		}
		list[i].Ttime = current - list[i].Arr ;
		list[i].Wtime = list[i].Ttime - list[i].Burst;
	}
	//for (int i = 0; i < list.size(); i++ ) cout << list[i].Id << " " << list[i].Wtime << " " << list[i].Ttime << endl;
	FClist = list;
	
	
	return str;
		
}    

bool SaveSort(plist &x, plist &y)  // for SaveFile Sort 
{
	if (x.Id < y.Id) // x Id bigger than y
		return true;
	else 
		return false;	
} 
   
void SaveFile(string str, string &name, int Method) 
{ 
	string output;
	name[0] = 'u' ;
	name[1] = 't' ;
	name = "10627149_o" + name ; // test
	output = name;
	ofstream fout( output.c_str() );	
	if ( Method == 1 )
	{
		sort(FClist.begin(),FClist.end(),SaveSort);
		fout << "=== FCFS ===" << endl << str << endl ;
		fout << endl << "Waiting Time" << endl ;
		fout << "ID	"<< "FCFS" << endl ;
		fout << "========================================================" << endl;
		for(int i = 0; i < FClist.size(); i++) fout << FClist[i].Id << "	" << FClist[i].Wtime << endl;
		fout << endl << "Turnaround Time" << endl ;
		fout << "ID	"<< "FCFS" << endl ;
		fout << "========================================================" << endl;
		for(int i = 0; i < FClist.size(); i++) fout << FClist[i].Id << "	" << FClist[i].Ttime << endl; 
	}
	else if ( Method == 2 )
	{
		sort(RRlist.begin(),RRlist.end(),SaveSort);
		fout << "=== RR ===" << endl << str << endl ;
		fout << endl << "Waiting Time" << endl ;
		fout << "ID	"<< "RR" << endl ;
		fout << "========================================================" << endl;
		for(int i = 0; i < RRlist.size(); i++) fout << RRlist[i].Id << "	" << RRlist[i].Wtime << endl;
		fout << endl << "Turnaround Time" << endl ;
		fout << "ID	"<< "RR" << endl ;
		fout << "========================================================" << endl;
		for(int i = 0; i < RRlist.size(); i++) fout << RRlist[i].Id << "	" << RRlist[i].Ttime << endl; 		
	}
	else if ( Method == 3 )
	{
		sort(PSlist.begin(),PSlist.end(),SaveSort);
		fout << "=== PSJF ===" << endl << str << endl ;
		fout << endl << "Waiting Time" << endl ;
		fout << "ID	"<< "PSJF" << endl ;
		fout << "========================================================" << endl;
		for(int i = 0; i < PSlist.size(); i++) fout << PSlist[i].Id << "	" << PSlist[i].Wtime << endl;
		fout << endl << "Turnaround Time" << endl ;
		fout << "ID	"<< "PSJF" << endl ;
		fout << "========================================================" << endl;
		for(int i = 0; i < PSlist.size(); i++) fout << PSlist[i].Id << "	" << PSlist[i].Ttime << endl; 		
	}
	else if ( Method == 4 )
	{
		sort(NSlist.begin(),NSlist.end(),SaveSort);
		fout << "=== NPSJF ===" << endl << str << endl ;
		fout << endl << "Waiting Time" << endl ;
		fout << "ID	"<< "NPSJF" << endl ;
		fout << "========================================================" << endl;
		for(int i = 0; i < NSlist.size(); i++) fout << NSlist[i].Id << "	" << NSlist[i].Wtime << endl;
		fout << endl << "Turnaround Time" << endl ;
		fout << "ID	"<< "NPSJF" << endl ;
		fout << "========================================================" << endl;
		for(int i = 0; i < NSlist.size(); i++) fout << NSlist[i].Id << "	" << NSlist[i].Ttime << endl; 		
	}
	else if ( Method == 5 )
	{
		sort(PPlist.begin(),PPlist.end(),SaveSort);
		fout << "=== Priority ===" << endl << str << endl ;
		fout << endl << "Waiting Time" << endl ;
		fout << "ID	"<< "Priority" << endl ;
		fout << "========================================================" << endl;
		for(int i = 0; i < PPlist.size(); i++) fout << PPlist[i].Id << "	" << PPlist[i].Wtime << endl;
		fout << endl << "Turnaround Time" << endl ;
		fout << "ID	"<< "Priority" << endl ;
		fout << "========================================================" << endl;
		for(int i = 0; i < PPlist.size(); i++) fout << PPlist[i].Id << "	" << PPlist[i].Ttime << endl; 		
	}
	else if ( Method == 6 )
	{
		fout << "=== FCFS ===" << endl << Allstr[0] << endl ;
		fout << "=== RR ===" << endl << Allstr[1] << endl ;
		fout << "=== PSJF ===" << endl << Allstr[2] << endl ;
		fout << "=== NPSJF ===" << endl << Allstr[3] << endl ;
		fout << "=== Priority ===" << endl << Allstr[4] << endl ;
		fout << "========================================================" << endl;
		fout << "Waiting Time" << endl ;
		fout << "ID	" << "FCFS	" << "RR	" << "PSJF	" << "NPSJF	" << "Priority	" << endl;
		fout << "========================================================" << endl;
		//cout << FClist.size() << RRlist.size() << PSlist.size() << NSlist.size() << PPlist.size() << endl;
		for(int i = 0; i < FClist.size(); i++) fout << FClist[i].Id << "	" << FClist[i].Wtime << "	" << RRlist[i].Wtime << "	" << PSlist[i].Wtime << "	" << NSlist[i].Wtime << "	" << PPlist[i].Wtime << endl;
		fout << endl << "Turnaround Time" << endl ;
		fout << "ID	" << "FCFS	" << "RR	" << "PSJF	" << "NPSJF	" << "Priority	" << endl;
		fout << "========================================================" << endl;
		for(int i = 0; i < FClist.size(); i++) fout << FClist[i].Id << "	" << FClist[i].Ttime << "	" << RRlist[i].Ttime << "	" << PSlist[i].Ttime << "	" << NSlist[i].Ttime << "	" << PPlist[i].Ttime << endl;
		fout << "========================================================" << endl;



	}
	fout.close();

}   
    
    
void ReadFile(int & Method, int & Tslice, string & name)
{
	string str;
	ifstream fin; 
	cout << "Input a file number:" ;  // scanf the file name
	cin >> name ;
	name = name + ".txt" ;
	fin.open( name.c_str() );
	
    while ( !fin ) 
	{ 
	    cout << "### " << name << " does not exist! ###" << endl;
		cout << "Input a file number:" ;
		cin >> name ;
		name = name + ".txt" ;
		fin.open( name.c_str() );	
	}	
	
	// set the output name

	
	fin >> Method ;
	fin >> Tslice ;
	for(int i = 0;i < 6;i++) fin >> str; // scanf the title
	
	for(int i =0;!fin.eof();i++)   // scanf the process information
	{
		list.push_back(temp);
		for(int k = 0;k < 4;k++ )
		{
			if(k == 0) fin >> list[i].Id ;
			else if(k == 1) fin >> list[i].Burst ;
			else if(k == 2) fin >> list[i].Arr ;
			else fin >> list[i].Priority ;
		}
		
	}
    list.pop_back(); //delete the last queue
    
    
	//cout << "Method: " << Method << endl;
	//cout << "Time slice: " << Tslice << endl;
	//for (int i = 0; i < list.size(); i++ ) cout << list[i].Id << " " << list[i].Burst << " " << list[i].Arr << " " << list[i].Priority << endl;
    //cout << list.size() << endl;
		
}
