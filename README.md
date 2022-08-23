# CPU-Scheduling

## input檔案格式
第1列的第1個數字為方法，有方法1~方法6  
第1列的第2個數字為Time Slice，範圍不定  
第2列的欄位名稱為 ProcessID, CPU Burst, Arrival Time, Priority  
第3列開始每行依序為[ProcessID][CPU Burst][Arrival Time][Priority]，且皆為正整數。不同欄位之間會用"空白"或是"tab"隔開  

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
