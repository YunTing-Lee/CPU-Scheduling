#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <time.h>
#include <map>
#include <queue>
#include <algorithm>
#include <iterator>

using namespace std;

struct Process{
  int pid ;
  char chPid ; // 10以上會變字母
  int cpuBurst ;
  int remainCpuBurst ; // 剩餘需要的CPU時間
  int arrivalTime ;
  int priority ;
  double responseRatio ;
};

class ProcessTime {
public :
  int mWaitingTime ;
  int mTurnaroundTime ;

  ProcessTime() {}
  ProcessTime( int wait, int turnaround ) {
    mWaitingTime = wait ;
    mTurnaroundTime = turnaround ;
  } // constructor

};

class CPU_Scheduler {

public :

  vector<Process> mProcessList ;
  queue<Process> mUnArrivalProcess ; // 記錄在現在的時間還未到達的process
  queue<Process> mReadyQueue ; // 在等待CPU的process
  int mTimeSlice ;
  int mCurrentTime ;

  CPU_Scheduler( vector<Process> inputProcess, int inputTimeSlice ) {
    mProcessList = inputProcess ;
    mTimeSlice = inputTimeSlice ;
    mCurrentTime = 0 ;
    MergeSort( 0, mProcessList.size()-1 ) ;
    for( int i = 0 ; i < mProcessList.size() ; i++ ) // 把原本儲存在vector的放進queue裡
      mUnArrivalProcess.push( mProcessList[i] ) ;
  } // constructor

  CPU_Scheduler(){}

  vector<char> grantChart ;
  map<int, ProcessTime> mProcessTime ; // pid,

  void MergeSort( int first, int last ) ;
  void MergeData( int first, int mid, int last ) ;
  void WriteSortedData( string name ) ;

};

class FCFS : public CPU_Scheduler {
public :

  FCFS( vector<Process> inputProcess, int inputTimeSlice ) : CPU_Scheduler( inputProcess, inputTimeSlice ) {}
  void FisrtComeFisrtServe() ;

};

class RR : public CPU_Scheduler {
public :

  RR( vector<Process> inputProcess, int inputTimeSlice ) : CPU_Scheduler( inputProcess, inputTimeSlice ) {}
  void RoundRobin() ;
  void SubmitNewProcessToReadyQueue() ;

};

class SRTF : public CPU_Scheduler{
public :
  vector<Process> mUnDone ;
  SRTF( vector<Process> inputProcess, int inputTimeSlice ) : CPU_Scheduler( inputProcess, inputTimeSlice ) {}
  void ShorestRemainTimeFisrt() ;
  void SubmitNewProcessToReadyQueue() ;
  void InsertionSort( Process newProcess ) ;
  void MergeSort( int first, int last ) ;
  void MergeData( int first, int mid, int last ) ;

};

class PPRR : public CPU_Scheduler{
public :
  vector<Process> mUnDone ;
  vector< queue<Process> > mRRQueue ;

  void PreemptivePriortyRoundRobin() ;
  void MergeSort( int first, int last )  ;
  void MergeData( int first, int mid, int last ) ;

  void RoundRobin() ;
  void DoneThisProcess( Process & process ) ;
  void DoRRQueueFront( int timeSlice ) ;
  void ReLineUp() ;
  void InsertionSort( Process newProcess )  ;
  void SubmitNewProcessToReadyQueue() ;

  PPRR( vector<Process> inputProcess, int inputTimeSlice ) {
    mProcessList = inputProcess ;
    mTimeSlice = inputTimeSlice ;
    mCurrentTime = 0 ;
    MergeSort( 0, mProcessList.size()-1 ) ;
    for( int i = 0 ; i < mProcessList.size() ; i++ ) // 把原本儲存在vector的放進queue裡
      mUnArrivalProcess.push( mProcessList[i] ) ;
  } // constructor


};

class HRRN : public CPU_Scheduler{

public :
  vector<Process> mUnDone ;
  HRRN( vector<Process> inputProcess, int inputTimeSlice ) : CPU_Scheduler( inputProcess, inputTimeSlice ) {}
  void HighestResponseRatioNext() ;
  void SubmitNewProcessToReadyQueue() ;
  void CalResponseRatio() ;

};

void Enter() ;
void ReadFile( string & inputFullName, string & fileId , bool & isQuit, int & command, int & timeSlice,
               vector<Process> & inputProcess  ) ;
void ReadUntilEnter( fstream & file ) ;
void ReadInputFileName( string & fileId,string & inputFullName ) ; // 讀input檔編號
bool OpenInputFile( string & fileId,string & inputFullName,fstream & inputFile ) ;  // 開啟input檔
void ReadInputData( fstream & file, vector<int> & unsortedNum ) ;//將input檔裡的資料讀進vector<School> sch裡
string ReadString(fstream & file); // 從file裡讀一個string(以'\t'做分割)
void Swap( vector<int> & num, int a, int b ) ; // 將vector裡第a個元素與第b個元素做交換
int StringToInt( string str ) ;
void ReadANum ( int & userNum ) ;
char ConvertPid( int pid ) ;
void WriteResult(  string name, int command, CPU_Scheduler scheduler ) ;
void WriteAllResult( string name, FCFS fcfs, RR rr, SRTF srtf, PPRR pprr, HRRN hrrn ) ;
void AverageWaitingAndTurnaroundTime(  CPU_Scheduler scheduler, float & averageWaiting, float & averageTurnaround ) ;
void AllAverageWaitingAndTurnaroundTime( FCFS fcfs, RR rr, SRTF srtf, PPRR pprr, HRRN hrrn ) ;

int main() {

    string inputFullName, fileId ;
    bool isQuit = false ;
    fstream inputFile ;
    int command ;
    int timeSlice ;
    vector<Process> inputProcess ;

    ReadFile( inputFullName, fileId, isQuit, command, timeSlice, inputProcess ) ;


    while( ! isQuit ) {
      if ( command == 1 ) {
        FCFS fcfsScheduler( inputProcess, timeSlice );
        fcfsScheduler.FisrtComeFisrtServe() ;
        WriteResult( fileId, command, fcfsScheduler ) ;
        float fcfsAverageWaiting, fcfsAverageTurnaround ;
        AverageWaitingAndTurnaroundTime( fcfsScheduler, fcfsAverageWaiting, fcfsAverageTurnaround ) ;
        cout << "FCFS Average Waiting Time : " << fcfsAverageWaiting << endl << endl ;
        cout << "FCFS Average Turnaround Time : " << fcfsAverageTurnaround << endl << endl ;
      } // if
      else if ( command == 2 ) {
        RR rrScheduler( inputProcess, timeSlice ) ;
        rrScheduler.RoundRobin() ;
        WriteResult( fileId, command, rrScheduler ) ;
        float rrAverageWaiting, rrAverageTurnaround ;
        AverageWaitingAndTurnaroundTime( rrScheduler, rrAverageWaiting, rrAverageTurnaround ) ;
        cout << "RR Average Waiting Time : " << rrAverageWaiting << endl << endl ;
        cout << "RR Average Turnaround Time : " << rrAverageTurnaround << endl << endl ;
      } // else if
      else if ( command == 3 ) {
        SRTF srtfScheduler( inputProcess, timeSlice ) ;
        srtfScheduler.ShorestRemainTimeFisrt() ;
        WriteResult( fileId, command, srtfScheduler ) ;
        float srtfAverageWaiting, srtfAverageTurnaround ;
        AverageWaitingAndTurnaroundTime( srtfScheduler, srtfAverageWaiting, srtfAverageTurnaround ) ;
        cout << "SRTF Average Waiting Time : " << srtfAverageWaiting << endl << endl ;
        cout << "SRTF Average Turnaround Time : " << srtfAverageTurnaround << endl << endl ;
      } // else if
      else if ( command == 4 ) {
        PPRR pprrScheduler( inputProcess, timeSlice ) ;
        pprrScheduler.PreemptivePriortyRoundRobin() ;
        WriteResult( fileId, command, pprrScheduler ) ;
        float pprrAverageWaiting, pprrAverageTurnaround ;
        AverageWaitingAndTurnaroundTime( pprrScheduler, pprrAverageWaiting, pprrAverageTurnaround ) ;
        cout << "PPRR Average Waiting Time : " << pprrAverageWaiting << endl << endl ;
        cout << "PPRR Average Turnaround Time : " << pprrAverageTurnaround << endl << endl ;
      } // else if
      else if ( command == 5 ) {
        HRRN hrrnScheduler( inputProcess, timeSlice ) ;
        hrrnScheduler.HighestResponseRatioNext() ;
        WriteResult( fileId, command, hrrnScheduler ) ;
        float hrrnAverageWaiting, hrrnAverageTurnaround ;
        AverageWaitingAndTurnaroundTime( hrrnScheduler, hrrnAverageWaiting, hrrnAverageTurnaround ) ;
        cout << "HRRN Average Waiting Time : " << hrrnAverageWaiting << endl << endl ;
        cout << "HRRN Average Turnaround Time : " << hrrnAverageTurnaround << endl << endl ;
      } // else if
      else if ( command == 6 ) {

        FCFS fcfsScheduler( inputProcess, timeSlice );
        fcfsScheduler.FisrtComeFisrtServe() ;

        RR rrScheduler( inputProcess, timeSlice ) ;
        rrScheduler.RoundRobin() ;

        SRTF srtfScheduler( inputProcess, timeSlice ) ;
        srtfScheduler.ShorestRemainTimeFisrt() ;

        PPRR pprrScheduler( inputProcess, timeSlice ) ;
        pprrScheduler.PreemptivePriortyRoundRobin() ;

        HRRN hrrnScheduler( inputProcess, timeSlice ) ;
        hrrnScheduler.HighestResponseRatioNext() ;

        WriteAllResult( fileId, fcfsScheduler, rrScheduler, srtfScheduler, pprrScheduler, hrrnScheduler ) ;
        AllAverageWaitingAndTurnaroundTime( fcfsScheduler, rrScheduler, srtfScheduler, pprrScheduler, hrrnScheduler ) ;

      } // else if
      else {
        cout << "The command is not correct.It can be only 1-6." << endl ;
      } // else

      ReadFile( inputFullName, fileId, isQuit, command, timeSlice, inputProcess ) ;
    } // while

    cout << "Program end..." << endl ;
    return 0;
} // main()


void ReadFile( string & inputFullName, string & fileId , bool & isQuit, int & command, int & timeSlice,
               vector<Process> & inputProcess  ) {
  fstream inputFile ;
  ReadInputFileName( fileId,inputFullName ) ;   // 讀檔案編號
  if ( ! OpenInputFile( fileId, inputFullName, inputFile ) )  // 開啟檔案，若檔案編號是0則直接結束重新讀command
    isQuit = true ;
  else {
    // 讀第一列的兩個數字( command, timeslice )

    inputFile >> command >> timeSlice ;

    // cout << "command : " << command << "  timeSlice : " << timeSlice << endl ;


    ReadUntilEnter( inputFile ) ;
    string line ;
    getline( inputFile, line ) ; // 讀掉標題


    inputProcess.clear() ;
    Process aProcess ;
    while( inputFile >> aProcess.pid ) {
      inputFile >> aProcess.cpuBurst ;
      inputFile >> aProcess.arrivalTime ;
      inputFile >> aProcess.priority ;
      aProcess.chPid = ConvertPid(  aProcess.pid  ) ;
      aProcess.remainCpuBurst = aProcess.cpuBurst ;
      ReadUntilEnter( inputFile ) ;
      inputProcess.push_back( aProcess ) ;
    } // while
    inputFile.close() ;
    isQuit = false ;
  } // else
} // ReadUserInput

void Enter() {        // 讀到'\n'為止
  char enter ;
  cin.get(enter) ;
  while( enter != '\n' )
    cin.get(enter) ;
} // Enter()

void ReadUntilEnter( fstream & file ) {

  char aChar = file.get();
  while( aChar != '\n' ) {
    aChar = file.get();
  }
} // ReadUntilEnter

void ReadInputFileName( string & fileId,string & inputFullName ){
  cout << "請輸入檔案名稱(eg.input1, input2,......)[0]Quit : " ;
  cin >> fileId ;
  char ch ;
  cin.get(ch);
  if ( ch != '\n')
    Enter() ;
  inputFullName = fileId + ".txt" ;
} // ReadInputFileName

bool OpenInputFile( string & fileId, string & inputFullName, fstream & inputFile ){

  if ( fileId.compare("0") == 0 )  // 如果編號是0，直接結束，重新讀command
   return false ;
  else {
    inputFile.open( inputFullName.c_str(), ios::in ) ;
    while ( ! inputFile ) {    // 做到檔案成功開啟
      cout << "### " << inputFullName << " does not exist ### " << endl ;
      fileId = "" ;
      ReadInputFileName( fileId,inputFullName ) ;
      if ( fileId.compare("0") == 0 )   // 如果編號是0，直接結束，重新讀command
        return false ;
      inputFile.open( inputFullName.c_str(), ios::in) ;
    } // while
  } // else
  return true ;
} // OpenFile()


void FCFS :: FisrtComeFisrtServe() {

  int currentTime = 0 ;
  for( int i = 0 ; i < mProcessList.size() ; i++ ) {
    if ( currentTime < mProcessList[i].arrivalTime ) {
       int idleTime = mProcessList[i].arrivalTime - currentTime ;
       grantChart.insert( grantChart.end(), idleTime, '-' ) ;
       currentTime += idleTime ;
    } // if

    grantChart.insert( grantChart.end(), mProcessList[i].cpuBurst, mProcessList[i].chPid ) ;
    currentTime += mProcessList[i].cpuBurst ;

    ProcessTime pt( currentTime - mProcessList[i].arrivalTime- mProcessList[i].cpuBurst, currentTime - mProcessList[i].arrivalTime ) ;

    mProcessTime.insert(pair<int, ProcessTime>( mProcessList[i].pid, pt ) );
  } // for

} // FCFS :: FisrtComeFisrtServe()


void RR :: RoundRobin() {
  mCurrentTime = 0 ; // 現在時間

  while( ! mReadyQueue.empty() ||  ! mUnArrivalProcess.empty() ) { // readyQueue還有process在等待或是還有process未到達

    // readyQueue是空的，還有prcoess未到達，現在時間小於下一個process到達的時間
    if ( mReadyQueue.empty() && ! mUnArrivalProcess.empty() && mCurrentTime < mUnArrivalProcess.front().arrivalTime ) {
      int idleTime = mUnArrivalProcess.front().arrivalTime - mCurrentTime ;  // CPU需閒置的時間
      grantChart.insert( grantChart.end(), idleTime, '-' ) ;
      mCurrentTime += idleTime ;
    } // if
    else if ( mReadyQueue.empty() && ! mUnArrivalProcess.empty() && mCurrentTime >= mUnArrivalProcess.front().arrivalTime ) {
      SubmitNewProcessToReadyQueue() ;
    } // else if
    else if ( ! mReadyQueue.empty() ) { // readyQueue還有process在等待執行

      Process nowProcess = mReadyQueue.front() ;

      if ( nowProcess.remainCpuBurst <= mTimeSlice ) { // 這個process所剩餘的時間小於等於一個time slice，這次做完就結束了

        grantChart.insert( grantChart.end(), nowProcess.remainCpuBurst, nowProcess.chPid ) ;
        mCurrentTime += nowProcess.remainCpuBurst;
        nowProcess.remainCpuBurst = 0 ;
        ProcessTime pt( mCurrentTime - nowProcess.arrivalTime - nowProcess.cpuBurst, mCurrentTime - nowProcess.arrivalTime ) ;
        mProcessTime.insert(pair<int, ProcessTime>( nowProcess.pid, pt ) );
        mReadyQueue.pop() ;
        SubmitNewProcessToReadyQueue() ;
      } // if
      else { // 這次time slice做不完
        grantChart.insert( grantChart.end(), mTimeSlice, nowProcess.chPid ) ;
        mCurrentTime += mTimeSlice;
        nowProcess.remainCpuBurst -= mTimeSlice ;
        SubmitNewProcessToReadyQueue() ;
        mReadyQueue.pop() ;
        mReadyQueue.push( nowProcess ) ; // 因為還沒做完，所以把他加到最後面
      } // else
    } // else if
  } // while

} // RRr :: RoundRobin()

void RR :: SubmitNewProcessToReadyQueue() {

  while ( ! mUnArrivalProcess.empty() && mCurrentTime >= mUnArrivalProcess.front().arrivalTime ) {
    mReadyQueue.push( mUnArrivalProcess.front() ) ;
    mUnArrivalProcess.pop() ;
  } // while

} // RR :: SubmitProcessToReadyQueue()


void SRTF :: ShorestRemainTimeFisrt() {
  mCurrentTime = 0 ; // 現在時間

  while( ! mUnDone.empty() || ! mUnArrivalProcess.empty() ) {

    if ( mUnDone.empty() && mCurrentTime < mUnArrivalProcess.front().arrivalTime ) {
      int idleTime = mUnArrivalProcess.front().arrivalTime - mCurrentTime ;  // CPU需閒置的時間
      grantChart.insert( grantChart.end(), idleTime, '-' ) ;
      mCurrentTime += idleTime ;
    } // if
    else if ( mUnDone.empty() && mCurrentTime >= mUnArrivalProcess.front().arrivalTime ) {
      SubmitNewProcessToReadyQueue() ;
    } // else if
    else if ( ! mUnDone.empty() ) {

      // 找 remain CPU Burst 最少的做
      if ( ! mUnArrivalProcess.empty() ) {
        int nextArrival = mUnArrivalProcess.front().arrivalTime ;
        int nowRemainCpuBurst = mUnDone.front().remainCpuBurst ;
        int timeSlice = nextArrival - mCurrentTime ;

        if ( nowRemainCpuBurst <= timeSlice ) { // 可以全部做完
          grantChart.insert( grantChart.end(), nowRemainCpuBurst, mUnDone.front().chPid ) ;
          mCurrentTime += nowRemainCpuBurst ;
          mUnDone.front().remainCpuBurst = 0 ;
          ProcessTime pt( mCurrentTime - mUnDone.front().arrivalTime - mUnDone.front().cpuBurst, mCurrentTime - mUnDone.front().arrivalTime ) ;
          mProcessTime.insert( pair<int, ProcessTime>( mUnDone.front().pid, pt ) );
          mUnDone.erase( mUnDone.begin() ) ;
          SubmitNewProcessToReadyQueue() ;
        } // if
        else {

          grantChart.insert( grantChart.end(), timeSlice, mUnDone.front().chPid ) ;
          mUnDone.front().remainCpuBurst -= timeSlice ;
          mCurrentTime += timeSlice ;
          SubmitNewProcessToReadyQueue() ;
        } // else
      } // if
      else { // 全部的process都已經到達，皆放入unDo中

        int nowRemainCpuBurst = mUnDone.front().remainCpuBurst ;
        grantChart.insert( grantChart.end(), nowRemainCpuBurst, mUnDone.front().chPid ) ;
        mCurrentTime += nowRemainCpuBurst ;
        mUnDone.front().remainCpuBurst = 0 ;
        ProcessTime pt( mCurrentTime - mUnDone.front().arrivalTime - mUnDone.front().cpuBurst, mCurrentTime - mUnDone.front().arrivalTime ) ;
        mProcessTime.insert( pair<int, ProcessTime>( mUnDone.front().pid, pt ) );
        mUnDone.erase( mUnDone.begin() ) ;
      } // else
    } // else if
  } // while

} // SRTF :: ShorestRemainTimeFisrt()

void SRTF :: SubmitNewProcessToReadyQueue() {

  while ( ! mUnArrivalProcess.empty() && mCurrentTime >= mUnArrivalProcess.front().arrivalTime ) {
    InsertionSort( mUnArrivalProcess.front() ) ;
    mUnArrivalProcess.pop() ;
  } // while

} // SRTF :: SubmitProcessToReadyQueue()

void SRTF :: InsertionSort( Process newProcess ) {

  int newRemainCpu = newProcess.remainCpuBurst ;
  int newArrival = newProcess.arrivalTime ;
  int newPid = newProcess.pid ;

  int i = 0 ;
  for ( i = 0 ; i < mUnDone.size() ; i++ ) {
    if( ( newRemainCpu < mUnDone[i].remainCpuBurst ) ||
        ( newRemainCpu == mUnDone[i].remainCpuBurst && newArrival < mUnDone[i].arrivalTime ) ||
        ( newRemainCpu == mUnDone[i].remainCpuBurst && newArrival == mUnDone[i].arrivalTime && newPid < mUnDone[i].pid ) ) {
      mUnDone.insert( mUnDone.begin()+i, newProcess ) ;
      return ;
    } // if
  } // for

  mUnDone.insert( mUnDone.begin()+i, newProcess ) ;
} // SRTF :: InsertionSort()


void PPRR :: PreemptivePriortyRoundRobin() {


  mCurrentTime = 0 ; // 現在時間

  while( ! mUnDone.empty() || ! mUnArrivalProcess.empty() || mRRQueue.size() != 0  ) {

    // 現在沒有process要執行，等待下一個process抵達
    if ( mUnDone.empty() && mRRQueue.size() == 0 && mCurrentTime < mUnArrivalProcess.front().arrivalTime ) {
      int idleTime = mUnArrivalProcess.front().arrivalTime - mCurrentTime ;  // CPU需閒置的時間
      grantChart.insert( grantChart.end(), idleTime, '-' ) ;
      mCurrentTime += idleTime ;
    } // if
    else if ( mUnDone.empty() && mRRQueue.size() == 0 && mCurrentTime >= mUnArrivalProcess.front().arrivalTime ) {
      SubmitNewProcessToReadyQueue() ;
    } // else if
    else if ( ! mUnDone.empty() && ! mRRQueue.empty() ) { // rrQueue有人在排隊，mUnDone也有人在排隊，看哪個優先權較高
      int mUnDonePriority = mUnDone.front().priority ;

      if ( mUnDonePriority < mRRQueue[0].front().priority ) { // mUnDone優先權較高

        queue<Process> newRRQueue ;

        while ( ! mUnDone.empty() && mUnDone.front().priority == mUnDonePriority ) { // 把mUnDone內優先權一樣的放進queue裡
          newRRQueue.push( mUnDone.front() ) ;
          mUnDone.erase( mUnDone.begin() ) ;
        } // while
        mRRQueue.insert( mRRQueue.begin(), newRRQueue ) ; // 新的queue優先權較高

      } // if
      else if ( mUnDonePriority == mRRQueue[0].front().priority ) {
        while ( ! mUnDone.empty() && mUnDone.front().priority == mUnDonePriority ) { // 把mUnDone內優先權一樣的放進queue裡
          mRRQueue[0].push( mUnDone[0] ) ;
          mUnDone.erase( mUnDone.begin() ) ; // 有放進queue的就刪掉了
        } // while

      } // else if
      else { // rrQueue內的process優先權較高，rrQueue內的process先做
        RoundRobin() ;
      } // else
    } // else if
    else if ( ! mUnDone.empty() && mRRQueue.empty() ) { // rrQueue沒人在排隊，

      int minPriority = mUnDone.front().priority ;
      queue<Process> newRRQueue ;
      while ( ! mUnDone.empty() && mUnDone.front().priority == minPriority ) { // 把mUnDone內優先權一樣的放進queue裡
        newRRQueue.push( mUnDone.front() ) ;
        mUnDone.erase( mUnDone.begin() ) ;
      } // while

      mRRQueue.push_back( newRRQueue ) ; // 新的queue優先權較高

    } // else if
    else if ( mUnDone.empty() && ! mRRQueue.empty() ) {
      RoundRobin() ;
    } // else if

  } // while


} // PPRR :: PreemptivePriortyRoundRobin()

void PPRR :: RoundRobin() {

  int nextArrival = mUnArrivalProcess.front().arrivalTime ; // 下一個process抵達的時間
  int interval = nextArrival - mCurrentTime ; // 下一個process到之前有多少時間
  int currentPriority = mRRQueue[0].front().priority ;
  int nextPriorty = mUnArrivalProcess.front().priority ;

  // queue內的process都做完了，或是有新的process到達  && ( mCurrentTime < nextArrival && nextPriorty <= currentPriority )
  while ( mRRQueue[0].size() != 0 ) {

    if ( ! mUnArrivalProcess.empty() ) {

      nextArrival = mUnArrivalProcess.front().arrivalTime ; // 下一個process抵達的時間
      nextPriorty = mUnArrivalProcess.front().priority ;
      interval = nextArrival - mCurrentTime ;

      int thisTimeSlice ;

      if ( mTimeSlice <= interval || ( interval <  mTimeSlice && nextPriorty >= currentPriority ) ) // 一次做mTimeSlice
        thisTimeSlice = mTimeSlice ;
      else // 一次做timeslice
        thisTimeSlice = interval ;

      DoRRQueueFront( thisTimeSlice ) ;

      if ( mCurrentTime >= nextArrival ) { // 有process抵達
        if ( nextPriorty < currentPriority ) { // 新的process優先權較高

          SubmitNewProcessToReadyQueue() ;
          ReLineUp() ; // 剛剛做的process如果還沒做完就去重新排隊，如果做完了就移除
          if ( mRRQueue[0].size() == 0 )
            mRRQueue.erase( mRRQueue.begin() ) ;
          return ;
        } // if
        else if ( nextPriorty == currentPriority ) { // 優先權與現在在做的相同，把他加到queue後面

          SubmitNewProcessToReadyQueue() ;
          while ( ! mUnDone.empty() && mUnDone.front().priority == currentPriority ) { // 把mUnDone內優先權一樣的放進queue裡，還沒做過得先排
            mRRQueue[0].push( mUnDone[0] ) ;
            mUnDone.erase( mUnDone.begin() ) ; // 有放進queue的就刪掉了
          } // while
          ReLineUp() ;
        } // else if
        else {
          SubmitNewProcessToReadyQueue() ;
          ReLineUp() ;
        } // else
      } // if
      else {
        ReLineUp() ;
      } // else

    } // if
    else { // 所有的process都已經到達，皆放入mUndone中
      while ( mRRQueue[0].size() != 0  ) {
        DoRRQueueFront( mTimeSlice ) ;
        ReLineUp() ;
      } // while
    } // else
  } // while

  if ( mRRQueue[0].size() == 0 )
    mRRQueue.erase( mRRQueue.begin() ) ;

} // PPRR :: RoundRobin()

void PPRR :: ReLineUp() { // 原本排在queue最前面的process剛剛已經做完一個time slice了，要去重新排隊

  if ( mRRQueue[0].front().remainCpuBurst > 0 ) {
    mRRQueue[0].push(  mRRQueue[0].front() ) ;
  } // if
  mRRQueue[0].pop() ;

} // PPRR :: ReLineUp()

void PPRR :: DoRRQueueFront( int timeSlice ) {
  Process nowProcess = mRRQueue[0].front() ;

  if ( nowProcess.remainCpuBurst <= timeSlice ) { // 這次timeSlice可以做完
    DoneThisProcess( mRRQueue[0].front() ) ;
    //mRRQueue[0].pop() ;
  } // if
  else { // 這次time slice做不完
    grantChart.insert( grantChart.end(), timeSlice, nowProcess.chPid ) ;
    mCurrentTime += timeSlice;
    mRRQueue[0].front().remainCpuBurst -= timeSlice ;
    //mRRQueue[0].pop() ;
    //mReadyQueue.push( nowProcess ) ; // 因為還沒做完，所以把他加到最後面
  } // else
} //  PPRR :: DoRoundRobin()


void PPRR :: DoneThisProcess( Process & process ) {

  int nowRemainCpuBurst = process.remainCpuBurst ;
  grantChart.insert( grantChart.end(), nowRemainCpuBurst, process.chPid ) ;
  mCurrentTime += nowRemainCpuBurst ;
  process.remainCpuBurst = 0 ;
  ProcessTime pt( mCurrentTime - process.arrivalTime - process.cpuBurst, mCurrentTime - process.arrivalTime ) ;
  mProcessTime.insert( pair<int, ProcessTime>( process.pid, pt ) );

} // PPRR :: DoneUnDoneFrontProcess()

void PPRR :: SubmitNewProcessToReadyQueue() {

  while ( ! mUnArrivalProcess.empty() && mCurrentTime >= mUnArrivalProcess.front().arrivalTime ) {
    InsertionSort( mUnArrivalProcess.front() ) ;
    mUnArrivalProcess.pop() ;
  } // while

} // PPRR :: SubmitProcessToReadyQueue()

void PPRR :: InsertionSort( Process newProcess ) {

  int newPriority = newProcess.priority ;
  int newArrival = newProcess.arrivalTime ;
  int newPid = newProcess.pid ;

  int i = 0 ;
  for ( i = 0 ; i < mUnDone.size() ; i++ ) {
    if( ( newPriority < mUnDone[i].priority ) ||
        ( newPriority == mUnDone[i].priority && newArrival < mUnDone[i].arrivalTime ) ||
        ( newPriority == mUnDone[i].priority && newArrival == mUnDone[i].arrivalTime && newPid < mUnDone[i].pid ) ) {
      mUnDone.insert( mUnDone.begin()+i, newProcess ) ;
      return ;
    } // if
  } // for

  mUnDone.insert( mUnDone.begin()+i, newProcess ) ;
} // PPRR :: InsertionSort()


void HRRN :: HighestResponseRatioNext() {

  mCurrentTime = 0 ; // 現在時間
  while ( ! mUnDone.empty() || ! mUnArrivalProcess.empty() ) {
    if ( mUnDone.empty() && mCurrentTime < mUnArrivalProcess.front().arrivalTime ) {
      int idleTime = mUnArrivalProcess.front().arrivalTime - mCurrentTime ;  // CPU需閒置的時間
      grantChart.insert( grantChart.end(), idleTime, '-' ) ;
      mCurrentTime += idleTime ;
    } // if
    else if ( mUnDone.empty() && mCurrentTime >= mUnArrivalProcess.front().arrivalTime ) {
      SubmitNewProcessToReadyQueue() ;
    } // else if
    else if ( ! mUnDone.empty() ) {
      CalResponseRatio() ;
      int maxIndex = 0 ;
      double maxResponseRatio = mUnDone[0].responseRatio ;
      int arrivalKey = mUnDone[0].arrivalTime ;
      int pidKey = mUnDone[0].pid ;
      for ( int i = 1 ; i < mUnDone.size() ; i++ ) {
        if( ( mUnDone[i].responseRatio > maxResponseRatio ) ||
            ( mUnDone[i].responseRatio == maxResponseRatio && mUnDone[i].arrivalTime < arrivalKey ) ||
            ( mUnDone[i].responseRatio == maxResponseRatio && mUnDone[i].arrivalTime == arrivalKey && mUnDone[i].pid < pidKey ) ) {
            maxResponseRatio = mUnDone[i].responseRatio ;
            arrivalKey = mUnDone[i].arrivalTime ;
            pidKey = mUnDone[i].pid ;
            maxIndex = i ;
        } // if
      } // for

      int nowCpuBurst = mUnDone[maxIndex].remainCpuBurst ;
      grantChart.insert( grantChart.end(), nowCpuBurst, mUnDone[maxIndex].chPid ) ;
      mCurrentTime += nowCpuBurst ;
      mUnDone[maxIndex].remainCpuBurst = 0 ;
      ProcessTime pt( mCurrentTime - arrivalKey - nowCpuBurst, mCurrentTime - arrivalKey ) ;
      mProcessTime.insert( pair<int, ProcessTime>( pidKey, pt ) );
      mUnDone.erase( mUnDone.begin() + maxIndex ) ;
      SubmitNewProcessToReadyQueue() ;

    } // else if

  } // while

} // HRRN :: HighestResponseRatioNext()

void HRRN :: CalResponseRatio() {

  for( int i = 0 ; i < mUnDone.size() ; i++ ) {
    int waitTime = mCurrentTime-mUnDone[i].arrivalTime ;
    int cpuBurst = mUnDone[i].cpuBurst ;
    mUnDone[i].responseRatio = (double) ( waitTime + cpuBurst ) / (double)cpuBurst ;
  } // for

} // HRRN :: ResponseRatio()


void HRRN :: SubmitNewProcessToReadyQueue() {

  while ( ! mUnArrivalProcess.empty() && mCurrentTime >= mUnArrivalProcess.front().arrivalTime ) {
    mUnDone.push_back( mUnArrivalProcess.front() ) ;
    mUnArrivalProcess.pop() ;
  } // while

} // RR :: SubmitProcessToReadyQueue()

void WriteSortedData( vector<int> sortedNum, string fileId, int command, double sortedCPUTime ){

  fstream outputFile ;
  string outputFileName = fileId + "_output" + to_string( command ) + ".txt" ;
  outputFile.open( outputFileName.c_str(), ios::out ) ;
  outputFile <<  "Sort : " << endl ;

  clock_t start,terminate ;  // 計算時間用
  start = clock();
  for( int i = 0 ; i < sortedNum.size() ; i++ ) {
    outputFile << sortedNum[i] << endl ;
  } // for

  outputFile << "CPU Time : " << sortedCPUTime / CLOCKS_PER_SEC << " s" << endl ;

  char tt[100];
  time_t now = time(nullptr);
  auto tm_info = localtime(&now);

  strftime(tt, 100, "%Y-%m-%d %H:%M:%S", tm_info);
  outputFile << "Output Time : " << tt << endl ;
  outputFile.close() ;
  terminate = clock();
  sortedCPUTime = terminate - start ;
  //cout << "Finished Writing Into File! " << endl << "Writing CPU Time : " << sortedCPUTime / CLOCKS_PER_SEC << " s" << endl ;

} // WriteSortedData

void CPU_Scheduler :: MergeSort( int first, int last ) { // 持續將資料分成一半，直到只剩下一個元素，再將被分成一半的資料，排序好並合併起來

  if ( first < last ) { // 若還有多於一個元素，就繼續分成一半
    int mid = ( first + last ) / 2 ;
     MergeSort( first, mid) ;  // 左半邊
     MergeSort( mid+1, last) ; // 右半邊
     MergeData( first, mid, last); // 將左半邊與右半邊合併並且排序好
  } // if

} // MergeSort

void CPU_Scheduler :: MergeData( int first, int mid, int last ) { // 合併左半部、右半部並排序好
  vector<Process> tempProcess( mProcessList.size() ) ;  // 先將排序好的資料暫存在tempSch裡
  int leftFirst = first, leftLast = mid ;  // 左半邊從位置first~mid
  int rightFirst = mid+1, rightLast = last ; // 右半邊從位置mid+1~last
  int index = first ; //要放進的位置
  for ( index = first ; ( leftFirst<= leftLast ) && ( rightFirst <= rightLast ) ; index++ ) { //比較左半邊與右半邊最前面的元素，若有一邊已經都已經比完了，就結束
    int leftArrival = mProcessList[leftFirst].arrivalTime, rightArrival = mProcessList[rightFirst].arrivalTime ;
    int leftPid = mProcessList[leftFirst].pid, rightPid = mProcessList[rightFirst].pid ;

    if ( leftArrival < rightArrival || ( leftArrival == rightArrival && leftPid < rightPid ) ) {  // 若左半邊大於等於右半邊
     tempProcess[index] = mProcessList[leftFirst] ; // 將左半邊最前面的元素放進tempSch裡
     leftFirst++ ;  // 將左半邊的index往後一格繼續與右半邊比較
    } // if
    else {  // 右半邊最前面的元素大於左半邊最前面的元素
      tempProcess[index] = mProcessList[rightFirst] ;  // 將右左半邊最前面的元素放進tempSch裡
      rightFirst++ ; // 將右半邊的index往後一格繼續與左半邊比較
    } // else
  } // for

  for( leftFirst ; leftFirst<= leftLast ; leftFirst++,index++)  // 將左半邊還沒放進tempSch裡的，依序放進去
    tempProcess[index] = mProcessList[leftFirst] ;

  for( rightFirst ; rightFirst<= rightLast ; rightFirst++,index++) // 將右半邊還沒放進tempSch裡的，依序放進去
    tempProcess[index] = mProcessList[rightFirst] ;

  for ( index = first ; index <= last ; index++ ) // 將剛剛排序好存進tempSch裡的資料依序放回mergeSchool中
    mProcessList[index] = tempProcess[index] ;
} // MergeData()


void PPRR :: MergeSort( int first, int last ) { // 持續將資料分成一半，直到只剩下一個元素，再將被分成一半的資料，排序好並合併起來

  if ( first < last ) { // 若還有多於一個元素，就繼續分成一半
    int mid = ( first + last ) / 2 ;
     MergeSort( first, mid) ;  // 左半邊
     MergeSort( mid+1, last) ; // 右半邊
     MergeData( first, mid, last); // 將左半邊與右半邊合併並且排序好
  } // if

} // MergeSort

void PPRR :: MergeData( int first, int mid, int last ) { // 合併左半部、右半部並排序好

  vector<Process> tempProcess( mProcessList.size() ) ;  // 先將排序好的資料暫存在tempSch裡
  int leftFirst = first, leftLast = mid ;  // 左半邊從位置first~mid
  int rightFirst = mid+1, rightLast = last ; // 右半邊從位置mid+1~last
  int index = first ; //要放進的位置
  for ( index = first ; ( leftFirst<= leftLast ) && ( rightFirst <= rightLast ) ; index++ ) { //比較左半邊與右半邊最前面的元素，若有一邊已經都已經比完了，就結束
    int leftArrival = mProcessList[leftFirst].arrivalTime, rightArrival = mProcessList[rightFirst].arrivalTime ;
    int leftPid = mProcessList[leftFirst].pid, rightPid = mProcessList[rightFirst].pid ;
    int leftPriority =  mProcessList[leftFirst].priority, rightPriority = mProcessList[rightFirst].priority ;

    if ( leftArrival < rightArrival || ( leftArrival == rightArrival && leftPriority < rightPriority ) ||
        ( leftArrival == rightArrival && leftPriority == rightPriority && leftPid < rightPid ) ) {  // 若左半邊大於等於右半邊
     tempProcess[index] = mProcessList[leftFirst] ; // 將左半邊最前面的元素放進tempSch裡
     leftFirst++ ;  // 將左半邊的index往後一格繼續與右半邊比較
    } // if
    else {  // 右半邊最前面的元素大於左半邊最前面的元素
      tempProcess[index] = mProcessList[rightFirst] ;  // 將右左半邊最前面的元素放進tempSch裡
      rightFirst++ ; // 將右半邊的index往後一格繼續與左半邊比較
    } // else
  } // for

  for( leftFirst ; leftFirst<= leftLast ; leftFirst++,index++)  // 將左半邊還沒放進tempSch裡的，依序放進去
    tempProcess[index] = mProcessList[leftFirst] ;

  for( rightFirst ; rightFirst<= rightLast ; rightFirst++,index++) // 將右半邊還沒放進tempSch裡的，依序放進去
    tempProcess[index] = mProcessList[rightFirst] ;

  for ( index = first ; index <= last ; index++ ) // 將剛剛排序好存進tempSch裡的資料依序放回mergeSchool中
    mProcessList[index] = tempProcess[index] ;
} // MergeData()


void CPU_Scheduler :: WriteSortedData( string name ){

  fstream outFile ;
  string outName = name + "_SORTED_PPRR.txt" ;
  outFile.open( outName.c_str() , ios::out) ;
  for( int i = 0 ; i < mProcessList.size() ; i++ ) {
    outFile << mProcessList[i].pid << "[" << mProcessList[i].chPid << "]" << '\t' << mProcessList[i].cpuBurst << '\t'
               << mProcessList[i].arrivalTime << '\t' << mProcessList[i].priority << endl ;
  } // for
  outFile.close() ;
} // WriteSortedData

char ConvertPid( int pid ) {

  if ( pid >= 0 && pid <= 9 ) {
    char charValue = pid + '0' ;
    return charValue ;
  } // e+if
  else {
    int num = pid + 55 ;
    char charValue = (char)num ;
    return charValue ;
  }

} // ConvertPid



void WriteResult(  string name, int command, CPU_Scheduler scheduler ) {

  fstream outputFile ;
  string fileName = "out_" + name + ".txt" ;
  outputFile.open( fileName.c_str() , ios::out) ;

  if ( command == 1 ) {
    outputFile << "FCFS" << endl ;
    outputFile << "==        FCFS==" << endl ;
  } // if
  else if ( command == 2 ) {
    outputFile << "RR" << endl ;
    outputFile << "==          RR==" << endl ;
  } // else if
  else if ( command == 3 ) {
    outputFile << "SRTF" << endl ;
    outputFile << "==        SRTF==" << endl ;
  } // else if
  else if ( command == 4 ) {
    outputFile << "Priority RR" << endl ;
    outputFile << "==        PPRR==" << endl ;
  } // else if
  else {
    outputFile << "HRRN" << endl ;
    outputFile << "==        HRRN==" << endl ;
  } // else if

  copy( scheduler.grantChart.begin(), scheduler.grantChart.end(), ostream_iterator<char>(outputFile, "") ) ;
  outputFile << endl ;
  outputFile << "===========================================================" << endl << endl ;


  outputFile << "Waiting Time" << endl ;

  if ( command == 1 ) {
    outputFile << "ID	FCFS" << endl ;
  } // if
  else if ( command == 2 ) {
    outputFile << "ID	RR" << endl ;
  } // else if
  else if ( command == 3 ) {
    outputFile << "ID	SRTF" << endl ;
  } // else if
  else if ( command == 4 ) {
    outputFile << "ID	PPRR" << endl ;
  } // else if
  else {
    outputFile << "ID	HRRN" << endl ;
  } // else

  outputFile << "===========================================================" << endl ;
  map<int, ProcessTime>::iterator iter = scheduler.mProcessTime.begin() ;


  for(  ; iter != scheduler.mProcessTime.end() ; iter++ ) {
    outputFile << iter->first << '\t' ;
    outputFile << iter->second.mWaitingTime << endl ;
  } // for

  outputFile << "===========================================================" << endl << endl ;


  outputFile << "Turnaround Time" << endl ;
  if ( command == 1 )
    outputFile << "ID	FCFS" << endl ;
  else if ( command == 2 )
    outputFile << "ID	RR" << endl ;
  else if ( command == 3 )
    outputFile << "ID	SRTF" << endl ;
  else if ( command == 4 )
    outputFile << "ID	PPRR" << endl ;
  else
    outputFile << "ID	HRRN" << endl ;

  outputFile << "===========================================================" << endl ;
  iter = scheduler.mProcessTime.begin() ;

  for(  ; iter != scheduler.mProcessTime.end() ; iter++ ) {
    outputFile << iter->first << '\t' ;
    outputFile << iter->second.mTurnaroundTime << endl ;
  } // for
  outputFile << "===========================================================" << endl << endl ;

  outputFile.close() ;
} // WriteResult()


void WriteAllResult( string name, FCFS fcfs, RR rr, SRTF srtf, PPRR pprr, HRRN hrrn ) {

  fstream allResultFile ;
  string fileName = "out_" + name + ".txt" ;
  allResultFile.open( fileName.c_str() , ios::out) ;

  allResultFile << "All" << endl ;
  allResultFile << "==        FCFS==" << endl ;
  copy( fcfs.grantChart.begin(), fcfs.grantChart.end(), ostream_iterator<char>(allResultFile, "") ) ;
  allResultFile << endl ;
  allResultFile << "==          RR==" << endl ;
  copy( rr.grantChart.begin(), rr.grantChart.end(), ostream_iterator<char>(allResultFile, "") );
  allResultFile << endl ;
  allResultFile << "==        SRTF==" << endl ;
  copy( srtf.grantChart.begin(), srtf.grantChart.end(), ostream_iterator<char>(allResultFile, "") ) ;
  allResultFile << endl ;
  allResultFile << "==        PPRR==" << endl ;
  copy( pprr.grantChart.begin(), pprr.grantChart.end(), ostream_iterator<char>(allResultFile, "") ) ;
  allResultFile << endl ;
  allResultFile << "==        HRRN==" << endl ;
  copy( hrrn.grantChart.begin(), hrrn.grantChart.end(), ostream_iterator<char>(allResultFile, "") ) ;
  allResultFile << endl ;
  allResultFile << "===========================================================" << endl << endl ;

  allResultFile << "Waiting Time" << endl ;
  allResultFile << "ID	FCFS	RR	SRTF	PPRR	HRRN" << endl ;
  allResultFile << "===========================================================" << endl ;
  map<int, ProcessTime>::iterator fcfsIter = fcfs.mProcessTime.begin(), rrIter = rr.mProcessTime.begin() ;
  map<int, ProcessTime>::iterator srtfIter = srtf.mProcessTime.begin(), pprrIter = pprr.mProcessTime.begin() ;
  map<int, ProcessTime>::iterator hrrnIter = hrrn.mProcessTime.begin() ;

  for(  ; fcfsIter != fcfs.mProcessTime.end() ; fcfsIter++, rrIter++, srtfIter++, pprrIter++, hrrnIter++ ) {
    allResultFile << fcfsIter->first << '\t' ;
    allResultFile << fcfsIter->second.mWaitingTime << '\t' << rrIter->second.mWaitingTime << '\t' << srtfIter->second.mWaitingTime << '\t'
         << pprrIter->second.mWaitingTime << '\t' << hrrnIter->second.mWaitingTime ;
    allResultFile << endl ;
  } // for
  allResultFile << "===========================================================" << endl << endl ;


  allResultFile << "Turnaround Time" << endl ;
  allResultFile << "ID	FCFS	RR	SRTF	PPRR	HRRN" << endl ;
  allResultFile << "===========================================================" << endl ;
  fcfsIter = fcfs.mProcessTime.begin(), rrIter = rr.mProcessTime.begin(), srtfIter = srtf.mProcessTime.begin() ;
  pprrIter = pprr.mProcessTime.begin(), hrrnIter = hrrn.mProcessTime.begin() ;

  for(  ; fcfsIter != fcfs.mProcessTime.end() ; fcfsIter++, rrIter++, srtfIter++, pprrIter++, hrrnIter++  ) {
    allResultFile << fcfsIter->first << '\t' ;
    allResultFile << fcfsIter->second.mTurnaroundTime << '\t' << rrIter->second.mTurnaroundTime << '\t' << srtfIter->second.mTurnaroundTime << '\t'
         << pprrIter->second.mTurnaroundTime << '\t' << hrrnIter->second.mTurnaroundTime;
    allResultFile << endl ;
  } // for
  allResultFile << "===========================================================" << endl << endl ;

  allResultFile.close() ;
} // WriteAllResult()

void AverageWaitingAndTurnaroundTime(  CPU_Scheduler scheduler, float & averageWaiting, float & averageTurnaround  ) {

  map<int, ProcessTime>::iterator iter = scheduler.mProcessTime.begin() ;

  averageWaiting = 0 ;
  averageTurnaround = 0 ;
  for(  ; iter != scheduler.mProcessTime.end() ; iter++ ) {
    averageWaiting += iter->second.mWaitingTime ;
    averageTurnaround += iter->second.mTurnaroundTime ;
  } // for

  averageWaiting = float(averageWaiting) / float(scheduler.mProcessTime.size() ) ;
  averageTurnaround = float(averageTurnaround) / float(scheduler.mProcessTime.size() ) ;
  //cout << "Average Waiting Time: " << averageWaiting << endl ;

} // AverageWaitingAndTurnaroundTime()

void AllAverageWaitingAndTurnaroundTime( FCFS fcfs, RR rr, SRTF srtf, PPRR pprr, HRRN hrrn ) {

  float fcfsAverageWaiting, fcfsAverageTurnaround ; AverageWaitingAndTurnaroundTime( fcfs, fcfsAverageWaiting, fcfsAverageTurnaround ) ;
  float rrAverageWaiting, rrAverageTurnaround     ; AverageWaitingAndTurnaroundTime( rr, rrAverageWaiting, rrAverageTurnaround ) ;
  float srtfAverageWaiting, srtfAverageTurnaround ; AverageWaitingAndTurnaroundTime( srtf, srtfAverageWaiting, srtfAverageTurnaround ) ;
  float pprrAverageWaiting, pprrAverageTurnaround ; AverageWaitingAndTurnaroundTime( pprr, pprrAverageWaiting, pprrAverageTurnaround ) ;
  float hrrnAverageWaiting, hrrnAverageTurnaround ; AverageWaitingAndTurnaroundTime( hrrn, hrrnAverageWaiting, hrrnAverageTurnaround ) ;

  cout << "Average Waiting Time: " << endl ;
  cout << "FCFS	RR	SRTF	PPRR	HRRN" << endl ;
  cout << fcfsAverageWaiting << '\t' << rrAverageWaiting << '\t' << srtfAverageWaiting << '\t' << pprrAverageWaiting << '\t' << hrrnAverageWaiting << endl << endl ;

  cout << "Average Turnaround Time: " << endl ;
  cout << "FCFS	RR	SRTF	PPRR	HRRN" << endl ;
  cout << fcfsAverageTurnaround << '\t' << rrAverageTurnaround << '\t' << srtfAverageTurnaround << '\t' << pprrAverageTurnaround << '\t' << hrrnAverageTurnaround << endl << endl ;
} // AverageWaitingTime()
