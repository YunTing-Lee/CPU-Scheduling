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
	- 第一行先輸出方法名稱，接著輸出該方法對應的Gantt Chart，若方法名稱為All，則依序輸出FCFS, RR, SRTF, PPRR, HRRN的Gantt Chart
	- ID為0-9, A-Z(由10開始為A依序向下代表，Process ID不會超過36個)，若該時間沒有Process在執行則由減號(-)來代表  
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
	- 若有Priority相同的Process正在執行中，則需等待其Time Slice用罄
	- 當Timeout或被Preemptive時，從佇列尾端開始依Priority大小排序，若恰巧有新來的Process，則讓新來的Process排在前面
	
## 方法5 : Highest Response Ratio Next(HRRN)
### 處理原則 : 
- 反應時間比率(Response Ratio)越高的Process優先處理
- 若Ratio相同的Process不只一個，則依Arrival Time小的先處理
- 若Ratio相同且Arrival Time也相同時，則依Process ID由小至大依序處理

## 方法6 : All
- 以上五種方法都要執行

## 結果與討論
不同排程法的比較( 紅色 : 時間最久	藍色 : 時間最短 )
- 平均等待時間（Average Waiting Time )  
![image](https://github.com/YunTing-Lee/CPU-Scheduling/blob/main/Picture/Compare%20Waiting%20Time.png)
- 工作往返時間（Turnaround Time） 
![image](https://github.com/YunTing-Lee/CPU-Scheduling/blob/main/Picture/Compare%20Turnaround%20Time.png)

因為工作往返時間就是等待時間加上CPU Burst Time，所以所得到的不同排程法的平均等待時間與平均工作往返時間的大小順序會是一樣的。因此以下統一以平均等待時間來討論 :  
由這次的測試數據，所得出的**平均等待時間結果大小順序大約為PPRR > RR > FCFS > HRRN > SRTF**

### 方法一 First Come First Serve : ( Non-preemptive、不會Starvation )
    在這次的測試數據中，FCFS的平均等待時間大概落在中間的位置。通常來說，使用FCFS的平均等待時間會較長。因FCFS的缺點是當有一個先到達的process有很長的CPU Burst Time時，那麼在他後面抵達的所有process就都必須要等待一段很長的時間，會導致平均等待時間大幅增加。因這次實驗的測試數據中沒有這種情況，所以FCFS所得出的平均等待時間就不會特別大。而FCFS也較公平，並且不會發生starvation，每個process只要等待比他先來的process做完就一定會換到他做。

### 方法二 Round Robin : ( Preemptive、不會Starvation )
    從比較的表中可以發現RR演算法的效能很大程度依賴於time slice所選擇的大小。當RR所選擇得time slice太小時，因為每個process都要分很多次才能執行完畢，而在實務上，小的time slice也會增加Context Switch的次數，Context switch太頻繁，會導致CPU Time未真正用在process的執行上，導致throughput變低，導致效率變差。但若time slice選擇大到大於process的最大CPU Burst Time時，RR排程法其實就會等同於FCFS排程法，變成沒有在分時執行，所以RR的time slice選擇不應太小也不應太大。RR為最公平的演算法，且雖然是preemptive 但不會發生Starvation，因為只要process到達後在ready queue排隊，就不會被插隊，也因此一定輪的到他執行。雖然RR等待時間會被分散在每個process上，但因為總等待時間也被拉得較長，所以平均等待時間也較久。

### 方法三 Shortest Remaining Time First : ( Preemptive、會Starvation )
    從比較的表中可以看到SRTF不管在甚麼情況下的平均等待時間都會是最小的，所以若以平均等待時間來評估方法的好壞，SRTF是在這之中最佳的演算法，但因為SRTF是以剩餘 CPU Burst Time最小的process來排程，所以若持續有CPU Burst Time較小的process到來，那麼CPU Burst Time大的process就可能永遠無法使用CPU，因為需要長時間的process會一直被無限推遲，導致starvation問題發生，因此SRTF對長時間的作業不利。且SRTF排程法有一個前提是必須要事先知道每一個process要花費多少時間，但是因為在實務上每個process的CPU Burst Time很難預測，所以這個排程演算法沒那麼實用。

### 方法四 Preemptive Priority Round Robin : ( Preemptive、會Starvation )
    在這次的測試數據中，PPRR的平均等待時間為較久的一個，原因是它是依process的priority大小依序執行，所以若還有比自己小的priority沒做完，就無法輪到自己做，所以較早抵達的process就可能需要等待很久，因此整個總等待時間被拉長，導致平均等待時間為這幾個演算法中最長的。這也顯現出PPRR會造成低優先權的process一直處於等待CPU的狀態，導致Starvation的情況發生，而可以解決這個狀況的方法有Aging，讓process可以隨著等待時間越久就提升優先權，讓低優先權的process可以越來越有機會執行。

### 方法五 Highest Response Ratio Next : ( Non-preemptive、不會Starvation)
  在這次的測試數據中，HRRN的平均等待時間大概落在第二小的位置。僅次於SRTF，而HRRN可以說是改良SJF的排程法，以反應時間比率當作變動優先等級來排程，Response Ratio = 1 + (Waiting Time / CPU Burst Time)，因此當一個process等得越久，他所算出來的反應時間比率就會越高，因此priority也會越高，也就更有機會先使用到CPU，從而讓process不會starvation，因為這就是HRRN的特性，需要工作很久的process經過等待時間，就可以逐漸提高priority，而不會被無限延宕。

