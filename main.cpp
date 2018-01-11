#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <onion-i2c.h>
#include <oled-exp.h>
#include <iostream>
#include <fstream>
using namespace std;

struct logData{
    char* logType;
    char* event;
    int eventHour;
    logData* next;
};

void timer();
void sleep(int milliseconds);
void logUnlock(int data[7][24]);
int getDayFuture();
int getHourFuture();
int getHour();
int getDay();
float dayMax(const int data[7][24]);
float hourMax(const int data[7][24]);
float avgHourMax(const int data[7][24]);
float wordsPerHour(const int data[7][24]);
float stdvWordsPerHour (const int data[7][24]);
void wait();
void writeArray(char output);
void addToArray(char array[], int input, bool writeNUM, bool writeCAP);
void writeFile(int logInformation[7][24]);
void addList(logData* node);
void writeLog();


logData* head=NULL;
void writeFile(int logInformation[7][24]){

//    float dayMax(const int data[7][24]);
//    float hourMax(const int data[7][24]);
//    float avgHourMax(const int data[7][24]);
//    float wordsPerHour(const int data[7][24]);
//    float stdvWordsPerHour (const int data[7][24]);
    if (getDayFuture()!=getDay()){
        ofstream outfile;
        outfile.open("KeyboardStats.txt");
        outfile << "Week before 2017-11-29"<<"\n";
        outfile << "Maximum Day: "<<  dayMax(logInformation) <<"\n";
        outfile << "Maximum Hour: "<< wordsPerHour(logInformation)<<"\n";
        outfile << "Maximum Hour AVG: " << hourMax(logInformation)<<"\n";
        outfile << "Words/Hour:" << wordsPerHour(logInformation)<< "\n";
        outfile << "STD Dev: "<< stdvWordsPerHour(logInformation)<< "\n";
        outfile.close();
    }
}

void writeLog(){
    ofstream outfile;
    outfile.open("LogFile.txt");
    if (head==NULL){
        outfile << "NO INPUT DATA TO LOG";
    }
    else{
        outfile<<"LOG TYPE \t EVENT TYPE \t EVENT HOUR\n";
        logData* tracer = head;
        while (tracer->next!=NULL){
            outfile << tracer->logType <<"\t \t  "<< tracer->event<<"\t"<<"2017-12-01-5:00\n";
            tracer=tracer->next;
        }
        outfile << tracer->logType <<"\t  \t  "<< tracer->event<<"\t"<<tracer->eventHour<<"2017-12-01-5:00\n";
    }
    outfile.close();
}


void writeArray(char output[]){
    int status;
    status=oledWrite(output);
    printf("character: %s",output);
}

void addToArray(char array[], int input, bool writeNUM, bool writeCAP){
    int lineSize=20;
    int index=0;
    while (array[index]!=0)
        index++;

    if(index>=21){
        int i=0;
        while(i<168){
            array[i]=0;
            i++;
        }
        index=0;
    }

    if (input==14){
        int status=oledClear();
        array[index-1]=0;	//////backspace
    }
    if (input==15){
        array[index]=' ';
        if ((index+1)%lineSize!=0){
            array[index+1]=' ';
            if((index+2)%lineSize!=0){
                array[index+2]=' ';                   //////////tab
                if((index+3)%lineSize!=0){
                    array[index+3]=' ';
                }
            }
        }
    }
    if (input==28){
        int q=index;
        while (q%lineSize-1!=0){         ///////new line
            array[q]=' ';
            q++;
        }
    }

    if (input==57)
        array[index]=' ';
    if(input==2&&writeNUM)
        array[index]='1';
    if(input==3&&writeNUM)
        array[index]='2';
    if(input==4&&writeNUM)
        array[index]='3';
    if(input==5&&writeNUM)
        array[index]='4';
    if(input==6&&writeNUM)
        array[index]='5';
    if(input==7&&writeNUM)
        array[index]='6';
    if(input==8&&writeNUM)
        array[index]='7';
    if(input==9&&writeNUM)
        array[index]='8';
    if(input==10&&writeNUM)
        array[index]='9';
    if(input==11&&writeNUM)
        array[index]='0';

    if(input==16&&writeCAP)
        array[index]='Q';
    if(input==17&&writeCAP)
        array[index]='W';
    if(input==18&&writeCAP)
        array[index]='E';
    if(input==19&&writeCAP)
        array[index]='R';
    if(input==20&&writeCAP)
        array[index]='T';
    if(input==21&&writeCAP)
        array[index]='Y';
    if(input==22&&writeCAP)
        array[index]='U';
    if(input==23&&writeCAP)
        array[index]='I';
    if(input==24&&writeCAP)
        array[index]='O';
    if(input==25&&writeCAP)
        array[index]='P';
    if(input==30&&writeCAP)
        array[index]='A';
    if(input==31&&writeCAP)
        array[index]='S';
    if(input==32&&writeCAP)
        array[index]='D';
    if(input==33&&writeCAP)
        array[index]='F';
    if(input==34&&writeCAP)
        array[index]='G';
    if(input==35&&writeCAP)
        array[index]='H';
    if(input==36&&writeCAP)
        array[index]='J';
    if(input==37&&writeCAP)
        array[index]='K';
    if(input==38&&writeCAP)
        array[index]='L';
    if(input==44&&writeCAP)
        array[index]='Z';
    if(input==45&&writeCAP)
        array[index]='X';
    if(input==46&&writeCAP)
        array[index]='C';
    if(input==47&&writeCAP)
        array[index]='V';
    if(input==48&&writeCAP)
        array[index]='B';
    if(input==49&&writeCAP)
        array[index]='N';
    if(input==50&&writeCAP)
        array[index]='M';

    if(input==16&&!writeCAP)
        array[index]='q';
    if(input==17&&!writeCAP)
        array[index]='w';
    if(input==18&&!writeCAP)
        array[index]='e';
    if(input==19&&!writeCAP)
        array[index]='r';
    if(input==20&&!writeCAP)
        array[index]='t';
    if(input==21&&!writeCAP)
        array[index]='y';
    if(input==22&&!writeCAP)
        array[index]='u';
    if(input==23&&!writeCAP)
        array[index]='i';
    if(input==24&&!writeCAP)
        array[index]='o';
    if(input==25&&!writeCAP)
        array[index]='p';
    if(input==30&&!writeCAP)
        array[index]='a';
    if(input==31&&!writeCAP)
        array[index]='s';
    if(input==32&&!writeCAP)
        array[index]='d';
    if(input==33&&!writeCAP)
        array[index]='f';
    if(input==34&&!writeCAP)
        array[index]='g';
    if(input==35&&!writeCAP)
        array[index]='h';
    if(input==36&&!writeCAP)
        array[index]='j';
    if(input==37&&!writeCAP)
        array[index]='k';
    if(input==38&&!writeCAP)
        array[index]='l';
    if(input==44&&!writeCAP)
        array[index]='z';
    if(input==45&&!writeCAP)
        array[index]='x';
    if(input==46&&!writeCAP)
        array[index]='c';
    if(input==47&&!writeCAP)
        array[index]='v';
    if(input==48&&!writeCAP)
        array[index]='b';
    if(input==49&&!writeCAP)
        array[index]='n';
    if(input==50&&!writeCAP)
        array[index]='m';

    if(input==12)
        array[index]='-';
    if(input==13)
        array[index]='=';
    if(input==26)
        array[index]='{';
    if(input==27)
        array[index]='}';
    if(input==39)
        array[index]=';';
    if(input==40)
        array[index]='\'';
    if(input==41)
        array[index]='`';
    if(input==43)
        array[index]='\\';
    if(input==51)
        array[index]=',';
    if(input==52)
        array[index]='.';
    if(input==53)
        array[index]='/';
    if(input==55)
        array[index]='*';
    //if(input==57)
    //array[index]='/';
    printf("character at i: %c",array[index]);
    printf("char array: %s",array);



    /*else{
    int i=0;
    while(i<168){
    array[i]=0;
    i++;
}
array[0]=
}*/
}




int getHourFuture(){
    time_t now;
    time_t tenMinutesAgo;
    struct tm *tenMinutesAgo_tm;
    char* c_time_string;
    now = time(NULL);
    tenMinutesAgo = now + 60;

    tenMinutesAgo_tm = localtime(&tenMinutesAgo);
    int hour;
    hour = tenMinutesAgo_tm->tm_hour;
    return hour;
}
int getDayFuture(){

    time_t now;
    time_t tenMinutesAgo;
    struct tm *tenMinutesAgo_tm;
    char* c_time_string;
    now = time(NULL);
    tenMinutesAgo = now + 60;

    tenMinutesAgo_tm = localtime(&tenMinutesAgo);
    int day;
    day = tenMinutesAgo_tm->tm_wday;
    return day;
}
int getHour(){
    time_t now;
    struct tm *now_tm;
    int hour;

    now = time(NULL);
    now_tm = localtime(&now);
    hour = now_tm->tm_hour;
    hour=hour+5;
    if (hour>=24){
        hour=hour-24;
    }
    //printf("hour: %x \n",hour);
    return hour;
}
int getDay(){
    time_t now;
    struct tm *now_tm;
    int day;

    now = time(NULL);
    now_tm = localtime(&now);
    day = now_tm->tm_wday;
    printf("day: %x \n", day);
    return day;
}
void logUnlock(int data[7][24]){
    data[getDay()][getHour()]++;
    if (getDay()!=getDayFuture())
        data[getDayFuture()][0]=0;
    else if (getHour()!=getHourFuture())
        data[getDay()][getHourFuture()]=0;
}
float dayMax(const int data[7][24]){
    int dayTotal[7];
    for (int i=0;i<7;i++){
        int sum=0;
        for (int k=0;k<24;k++){
            sum=sum+data[i][k];
        }
        dayTotal[i]=sum;
    }
    int max=0;
    for (int i=0;i<7;i++){
        if (dayTotal[i]>max)
            max=dayTotal[i];
    }
    int k=0;
    while(dayTotal[k]!=max)
        k++;
    return k;//k is the day at which the max occurs
}
float hourMax(const int data[7][24]){
    int max=0;
    int maxLoc=0;
    for(int i=0;i<7;i++){
        for (int k=0;k<24;k++){
            if (data[i][k]>max){
                max=data[i][k];
                maxLoc=i*7+k;
            }
        }
    }
    return maxLoc;
}

float avgHourMax(const int data[7][24]){
    int max=0;
    int hourTotal[24];
    for (int i=0;i<24;i++){
        int sum=0;
        for (int k=0;k<7;k++){
            sum=sum+data[k][i];
        }
        hourTotal[i]=sum;
    }
    for (int j=0;j<24;j++){
        if (hourTotal[j]>max)
            max=hourTotal[j];
    }
    int q=0;
    while(hourTotal[q]!=max)
        q++;
    return q;
}

float wordsPerHour(const int data[7][24]){
    int effectiveSize;
    for (int i=0;i<7;i++){
        for (int k=0;k<24;k++){
            if (data[i][k]!=0)
                effectiveSize++;
        }
    }
    float effectiveData[effectiveSize];
    int j=0;
    for (int i=0;i<7;i++){
        for (int k=0;k<24;k++){
            if (data[i][k]!=0){
                effectiveData[j]=data[i][k];
                j++;
            }
        }
    }
    int sum=0;
    for(int p=0;p<effectiveSize;p++)
        sum=sum+effectiveData[p];
    return sum/(effectiveSize*5);
}

float stdvWordsPerHour (const int data[7][24]){
    int effectiveSize;
    for (int i=0;i<7;i++){
        for (int k=0;k<24;k++){
            if (data[i][k]!=0)
                effectiveSize++;
        }
    }
    float effectiveData[effectiveSize];
    int j=0;
    for (int i=0;i<7;i++){
        for (int k=0;k<24;k++){
            if (data[i][k]!=0){
                effectiveData[j]=data[i][k];
                j++;
            }
        }
    }
    int sum=0;
    for(int p=0;p<effectiveSize;p++)
        sum=sum+effectiveData[p];
    float avg=sum/effectiveSize;
    float totalDev;
    for (int q=0;q<effectiveSize;q++)
        totalDev=totalDev+((avg-effectiveData[q])*(avg-effectiveData[q]));
    return float(pow((totalDev/(effectiveSize-1)),0.5));

}
void sleep(int milliseconds){
    clock_t time_end = time_end = clock() + milliseconds * CLOCKS_PER_SEC/1000;
    while (clock() < time_end) {}
}

void timer(){
    for (int sec=60;sec>=0;sec--){
        char s;
        char temp=25;
        if (sec<10){
            s=sec+'0';
        }
        else{
            temp=sec/10+'0';
            s=(sec%10)+'0';
        }
        int status;
        if (sec==60){
            status=oledWrite("Try again in 60 seconds");
        }
        else{
            printf("s = %c",s);

            status = oledWrite("   seconds");
            if (temp==25){
                status = oledWriteChar(s);
            }
            else{
                status = oledWriteChar(temp);
                status = oledWriteChar(s);

            }
            printf("Please try again in %d seconds\n",sec);
        }
        sleep(1000);
        status=oledClear();
        //printf(" seconds.%s\n");
    }
}

void addList(logData* node){
    if (head==NULL){
        head=node;
        head->next=NULL;
    }
    else{
        logData* temp=head;
        while(temp->next!=NULL){
            temp=temp->next;
        }
        temp->next=node;

    }
}


int main(int argc, char **argv)
{
    logData* dataLogging = new logData;
    dataLogging->logType="TRACE";
    dataLogging->event="DEVICE_LOCK";
    dataLogging->eventHour=getHour();
    dataLogging->next=NULL;
    addList(dataLogging);
    int test;
    test =getDay();
    test =getHour();
    int logInformation[7][24];
    //set contents of log array to 0
    for (int i = 0; i < 7; i++) {
        for (int k = 0; k < 24; k++) {
            logInformation[i][k] = 0;
        }
    }

    char charArray[168];    // add size
    for (int i=0;i<168;i++){
        charArray[i]=0;
    }
    int status=oledDriverInit();
    status = oledSetDisplayPower(1);
    int arrayUnlock [7][24];
    for (int i=0;i<7;i++){
        for (int j=0;j<24;j++){
            arrayUnlock[i][j]=0;
        }
    }

    int fd;
    if(argc < 2) {
        printf("usage: %s \n", argv[0]);
        return 1;
    }
    fd = open(argv[1], O_RDONLY);
    struct input_event ev;
    int input[20];
    for (int i=0;i<20;i++){
        input[i]=0;
    }
    int pass[10]={25,30,31,31,17,24,19,32,2,3};
    int counter=0;
    int wrongPassCounter=0;
    bool flag=true;
    bool locked=true;
    bool writeNUM=true;
    bool writeCAP=false;
    status=oledWrite("Device locked. Enter password.");
    //status=oledWrite("Device unlocked");
    //status=oledWrite("Type freely!\n");
    sleep(4000);
    status =oledClear();
    while(flag){
        writeFile(logInformation);
        int counter=0;
        for (int i=0;i<20;i++){
            input[i]=0;
        }
        //bool entered=false;
        //printf("checkpoint");
        while (counter<10 && locked)
        {
            writeFile(logInformation);
            read(fd, &ev, sizeof(struct input_event));
            if(ev.type == 1)
                //printf("key %i state %i\n", ev.code, ev.value);
                if(ev.value == 0){
                    if (counter==0){
                        status=oledClear();
                    }
                    //if (counter==0){
                    //status=oledClear();
                    //}
                    status = oledWriteChar('*');
                    //status = oledWriteChar(keyconverter(ev.code, writeNUM, writeCAP));
                    printf(" : [key %c]\n ", keyconverter(ev.code, writeNUM, writeCAP));
                    //printf(" : [key %i]\n ", ev.code);
                    if (ev.code==28){
                        status=oledClear();
                        break;
                    }
                    input[counter]=ev.code;
                    counter++;
                }
        }
        if (locked){
            int passCount=0;
            for (int i=0;i<10;i++){
                if (input[i]==pass[i]){
                    passCount++;
                }
            }
            if (passCount==10){
                status=oledClear();
                int index=0;
                while (charArray[index]!=0){
                    charArray[index]=0;
                    index++;
                }
                //status=oledWrite("Password is correct\n");
                //status=oledWrite("Device unlocked\n");
                status=oledWrite("You can now type freely");
                logData* dataLogging2 = new logData;
                dataLogging2->logType="TRACE";
                dataLogging2->event="CORRECT_PASS";
                dataLogging2->eventHour=getHour();
                dataLogging2->next=NULL;
                addList(dataLogging2);
                logData* dataLogging1 = new logData;
                dataLogging1->logType="TRACE";
                dataLogging1->event="DEVICE_UNLOCK";
                dataLogging1->eventHour=getHour();
                dataLogging1->next=NULL;
                addList(dataLogging1);
                sleep(4000);
                status=oledClear();
                locked=false;
            }
            else{
                if (wrongPassCounter==2){
                    logData* dataLogging3 = new logData;
                    dataLogging3->logType="TRACE";
                    dataLogging3->event="INCORRECT_PASS";
                    dataLogging3->eventHour=getHour();
                    dataLogging3->next=NULL;
                    addList(dataLogging3);
                    logData* dataLogging4 = new logData;
                    dataLogging4->logType="TRACE";
                    dataLogging4->event="LOCK_DELAY";
                    dataLogging4->eventHour=getHour();
                    dataLogging4->next=NULL;
                    addList(dataLogging4);
                    status = oledClear();
                    status = oledWrite("Too many tries.");
                    sleep(2000);
                    status = oledClear();
                    timer();
                    wrongPassCounter=0;
                    status = oledClear();
                    status=oledWrite("Device locked. Enter password.");
                    printf("Device is locked\n");
                }
                else{
                    logData* dataLogging5 = new logData;
                    dataLogging5->logType="TRACE";
                    dataLogging5->event="INCORRECT_PASS";
                    dataLogging5->eventHour=getHour();
                    dataLogging5->next=NULL;
                    addList(dataLogging5);
                    status = oledClear();
                    status = oledWrite("Password Incorrect.    Try again.");
                    sleep(2000);
                    status = oledClear();
                    status=oledWrite("Device locked. Enter password.");
                    printf("Device is locked\n");
                    wrongPassCounter++;
                }
            }
        }
        int unlockCount=0;
        int freeTypeCount=0;
        while (!locked)
        {
            writeFile(logInformation);
            /*	if (unlockCount==0){
            //   status = oledWrite("Device is unlocked\n");
            //status=oledClear();
        }
        */
            bool capNumCheck=false;
            wrongPassCounter=0;
            read(fd, &ev, sizeof(struct input_event));
            if(ev.type == 1)
                //printf("key %i state %i\n", ev.code, ev.value);
                if(ev.value == 0){
                    freeTypeCount++;
                    if (unlockCount==0){

                    }
                    //keyconverter(ev.code);
                    if (ev.code==58){
                        writeCAP=!writeCAP;
                        capNumCheck=true;
                        freeTypeCount--;
                    }
                    if (ev.code==69){
                        writeNUM=!writeNUM;
                        capNumCheck=true;
                        freeTypeCount--;
                    }
                    //status = oledWriteChar(keyconverter(ev.code, writeNUM, writeCAP));
                    //printf(" : [key %c]\n ",keyconverter(ev.code,writeNUM,writeCAP));
                    if (ev.code==1){
                        locked = true;
                        status=oledClear();
                        status=oledWrite("Device locked. Enter password.");
                        logData* dataLogging6 = new logData;
                        dataLogging6->logType="TRACE";
                        dataLogging6->event="DEVICE_LOCK";
                        dataLogging6->eventHour=getHour();
                        dataLogging6->next=NULL;
                        addList(dataLogging6);
                        printf("Device is locked\n");
                        break;
                    }
                    if (ev.code==29){
                        flag=false;
                        writeLog();
                        break;
                    }
                    if (ev.code==14 ){
                        freeTypeCount=freeTypeCount-2;
                    }
                    if (!capNumCheck){
                        if(ev.value ==0){ //when ev.value is 0, a key is being pressed
                            logUnlock(logInformation);
                        }
                        if (freeTypeCount==169){
                            status=oledClear();
                            freeTypeCount=1;
                        }
                        addToArray(charArray, ev.code, writeNUM, writeCAP );
                        writeArray(charArray);
                    }
                    //if (ev.code==69)
                    //writeNUM= !writeNUM;
                    input[counter]=ev.code;
                }
            unlockCount++;
        }
    }
}
