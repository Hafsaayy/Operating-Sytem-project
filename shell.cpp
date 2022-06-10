#include<iostream>
#include<algorithm>
#include<iomanip>
#include<climits> 
#include <queue>
#include<string>
using namespace std;
#define FIXED_FLOAT(x) std::fixed <<std::setprecision(2)<<(x)
string suggestions[4] = {"Exellent","Good","Fair","Poor"};
float avg_tat[4],avg_rt[4],avg_wt[4],avg_tp[4],avg_util[4];
struct process_struct_global
{
  int pid;
  int at;
  int bt;
  int ct,wt,tat,rt,start_time;
  int bt_remaining;
}global[100];

bool comparatorAT(struct process_struct_global a,struct process_struct_global b)
{
   int x =a.at;
   int y =b.at;
   return x < y;
} 

bool comparatorPID(struct process_struct_global a,struct process_struct_global b)
{
   int x =a.pid;
   int y =b.pid;
   return x < y;
}

int sjf(bool input,int proc){
	int n = proc;
    bool is_completed[100]={false},is_first_process=true;
    int current_time = 0;
    int completed = 0;; 
    int sum_tat=0,sum_wt=0,sum_rt=0,total_idle_time=0,prev=0,length_cycle;
    float cpu_utilization;
    int max_completion_time,min_arrival_time;
    
    if(input){
    	cout<<"Enter total number of processes: ";
    	cin>>n;   
		cout << fixed << setprecision(2);
		
		for(int i=0;i<n;i++)
		{
		    cout<<"\nEnter Process " <<i<< " Arrival Time: ";
		    cin >> global[i].at;
		    global[i].pid=i;
		}
		
		for(int i=0;i<n;i++)
		{
		    cout<<"\nEnter Process " <<i<< " Burst Time: ";
		    cin >> global[i].bt;
		}
	}
    
    while(completed!=n)
    {
        //find process with min. burst time in ready queue at current time
        int min_index = -1;
        int minimum = INT_MAX;
        for(int i = 0; i < n; i++) {
            if(global[i].at <= current_time && is_completed[i] == false) {
                if(global[i].bt < minimum) {
                    minimum = global[i].bt;
                    min_index = i;
                }
                if(global[i].bt== minimum) {
                    if(global[i].at < global[min_index].at) {
                        minimum= global[i].bt;
                        min_index = i;
                    }
                }
            }
        }

        if(min_index==-1)
        {
            current_time++;
        }
        else
        {
        global[min_index].start_time = current_time;
        global[min_index].ct = global[min_index].start_time + global[min_index].bt;
        global[min_index].tat = global[min_index].ct - global[min_index].at;
        global[min_index].wt = global[min_index].tat - global[min_index].bt;
        global[min_index].rt = global[min_index].wt;
        // global[min_index].rt = global[min_index].start_time - global[min_index].at;
                
        sum_tat +=global[min_index].tat;
        sum_wt += global[min_index].wt;
        sum_rt += global[min_index].rt;
        total_idle_time += (is_first_process==true) ? 0 : (global[min_index].start_time -  prev);
        
        completed++;
        is_completed[min_index]=true;
        current_time = global[min_index].ct;
        prev= current_time;
        is_first_process = false;  
        }
    }
    
    //Calculate Length of Process completion cycle
    max_completion_time = INT_MIN;
    min_arrival_time = INT_MAX;
    for(int i=0;i<n;i++)
    {
        max_completion_time = max(max_completion_time,global[i].ct);
        min_arrival_time = min(min_arrival_time,global[i].at);
    }
    length_cycle = max_completion_time - min_arrival_time;

     //Output
    if(input){
    	cout<<"\nPocess No.\tAT\tCPU Burst Time\tCT\tTAT\tWT\tRT\n";
	}
    for(int i=0;i<n;i++){
    	if(input){
     		cout<<i<<"\t\t"<<global[i].at<<"\t"<<global[i].bt<<"\t\t"<<global[i].ct<<"\t"<<global[i].tat<<"\t"<<global[i].wt<<"\t"<<global[i].rt<<endl;
     }
	 global[i].start_time = 0;
     global[i].ct = 0;
     global[i].tat = 0;
     global[i].wt = 0;
     global[i].rt = 0;
	}
    cout << endl;
    cpu_utilization = (float)(length_cycle - total_idle_time)/ length_cycle;
	
    if(input){
	  	cout<<"\nAverage Turn Around time= "<< (float)sum_tat/n;
	    cout<<"\nAverage Waiting Time= "<<(float)sum_wt/n;
	    cout<<"\nAverage Response Time= "<<(float)sum_rt/n;    
	    cout<<"\nThroughput= "<<n/(float)length_cycle;
	    cout<<"\nCPU Utilization(Percentage)= " << cpu_utilization*100;
	}
    avg_tat[3] = (float)sum_tat/n;
    avg_wt[3] = (float)sum_wt/n;
    avg_rt[3] = (float)sum_rt/n;
    avg_tp[3] = n/(float)length_cycle;
    avg_util[3] = cpu_utilization*100;
    
    return 0;
}
int rr(bool input,int opt,int proc){
	int n=proc,index;
    int cpu_utilization;
    queue<int> q;
    bool visited[100]={false},is_first_process=true;
    int current_time = 0,max_completion_time;
    int completed = 0,tq, total_idle_time=0,length_cycle;
    tq = opt;
    float sum_tat=0,sum_wt=0,sum_rt=0;
	if(input){
		cout<<"Enter total number of processes: ";
	    cin>>n;
	    cout << fixed << setprecision(2);
	
	    for(int i=0;i<n;i++)
	    {
	        cout<<"\nEnter Process " <<i<< " Arrival Time: ";
	        cin >> global[i].at;
	        global[i].pid=i;
	    }
	    
	    for(int i=0;i<n;i++)
	    {
	        cout<<"\nEnter Process " <<i<< " Burst Time: ";
	        cin >> global[i].bt;
	        global[i].bt_remaining= global[i].bt;
	    }
	    
	    cout<<"\nEnter time quanta: ";
	    cin>>tq;
	}
    //sort structure on the basis of Arrival time in increasing order
    sort(global,global+n,comparatorAT);
    
    q.push(0);  
    visited[0] = true;
   
    while(completed != n) 
    {
      index = q.front();      
      q.pop();
      
      if(global[index].bt_remaining == global[index].bt)
      {
            global[index].start_time = max(current_time,global[index].at);
            total_idle_time += (is_first_process == true) ? 0 : global[index].start_time - current_time;
            current_time =  global[index].start_time;
            is_first_process = false;
             
      }

      if(global[index].bt_remaining-tq > 0)
      {    
            global[index].bt_remaining -= tq;
            current_time += tq;
      }
      else 
      {
            current_time += global[index].bt_remaining;
            global[index].bt_remaining = 0;
            completed++;

            global[index].ct = current_time;
            global[index].tat = global[index].ct - global[index].at;
            global[index].wt = global[index].tat - global[index].bt;
            global[index].rt = global[index].start_time - global[index].at;

            sum_tat += global[index].tat;
            sum_wt += global[index].wt;
            sum_rt += global[index].rt;
      }


       //check which new Processes needs to be pushed to Ready Queue from Input list
      for(int i = 1; i < n; i++) 
      {
          if(global[i].bt_remaining > 0 && global[i].at <= current_time && visited[i] == false) 
          {
            q.push(i);
            visited[i] = true;
          }
      }
      //check if Process on CPU needs to be pushed to Ready Queue
      if( global[index].bt_remaining> 0) 
          q.push(index);
            
      //if queue is empty, just add one process from list, whose remaining burst time > 0
      if(q.empty())
      {
          for(int i = 1; i < n; i++)
          {
            if(global[i].bt_remaining > 0)
            {
              q.push(i);
              visited[i] = true;
              break;
            }
          }
      }
   } //end of while
   
   //Calculate Length of Process completion cycle
  max_completion_time = INT_MIN;
  
  for(int i=0;i<n;i++)
        max_completion_time = max(max_completion_time,global[i].ct);
      
  length_cycle = max_completion_time - global[0].at;  //global[0].start_time; 
    
  cpu_utilization = (float)(length_cycle - total_idle_time)/ length_cycle;

  //sort so that process ID in output comes in Original order (just for interactivity- Not needed otherwise)  
  sort(global, global+n , comparatorPID);

  //Output
  if(input){
  	cout<<"\nProcess No.\tAT\tCPU Burst Time\tStart Time\tCT\tTAT\tWT\tRT\n";
	}
  for(int i=0;i<n;i++){
  	if(input){
    	cout<<i<<"\t\t"<<global[i].at<<"\t"<<global[i].bt<<"\t\t"<<global[i].start_time<<"\t\t"<<global[i].ct<<"\t"<<global[i].tat<<"\t"<<global[i].wt<<"\t"<<global[i].rt<<endl;   
		}
	 global[i].start_time = 0;
     global[i].ct = 0;
     global[i].tat = 0;
     global[i].wt = 0;
     global[i].rt = 0;
	}
	cout<<endl; 
	if(input){
		cout<<"\nAverage Turn Around time= "<< (float)sum_tat/n;
		cout<<"\nAverage Waiting Time= "<<(float)sum_wt/n;
		cout<<"\nAverage Response Time= "<<(float)sum_rt/n;    
		cout<<"\nThroughput= "<<n/(float)length_cycle;
		cout<<"\nCPU Utilization(Percentage)= " << cpu_utilization*100;
	}
    avg_tat[2] = (float)sum_tat/n;
    avg_wt[2] = (float)sum_wt/n;
    avg_rt[2] = (float)sum_rt/n;
    avg_tp[2] = n/(float)length_cycle;
    avg_util[2] = cpu_utilization*100;
  return 0;
}
int fcfs(bool input,int proc){
	int n=proc;
    float sum_tat=0,sum_wt=0,sum_rt=0;
    int length_cycle,total_idle_time=0;
    float cpu_utilization;
    
    if(input){
	    cout<<"Enter total number of processes: ";
	    cin>>n;
	    cout << fixed << setprecision(2);
	    for(int i=0;i<n;i++)
	    {
	        cout<<"\nEnter Process" <<i<< "Arrival Time: ";
	        cin >> global[i].at;
	        global[i].pid=i;
	    }
	    
	    for(int i=0;i<n;i++)
	    {
	        cout<<"\nEnter Process" <<i<< "Burst Time: ";
	        cin >> global[i].bt;
	    }
	}
    //sort
    sort(global,global+n, comparatorAT);

    //calculation
    for(int i=0;i<n;i++)
    {
      global[i].start_time = (i==0) ? global[i].at : max(global[i].at, global[i-1].ct);  
      global[i].ct =  global[i].start_time + global[i].bt;
      global[i].tat = global[i].ct-global[i].at;       
      global[i].wt = global[i].tat-global[i].bt;
      global[i].rt=global[i].wt;
       
      sum_tat += global[i].tat;
      sum_wt += global[i].wt;
      sum_rt += global[i].rt;
      total_idle_time += (i==0) ? 0 : (global[i].start_time -  global[i-1].ct);
      
    }
    length_cycle = global[n-1].ct - global[0].start_time;
    //sort so that process ID in output comes in Original order (just for interactivity)
    sort(global,global+n, comparatorPID);
    
    //Output
    if(input){
   		cout<<"\nProcess No.\tAT\tCPU Burst Time\tCT\tTAT\tWT\tRT\n";
	}
    for(int i=0;i<n;i++){
     if(input){
	 	cout<<i<<"\t\t"<<global[i].at<<"\t"<<global[i].bt<<"\t\t"<<global[i].ct<<"\t"<<global[i].tat<<"\t"<<global[i].wt<<"\t"<<global[i].rt<<endl;
     }
	 global[i].start_time = 0;
     global[i].ct = 0;
     global[i].tat = 0;
     global[i].wt = 0;
     global[i].rt = 0;
	}
    cout<<endl;
    cpu_utilization = (float)(length_cycle - total_idle_time)/ length_cycle;
    
    if(input){
    	cout<<"\nAverage Turn Around time= "<< sum_tat/n;
	    cout<<"\nAverage Waiting Time= "<<sum_wt/n;
	    cout<<"\nAverage Response Time= "<<sum_rt/n;    
	    cout<<"\nThroughput= "<<n/(float)length_cycle;
	    cout<<"\nCPU Utilization(Percentage)= " << cpu_utilization*100;
	}
    avg_tat[1] = (float)sum_tat/n;
    avg_wt[1] = (float)sum_wt/n;
    avg_rt[1] = (float)sum_rt/n;
    avg_tp[1] = n/(float)length_cycle;
    avg_util[1] =cpu_utilization*100;
    return 0;
}
int srtf(bool input,int proc){
	int n=proc;
    float bt_remaining[100];
    bool is_completed[100]={false},is_first_process=true;
    int current_time = 0;
    int completed = 0;;    
    float sum_tat=0,sum_wt=0,sum_rt=0,total_idle_time=0,length_cycle,prev=0;
    float cpu_utilization;
    int max_completion_time,min_arrival_time;
    
    if(input){
	    cout << fixed << setprecision(2);
	    cout<<"Enter total number of processes: ";
	    cin>>n;
	    for(int i=0;i<n;i++)
	    {
	        cout<<"\nEnter Process" <<i<< "Arrival Time: ";
	        cin >> global[i].at;
	        global[i].pid=i;
	    }
	    
	    for(int i=0;i<n;i++)
	    {
	        cout<<"\nEnter Process" <<i<< "Burst Time: ";
	        cin >> global[i].bt;
	        bt_remaining[i]= global[i].bt;
	    }
	}
	else{
		for(int i=0;i<n;i++)
	    {
	        bt_remaining[i]= global[i].bt;
	    }
	}
    
    while(completed!=n)
    {
        //find process with min. burst time in ready queue at current time
        int min_index = -1;
        int minimum = INT_MAX;
        for(int i = 0; i < n; i++) {
            if(global[i].at <= current_time && is_completed[i] == false) {
                if(bt_remaining[i] < minimum) {
                    minimum = bt_remaining[i];;
                    min_index = i;
                }
                if(bt_remaining[i]== minimum) {
                    if(global[i].at < global[min_index].at) {
                        minimum= bt_remaining[i];;
                        min_index = i;
                    }
                }
            }
        }

    
        if(min_index==-1)
        {
            current_time++;
        }
        else
        {
            if(bt_remaining[min_index] == global[min_index].bt)
            {
                        global[min_index].start_time = current_time;
                        total_idle_time += (is_first_process==true) ? 0 : (global[min_index].start_time -  prev);
                        is_first_process=false;
            }
            bt_remaining[min_index] -= 1;
            current_time++; 
            prev=current_time;
            if(bt_remaining[min_index] == 0)
            {
                global[min_index].ct = current_time;
                global[min_index].tat = global[min_index].ct - global[min_index].at;
                global[min_index].wt= global[min_index].tat - global[min_index].bt;
                global[min_index].rt = global[min_index].start_time - global[min_index].at;
            
            
                sum_tat +=global[min_index].tat;
                sum_wt += global[min_index].wt;
                sum_rt += global[min_index].rt;
                completed++;
                is_completed[min_index]=true;       
                
                
            }
        }
    }
    //Calculate Length of Process completion cycle
    max_completion_time = INT_MIN;
    min_arrival_time = INT_MAX;
    for(int i=0;i<n;i++)
    {
        max_completion_time = max(max_completion_time,global[i].ct);
        min_arrival_time = min(min_arrival_time,global[i].at);
    }
    length_cycle = max_completion_time - min_arrival_time;

    
     //Output
     if(input){
     	cout<<"\nProcess No.\tAT\tCPU Burst Time\tCT\tTAT\tWT\tRT\n";	
	 }
    for(int i=0;i<n;i++){
     if(input){
	 	cout<<i<<"\t\t"<<global[i].at<<"\t"<<global[i].bt<<"\t\t"<<global[i].ct<<"\t"<<global[i].tat<<"\t"<<global[i].wt<<"\t"<<global[i].rt<<endl;
 	 }
	 global[i].start_time = 0;
     global[i].ct = 0;
     global[i].tat = 0;
     global[i].wt = 0;
     global[i].rt = 0;
	}
     cout<<endl;
    cpu_utilization = (float)(length_cycle - total_idle_time)/ length_cycle;

    if(input){
    	cout<<"\nAverage Turn Around time= "<< (float)sum_tat/n;
	    cout<<"\nAverage Waiting Time= "<<(float)sum_wt/n;
	    cout<<"\nAverage Response Time= "<<(float)sum_rt/n;    
	    cout<<"\nThroughput= "<<n/(float)length_cycle;
	    cout<<"\nCPU Utilization(Percentage)= " << cpu_utilization*100;
	}
    avg_tat[0] = (float)sum_tat/n;
    avg_wt[0] = (float)sum_wt/n;
    avg_rt[0] = (float)sum_rt/n;
    avg_tp[0] = n/(float)length_cycle;
    avg_util[0] = cpu_utilization*100;
    return 0;
}

int main()
{	
	int i = 9 ;
	while(i!=0){
		cout << "\n\n1 - Shortest Remaining Time First" <<endl;
		cout << "2 - First Come First Serve" << endl;
		cout << "3 - Round Robin CPU" << endl;
		cout << "4 - Shortest Job First" << endl;
		cout << "5 - Statistic" << endl;
		cout << "\n0 - Exit" <<endl;
		cout <<  "\n\nSelect An algorithm : ";
		cin >> i;
		if(i==1){
			cout << "\n\nShortest Remaining Time First (Selected)" <<endl;
			srtf(true,0);
		}
		if(i==2){
			cout << "\n\nFirst Come First Serve (Selected)" << endl;
			fcfs(true,0);
		}
		if(i==3){
			cout << "\n\nRound Robin CPU (Selected)" << endl;
			rr(true,0,0);
		}
		if(i==4){
			cout << "\n\nShortest Job First (Selected)" << endl;
			sjf(true,0);
		}
		if(i=5){
			cout << "\n\nCheck Statistic" << endl;
			int n,tq;
			float min=0.0,max=0.0,avg=0.0;
			cout<<"Enter total number of processes: ";
		    cin>>n;
		    for(int i=0;i<n;i++)
		    {
		        cout<<"\nEnter Process" <<i<< "Arrival Time: ";
		        cin >> global[i].at;
		        global[i].pid=i;
		    }
		    
		    for(int i=0;i<n;i++)
		    {
		        cout<<"\nEnter Process" <<i<< "Burst Time: ";
		        cin >> global[i].bt;
		    	global[i].bt_remaining= global[i].bt;
		    }
		    cout<<"\nEnter time quanta (some algorithm need it): ";
			cin>>tq;
			srtf(false,n);
			fcfs(false,n);
		    sjf(false,n);
		    rr(false,tq,n);
		    min = (float)avg_wt[0];
		    for(int i=0;i<4;i++){
		    	if((float)avg_wt[i] <= (float)min){
		    		min = avg_wt[i];
				}
			}
		    max = (float)avg_wt[0];
			for(int i=0;i<3;i++){
		    	
		    	if(max <= (float)avg_wt[i]){
		    		max = avg_wt[i];
				}
			}
		    for(int i=0;i<4;i++){
		    	avg += avg_wt[i];
			}
			avg = avg/4;
			cout << "\t\t\t\t\t\tStats\n" << endl;
			cout << "Algorithm\tTurnAround_Time\tRound_Time\tWaiting_Time\tThroughput\tUtilization\tSuggestion" << endl;
		    for(int i=0;i<4;i++){
			    if(i==0){
					cout << "SRTF\t" ;
				}
				if(i==1){
					cout << "FCFS\t" ;
				}
				if(i==2){
					cout << "RR\t" ;
				}
				if(i==3){
					cout << "SJF\t" ;
			    }
			    cout << "\t" <<avg_tat[i] <<"\t\t" << avg_rt[i] << "\t\t" << avg_wt[i] << "\t\t" << FIXED_FLOAT(avg_tp[i]) << "\t\t" << avg_util[i] << "\t\t";
			    if(avg_wt[i] == min){
			    	cout << suggestions[0] << endl;
				}
				else if(avg_wt[i] == max){
			    	cout << suggestions[3] << endl;
				}
				else{
					if(avg_wt[i] > avg){
			    		cout << suggestions[2] << endl;
					}
					else{
						cout << suggestions[1] << endl;
					}
				}
			}
		}
	}
}
