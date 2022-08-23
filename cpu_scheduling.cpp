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
  char chPid ; // 10�H�W�|�ܦr��
  int cpuBurst ;
  int remainCpuBurst ; // �Ѿl�ݭn��CPU�ɶ�
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
  queue<Process> mUnArrivalProcess ; // �O���b�{�b���ɶ��٥���F��process
  queue<Process> mReadyQueue ; // �b����CPU��process
  int mTimeSlice ;
  int mCurrentTime ;

  CPU_Scheduler( vector<Process> inputProcess, int inputTimeSlice ) {
    mProcessList = inputProcess ;
    mTimeSlice = inputTimeSlice ;
    mCurrentTime = 0 ;
    MergeSort( 0, mProcessList.size()-1 ) ;
    for( int i = 0 ; i < mProcessList.size() ; i++ ) // ��쥻�x�s�bvector����iqueue��
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
    for( int i = 0 ; i < mProcessList.size() ; i++ ) // ��쥻�x�s�bvector����iqueue��
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
void ReadInputFileName( string & fileId,string & inputFullName ) ; // Ūinput�ɽs��
bool OpenInputFile( string & fileId,string & inputFullName,fstream & inputFile ) ;  // �}��input��
void ReadInputData( fstream & file, vector<int> & unsortedNum ) ;//�Ninput�ɸ̪����Ū�ivector<School> sch��
string ReadString(fstream & file); // �qfile��Ū�@��string(�H'\t'������)
void Swap( vector<int> & num, int a, int b ) ; // �Nvector�̲�a�Ӥ����P��b�Ӥ������洫
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
  ReadInputFileName( fileId,inputFullName ) ;   // Ū�ɮ׽s��
  if ( ! OpenInputFile( fileId, inputFullName, inputFile ) )  // �}���ɮסA�Y�ɮ׽s���O0�h�����������sŪcommand
    isQuit = true ;
  else {
    // Ū�Ĥ@�C����ӼƦr( command, timeslice )

    inputFile >> command >> timeSlice ;

    // cout << "command : " << command << "  timeSlice : " << timeSlice << endl ;


    ReadUntilEnter( inputFile ) ;
    string line ;
    getline( inputFile, line ) ; // Ū�����D


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

void Enter() {        // Ū��'\n'����
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
  cout << "�п�J�ɮצW��(eg.input1, input2,......)[0]Quit : " ;
  cin >> fileId ;
  char ch ;
  cin.get(ch);
  if ( ch != '\n')
    Enter() ;
  inputFullName = fileId + ".txt" ;
} // ReadInputFileName

bool OpenInputFile( string & fileId, string & inputFullName, fstream & inputFile ){

  if ( fileId.compare("0") == 0 )  // �p�G�s���O0�A���������A���sŪcommand
   return false ;
  else {
    inputFile.open( inputFullName.c_str(), ios::in ) ;
    while ( ! inputFile ) {    // �����ɮצ��\�}��
      cout << "### " << inputFullName << " does not exist ### " << endl ;
      fileId = "" ;
      ReadInputFileName( fileId,inputFullName ) ;
      if ( fileId.compare("0") == 0 )   // �p�G�s���O0�A���������A���sŪcommand
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
  mCurrentTime = 0 ; // �{�b�ɶ�

  while( ! mReadyQueue.empty() ||  ! mUnArrivalProcess.empty() ) { // readyQueue�٦�process�b���ݩάO�٦�process����F

    // readyQueue�O�Ū��A�٦�prcoess����F�A�{�b�ɶ��p��U�@��process��F���ɶ�
    if ( mReadyQueue.empty() && ! mUnArrivalProcess.empty() && mCurrentTime < mUnArrivalProcess.front().arrivalTime ) {
      int idleTime = mUnArrivalProcess.front().arrivalTime - mCurrentTime ;  // CPU�ݶ��m���ɶ�
      grantChart.insert( grantChart.end(), idleTime, '-' ) ;
      mCurrentTime += idleTime ;
    } // if
    else if ( mReadyQueue.empty() && ! mUnArrivalProcess.empty() && mCurrentTime >= mUnArrivalProcess.front().arrivalTime ) {
      SubmitNewProcessToReadyQueue() ;
    } // else if
    else if ( ! mReadyQueue.empty() ) { // readyQueue�٦�process�b���ݰ���

      Process nowProcess = mReadyQueue.front() ;

      if ( nowProcess.remainCpuBurst <= mTimeSlice ) { // �o��process�ҳѾl���ɶ��p�󵥩�@��time slice�A�o�������N�����F

        grantChart.insert( grantChart.end(), nowProcess.remainCpuBurst, nowProcess.chPid ) ;
        mCurrentTime += nowProcess.remainCpuBurst;
        nowProcess.remainCpuBurst = 0 ;
        ProcessTime pt( mCurrentTime - nowProcess.arrivalTime - nowProcess.cpuBurst, mCurrentTime - nowProcess.arrivalTime ) ;
        mProcessTime.insert(pair<int, ProcessTime>( nowProcess.pid, pt ) );
        mReadyQueue.pop() ;
        SubmitNewProcessToReadyQueue() ;
      } // if
      else { // �o��time slice������
        grantChart.insert( grantChart.end(), mTimeSlice, nowProcess.chPid ) ;
        mCurrentTime += mTimeSlice;
        nowProcess.remainCpuBurst -= mTimeSlice ;
        SubmitNewProcessToReadyQueue() ;
        mReadyQueue.pop() ;
        mReadyQueue.push( nowProcess ) ; // �]���٨S�����A�ҥH��L�[��̫᭱
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
  mCurrentTime = 0 ; // �{�b�ɶ�

  while( ! mUnDone.empty() || ! mUnArrivalProcess.empty() ) {

    if ( mUnDone.empty() && mCurrentTime < mUnArrivalProcess.front().arrivalTime ) {
      int idleTime = mUnArrivalProcess.front().arrivalTime - mCurrentTime ;  // CPU�ݶ��m���ɶ�
      grantChart.insert( grantChart.end(), idleTime, '-' ) ;
      mCurrentTime += idleTime ;
    } // if
    else if ( mUnDone.empty() && mCurrentTime >= mUnArrivalProcess.front().arrivalTime ) {
      SubmitNewProcessToReadyQueue() ;
    } // else if
    else if ( ! mUnDone.empty() ) {

      // �� remain CPU Burst �̤֪���
      if ( ! mUnArrivalProcess.empty() ) {
        int nextArrival = mUnArrivalProcess.front().arrivalTime ;
        int nowRemainCpuBurst = mUnDone.front().remainCpuBurst ;
        int timeSlice = nextArrival - mCurrentTime ;

        if ( nowRemainCpuBurst <= timeSlice ) { // �i�H��������
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
      else { // ������process���w�g��F�A�ҩ�JunDo��

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


  mCurrentTime = 0 ; // �{�b�ɶ�

  while( ! mUnDone.empty() || ! mUnArrivalProcess.empty() || mRRQueue.size() != 0  ) {

    // �{�b�S��process�n����A���ݤU�@��process��F
    if ( mUnDone.empty() && mRRQueue.size() == 0 && mCurrentTime < mUnArrivalProcess.front().arrivalTime ) {
      int idleTime = mUnArrivalProcess.front().arrivalTime - mCurrentTime ;  // CPU�ݶ��m���ɶ�
      grantChart.insert( grantChart.end(), idleTime, '-' ) ;
      mCurrentTime += idleTime ;
    } // if
    else if ( mUnDone.empty() && mRRQueue.size() == 0 && mCurrentTime >= mUnArrivalProcess.front().arrivalTime ) {
      SubmitNewProcessToReadyQueue() ;
    } // else if
    else if ( ! mUnDone.empty() && ! mRRQueue.empty() ) { // rrQueue���H�b�ƶ��AmUnDone�]���H�b�ƶ��A�ݭ����u���v����
      int mUnDonePriority = mUnDone.front().priority ;

      if ( mUnDonePriority < mRRQueue[0].front().priority ) { // mUnDone�u���v����

        queue<Process> newRRQueue ;

        while ( ! mUnDone.empty() && mUnDone.front().priority == mUnDonePriority ) { // ��mUnDone���u���v�@�˪���iqueue��
          newRRQueue.push( mUnDone.front() ) ;
          mUnDone.erase( mUnDone.begin() ) ;
        } // while
        mRRQueue.insert( mRRQueue.begin(), newRRQueue ) ; // �s��queue�u���v����

      } // if
      else if ( mUnDonePriority == mRRQueue[0].front().priority ) {
        while ( ! mUnDone.empty() && mUnDone.front().priority == mUnDonePriority ) { // ��mUnDone���u���v�@�˪���iqueue��
          mRRQueue[0].push( mUnDone[0] ) ;
          mUnDone.erase( mUnDone.begin() ) ; // ����iqueue���N�R���F
        } // while

      } // else if
      else { // rrQueue����process�u���v�����ArrQueue����process����
        RoundRobin() ;
      } // else
    } // else if
    else if ( ! mUnDone.empty() && mRRQueue.empty() ) { // rrQueue�S�H�b�ƶ��A

      int minPriority = mUnDone.front().priority ;
      queue<Process> newRRQueue ;
      while ( ! mUnDone.empty() && mUnDone.front().priority == minPriority ) { // ��mUnDone���u���v�@�˪���iqueue��
        newRRQueue.push( mUnDone.front() ) ;
        mUnDone.erase( mUnDone.begin() ) ;
      } // while

      mRRQueue.push_back( newRRQueue ) ; // �s��queue�u���v����

    } // else if
    else if ( mUnDone.empty() && ! mRRQueue.empty() ) {
      RoundRobin() ;
    } // else if

  } // while


} // PPRR :: PreemptivePriortyRoundRobin()

void PPRR :: RoundRobin() {

  int nextArrival = mUnArrivalProcess.front().arrivalTime ; // �U�@��process��F���ɶ�
  int interval = nextArrival - mCurrentTime ; // �U�@��process�줧�e���h�֮ɶ�
  int currentPriority = mRRQueue[0].front().priority ;
  int nextPriorty = mUnArrivalProcess.front().priority ;

  // queue����process�������F�A�άO���s��process��F  && ( mCurrentTime < nextArrival && nextPriorty <= currentPriority )
  while ( mRRQueue[0].size() != 0 ) {

    if ( ! mUnArrivalProcess.empty() ) {

      nextArrival = mUnArrivalProcess.front().arrivalTime ; // �U�@��process��F���ɶ�
      nextPriorty = mUnArrivalProcess.front().priority ;
      interval = nextArrival - mCurrentTime ;

      int thisTimeSlice ;

      if ( mTimeSlice <= interval || ( interval <  mTimeSlice && nextPriorty >= currentPriority ) ) // �@����mTimeSlice
        thisTimeSlice = mTimeSlice ;
      else // �@����timeslice
        thisTimeSlice = interval ;

      DoRRQueueFront( thisTimeSlice ) ;

      if ( mCurrentTime >= nextArrival ) { // ��process��F
        if ( nextPriorty < currentPriority ) { // �s��process�u���v����

          SubmitNewProcessToReadyQueue() ;
          ReLineUp() ; // ��谵��process�p�G�٨S�����N�h���s�ƶ��A�p�G�����F�N����
          if ( mRRQueue[0].size() == 0 )
            mRRQueue.erase( mRRQueue.begin() ) ;
          return ;
        } // if
        else if ( nextPriorty == currentPriority ) { // �u���v�P�{�b�b�����ۦP�A��L�[��queue�᭱

          SubmitNewProcessToReadyQueue() ;
          while ( ! mUnDone.empty() && mUnDone.front().priority == currentPriority ) { // ��mUnDone���u���v�@�˪���iqueue�̡A�٨S���L�o����
            mRRQueue[0].push( mUnDone[0] ) ;
            mUnDone.erase( mUnDone.begin() ) ; // ����iqueue���N�R���F
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
    else { // �Ҧ���process���w�g��F�A�ҩ�JmUndone��
      while ( mRRQueue[0].size() != 0  ) {
        DoRRQueueFront( mTimeSlice ) ;
        ReLineUp() ;
      } // while
    } // else
  } // while

  if ( mRRQueue[0].size() == 0 )
    mRRQueue.erase( mRRQueue.begin() ) ;

} // PPRR :: RoundRobin()

void PPRR :: ReLineUp() { // �쥻�Ʀbqueue�̫e����process���w�g�����@��time slice�F�A�n�h���s�ƶ�

  if ( mRRQueue[0].front().remainCpuBurst > 0 ) {
    mRRQueue[0].push(  mRRQueue[0].front() ) ;
  } // if
  mRRQueue[0].pop() ;

} // PPRR :: ReLineUp()

void PPRR :: DoRRQueueFront( int timeSlice ) {
  Process nowProcess = mRRQueue[0].front() ;

  if ( nowProcess.remainCpuBurst <= timeSlice ) { // �o��timeSlice�i�H����
    DoneThisProcess( mRRQueue[0].front() ) ;
    //mRRQueue[0].pop() ;
  } // if
  else { // �o��time slice������
    grantChart.insert( grantChart.end(), timeSlice, nowProcess.chPid ) ;
    mCurrentTime += timeSlice;
    mRRQueue[0].front().remainCpuBurst -= timeSlice ;
    //mRRQueue[0].pop() ;
    //mReadyQueue.push( nowProcess ) ; // �]���٨S�����A�ҥH��L�[��̫᭱
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

  mCurrentTime = 0 ; // �{�b�ɶ�
  while ( ! mUnDone.empty() || ! mUnArrivalProcess.empty() ) {
    if ( mUnDone.empty() && mCurrentTime < mUnArrivalProcess.front().arrivalTime ) {
      int idleTime = mUnArrivalProcess.front().arrivalTime - mCurrentTime ;  // CPU�ݶ��m���ɶ�
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

  clock_t start,terminate ;  // �p��ɶ���
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

void CPU_Scheduler :: MergeSort( int first, int last ) { // ����N��Ƥ����@�b�A����u�ѤU�@�Ӥ����A�A�N�Q�����@�b����ơA�ƧǦn�æX�ְ_��

  if ( first < last ) { // �Y�٦��h��@�Ӥ����A�N�~������@�b
    int mid = ( first + last ) / 2 ;
     MergeSort( first, mid) ;  // ���b��
     MergeSort( mid+1, last) ; // �k�b��
     MergeData( first, mid, last); // �N���b��P�k�b��X�֨åB�ƧǦn
  } // if

} // MergeSort

void CPU_Scheduler :: MergeData( int first, int mid, int last ) { // �X�֥��b���B�k�b���ñƧǦn
  vector<Process> tempProcess( mProcessList.size() ) ;  // ���N�ƧǦn����ƼȦs�btempSch��
  int leftFirst = first, leftLast = mid ;  // ���b��q��mfirst~mid
  int rightFirst = mid+1, rightLast = last ; // �k�b��q��mmid+1~last
  int index = first ; //�n��i����m
  for ( index = first ; ( leftFirst<= leftLast ) && ( rightFirst <= rightLast ) ; index++ ) { //������b��P�k�b��̫e���������A�Y���@��w�g���w�g�񧹤F�A�N����
    int leftArrival = mProcessList[leftFirst].arrivalTime, rightArrival = mProcessList[rightFirst].arrivalTime ;
    int leftPid = mProcessList[leftFirst].pid, rightPid = mProcessList[rightFirst].pid ;

    if ( leftArrival < rightArrival || ( leftArrival == rightArrival && leftPid < rightPid ) ) {  // �Y���b��j�󵥩�k�b��
     tempProcess[index] = mProcessList[leftFirst] ; // �N���b��̫e����������itempSch��
     leftFirst++ ;  // �N���b�䪺index����@���~��P�k�b����
    } // if
    else {  // �k�b��̫e���������j�󥪥b��̫e��������
      tempProcess[index] = mProcessList[rightFirst] ;  // �N�k���b��̫e����������itempSch��
      rightFirst++ ; // �N�k�b�䪺index����@���~��P���b����
    } // else
  } // for

  for( leftFirst ; leftFirst<= leftLast ; leftFirst++,index++)  // �N���b���٨S��itempSch�̪��A�̧ǩ�i�h
    tempProcess[index] = mProcessList[leftFirst] ;

  for( rightFirst ; rightFirst<= rightLast ; rightFirst++,index++) // �N�k�b���٨S��itempSch�̪��A�̧ǩ�i�h
    tempProcess[index] = mProcessList[rightFirst] ;

  for ( index = first ; index <= last ; index++ ) // �N���ƧǦn�s�itempSch�̪���ƨ̧ǩ�^mergeSchool��
    mProcessList[index] = tempProcess[index] ;
} // MergeData()


void PPRR :: MergeSort( int first, int last ) { // ����N��Ƥ����@�b�A����u�ѤU�@�Ӥ����A�A�N�Q�����@�b����ơA�ƧǦn�æX�ְ_��

  if ( first < last ) { // �Y�٦��h��@�Ӥ����A�N�~������@�b
    int mid = ( first + last ) / 2 ;
     MergeSort( first, mid) ;  // ���b��
     MergeSort( mid+1, last) ; // �k�b��
     MergeData( first, mid, last); // �N���b��P�k�b��X�֨åB�ƧǦn
  } // if

} // MergeSort

void PPRR :: MergeData( int first, int mid, int last ) { // �X�֥��b���B�k�b���ñƧǦn

  vector<Process> tempProcess( mProcessList.size() ) ;  // ���N�ƧǦn����ƼȦs�btempSch��
  int leftFirst = first, leftLast = mid ;  // ���b��q��mfirst~mid
  int rightFirst = mid+1, rightLast = last ; // �k�b��q��mmid+1~last
  int index = first ; //�n��i����m
  for ( index = first ; ( leftFirst<= leftLast ) && ( rightFirst <= rightLast ) ; index++ ) { //������b��P�k�b��̫e���������A�Y���@��w�g���w�g�񧹤F�A�N����
    int leftArrival = mProcessList[leftFirst].arrivalTime, rightArrival = mProcessList[rightFirst].arrivalTime ;
    int leftPid = mProcessList[leftFirst].pid, rightPid = mProcessList[rightFirst].pid ;
    int leftPriority =  mProcessList[leftFirst].priority, rightPriority = mProcessList[rightFirst].priority ;

    if ( leftArrival < rightArrival || ( leftArrival == rightArrival && leftPriority < rightPriority ) ||
        ( leftArrival == rightArrival && leftPriority == rightPriority && leftPid < rightPid ) ) {  // �Y���b��j�󵥩�k�b��
     tempProcess[index] = mProcessList[leftFirst] ; // �N���b��̫e����������itempSch��
     leftFirst++ ;  // �N���b�䪺index����@���~��P�k�b����
    } // if
    else {  // �k�b��̫e���������j�󥪥b��̫e��������
      tempProcess[index] = mProcessList[rightFirst] ;  // �N�k���b��̫e����������itempSch��
      rightFirst++ ; // �N�k�b�䪺index����@���~��P���b����
    } // else
  } // for

  for( leftFirst ; leftFirst<= leftLast ; leftFirst++,index++)  // �N���b���٨S��itempSch�̪��A�̧ǩ�i�h
    tempProcess[index] = mProcessList[leftFirst] ;

  for( rightFirst ; rightFirst<= rightLast ; rightFirst++,index++) // �N�k�b���٨S��itempSch�̪��A�̧ǩ�i�h
    tempProcess[index] = mProcessList[rightFirst] ;

  for ( index = first ; index <= last ; index++ ) // �N���ƧǦn�s�itempSch�̪���ƨ̧ǩ�^mergeSchool��
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
