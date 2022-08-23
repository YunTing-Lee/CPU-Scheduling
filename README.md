# CPU-Scheduling

## 使用流程 :
1. 編譯並執行
2. 螢幕會顯示提示字元 :「請輸入檔案名稱(eg.input1, input2,......)[0]Quit :」，請使用者輸入檔名，不用加上附檔名，若輸入的檔案找不到，則會顯示「###檔名does not exist###」，請使用者再輸入一次，直到輸入正確為止開始執行對應的方法或是輸入0自動結束程式。

## input檔案格式
- 第1列的第1個數字為方法，有方法1~方法6  
- 第1列的第2個數字為Time Slice，範圍不定  
- 第2列的欄位名稱為 ProcessID, CPU Burst, Arrival Time, Priority  
- 第3列開始每行依序為[ProcessID][CPU Burst][Arrival Time][Priority]，且皆為正整數。不同欄位之間會用"空白"或是"tab"隔開  

## output檔案格式 : 
- output輸出檔名 : "out_" + {input檔名}  
- 第1個區段為Gantt Chart(甘特圖)
	* 第一行先輸出方法名稱，接著輸出該方法對應的Gantt Chart，若方法名稱為All，則依序輸出FCFS, RR, SRTF, PPRR, HRRN的Gantt Chart
	* ID為0-9, A-Z(由10開始為A依序向下代表，Process ID不會超過36個)，若該時間沒有Process在執行則由減號(-)來代表  
![image](https://github.com/YunTing-Lee/CPU-Scheduling/blob/main/Picture/Gantt%20Chart.PNG)

- 第2個區段顯示各個Process ID在各個方法(可能數個)的Waiting Time
![image](https://github.com/YunTing-Lee/CPU-Scheduling/blob/main/Picture/Waiting%20Time.PNG)

- 第3個區段顯示各個Process ID在各個方法(可能數個)的Turnaround Time
![image](https://github.com/YunTing-Lee/CPU-Scheduling/blob/main/Picture/Turnaround%20Time.PNG)

## 方法1 : First Come Fist Serve(FCFS)
### 處理原則 : 
- 依 Arrival Time 的先後次序進行排程
- 若Arrival Time相同時，則依Process ID由小至大依序處理

## 方法2 : Round Robin(RR)
### 處理原則 : 
- 依 Arrival Time 的先後次序進行排程，時候未到的Process不能執行
- 若Arrival Time相同時，則依Process ID由小至大依序處理
- 當Timeout時，被換下的Process要從佇列尾端開始排序，若恰巧有新來的Process，則讓新來的Process排在前面
- 若Process的Time Slice未用完就結束時，就必須讓下一個Process執行，且擁有完整的Time Slice

## 方法3 : Shortest Remaining Time First(SRTF)
### 處理原則 : 
- 由剩餘CPU Burst最小的Process先排序
- 若剩餘CPU Burst相同的Process不只一個，則依Arrival Time小的先處理
- 若剩餘CPU Burst相同且Arrival Time也相同時，則依Process ID由小至大依序處理

## 方法4 : Preemptive Priority Round Robin(PPRR)
### 處理原則 : 
- Preemptive Priority Round Robin
- 依Priority大小依序處理，Priority Number小的Process代表優先處理
- 若Priority相同的Process不只一個，則採用Round Robin(RR)原則進行排程 : 
	* 若有Priority相同的Process正在執行中，則需等待其Time Slice用罄
	* 當Timeout或被Preemptive時，從佇列尾端開始依Priority大小排序，若恰巧有新來的Process，則讓新來的Process排在前面
	
## 方法5 : Highest Response Ratio Next(HRRN)
### 處理原則 : 
- 反應時間比率(Response Ratio)越高的Process優先處理
- 若Ratio相同的Process不只一個，則依Arrival Time小的先處理
- 若Ratio相同且Arrival Time也相同時，則依Process ID由小至大依序處理

## 方法6 : All
- 以上五種方法都要執行
