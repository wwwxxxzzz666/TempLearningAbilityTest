//#define WIN
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cmath>
#include <iostream>
#ifndef WIN
#include "cconio.h"
#else
#include <conio.h>
#endif

using namespace std;

double max(double a,double b)
{
    return ((a>b)?a:b);
}

void wait(int ms)
{
	clock_t endwait;
	endwait = clock() + ms * CLOCKS_PER_SEC / 1000;//CLOCKS_PER_SEC宏定义1000,标准时间差
	while(clock()<endwait);
}

char *col[16]={
"\033[m",         //NONE 0
"\033[0;32;31m",  //RED 1
"\033[1;31m",     //LIGHT_RED 2
"\033[0;32;32m",  //GREEN 3
"\033[1;32m",     //LIGHT_GREEN 4
"\033[0;32;34m",  //BLUE 5
"\033[1;34m",     //LIGHT_BLUE 6
"\033[1;30m",     //GRAY 7
"\033[0;36m",     //CYAN 8
"\033[1;36m",     //LIGHT_CYAN 9
"\033[0;35m",     //PURPLE 10
"\033[1;35m",     //LIGHT_PURPLE 11
"\033[0;33m",     //BROWN 12
"\033[1;33m",     //YELLOW 13
"\033[0;37m",     //LIGHT_GRAY 14
"\033[1;37m"};    //WHITE 15

void print(char *ss,int fc,int bc=0)
{
	printf("%s\e[4%dm%2s\e[0m",col[fc],bc,ss);
}

inline void clearin()
{
	while (kbhit()) getch();
}

/*void clrscr()
{
	printf("\33[2J");
}

void gotoxy(int x,int y)
{
	printf("\033[%d;%dH",x,y);
}*/

double ans=0;  //最终结果
double surv,reac,memo,calc,colo,hold;  //量表，反应（已弃用），记忆，计算，色名，憋气（已弃用）
int jiyis[4];  //记忆测试三次子测试的正确数
char t=' ';

double survw[10]={0,1.0/9,1.0/9,1.0/9,1.0/9,1.0/9,1.0/9,1.0/9,1.0/9,1.0/9};  //各问卷题占survey的权重
char survq[10][700];  //各个问题
char survo[10][4][700];  //各个问题的选项

double ww[5]={0,0.566014,0.137398,0.0877892,0.208799};  //各个模块占总结果的权重

void init()  //问题内容初始化
{
    strcpy(survq[1],"1 ,请问你现在说话时是什么状态和感受？\n");
    strcpy(survo[1][1]," 1 :轻松有力，声音很亮，可以浑厚\n");
    strcpy(survo[1][2]," 5 :不是那么有力的感觉，如果没有必要不想多说\n");
    strcpy(survo[1][3]," 9 :感到有气无力，自然发出的音量明显偏小，且声音偏细\n");

    strcpy(survq[2],"2 ,请问你现在腿部感受如何？\n");
    strcpy(survo[2][1]," 1 :感到舒适轻松有力，起身走动没问题\n");
    strcpy(survo[2][2]," 5 :不是很确定，没有很直接的感觉\n");
    strcpy(survo[2][3]," 9 :感到疲乏/酸胀/无力，不想走动\n");

    strcpy(survq[3],"3 ,请问你现在手臂感受如何？\n");
    strcpy(survo[3][1]," 1 :感到舒适轻松有力，拿/搬东西洗衣服没问题\n");
    strcpy(survo[3][2]," 5 :不是很确定，没有很直接的感觉\n");
    strcpy(survo[3][3]," 9 :感到疲乏/酸胀/无力，不想做洗衣服这类事情\n");

    strcpy(survq[4],"4 ,请问你现在脖子感受如何？\n");
    strcpy(survo[4][1]," 1 :感到舒适轻松，无任何不适感\n");
    strcpy(survo[4][2]," 5 :有一点点不适，稍微抬头扭转活动一下会感到舒服许多\n");
    strcpy(survo[4][3]," 9 :明显感到酸胀/酸痛等不适，即使活动拉伸好一会仍有不适\n");

    strcpy(survq[5],"5 ,请问你现在眼睛感受如何？\n");
    strcpy(survo[5][1]," 1 :视觉感到舒适清晰，仿佛刚做过眼保健操\n");
    strcpy(survo[5][2]," 5 :没什么直接的很深的感觉\n");
    strcpy(survo[5][3]," 9 :感到疲乏/酸胀/干涩，睁眼/聚焦看清楚东西有些费劲，无力\n");

    strcpy(survq[6],"6 ,请问你现在肩膀、上背感受如何？\n");
    strcpy(survo[6][1]," 1 :感到舒适轻松有力，无不适感\n");
    strcpy(survo[6][2]," 5 :没有什么强烈不适感，但将双肩外展拉伸后能立刻感到舒适许多\n");
    strcpy(survo[6][3]," 9 :明显感到疲乏/酸痛，多次/不停外展拉伸才能缓解一些\n");

    strcpy(survq[7],"7 ,请问你现在觉得？\n");
    strcpy(survo[7][1]," 1 :精力比较充沛，在床上可能要过很久才能睡着甚至失眠\n");
    strcpy(survo[7][2]," 5 :精力一般/偏弱，闭上眼/静下来有一些困意，在床上需要一点时间才能睡着\n");
    strcpy(survo[7][3]," 9 :感到精力十分疲乏，想保持清醒不睡是一件困难/挣扎的事情\n");

    strcpy(survq[8],"8 ,请问你现在阅读文字？\n");
    strcpy(survo[8][1]," 1 :注意力，理解力，注意力充足，获取/弄懂信息轻松\n");
    strcpy(survo[8][2]," 5 :略有一些心不在焉的感觉，偶尔有地方读完了却没看进去\n");
    strcpy(survo[8][3]," 9 :感到阅读理解较为困难，已经明显感到保持注意力很困难\n");
    
    strcpy(survq[9],"9 ,请问你现在觉得自己的学习状态是？\n");
    strcpy(survo[9][1]," 1 :极佳，对知识吸收非常主动，感觉学习过程轻松迅速\n");
    strcpy(survo[9][2]," 5 :一般，时而觉得容易，时而觉得困难，学了一会后会想暂停\n");
    strcpy(survo[9][3]," 9 :糟糕，学得很困难，有些闷燥，感觉像是强迫自己在学习\n");

}

inline void zhuguan()
{
    puts("一：主观测试");
    for (int i=1;i<=9;i++)
    {
        print(survq[i],9);
        print(survo[i][1],13);
        print(survo[i][2],13);
        print(survo[i][3],13);
        while ((t!='1')&&(t!='5')&&(t!='9')) t=getch();
        switch (t)
        {
            case '1':surv-=30*survw[i]; break;
            case '5':surv+=10*survw[i]; break;
            case '9':surv+=40*survw[i]; break;
        }
        t=' ';
        print("~~~~~~~~~~~~~~~~~~\n",11);
        clrscr(); gotoxy(1,1);
    }
}

inline void ready()
{
    clrscr();
    wait(1000);
    gotoxy(2,5);
    print("3",13);
    wait(1000);
    gotoxy(2,5);
    print("2",13);
    wait(1000);
    gotoxy(2,5);
    print("1",13);
    wait(1000);
    gotoxy(2,1);
    print("     GO!",13);
    wait(1000);
    clrscr();
}

inline void fanying()  //已废弃
{
    print("1,反应力测试\n",9);
}

inline void jiyi()
{
    double score=0;
    clrscr(); gotoxy(1,1);
    print("1,闪烁数字记忆\n",9);
    print("若有记不得的数字输入0代替；建议从左到右记忆，只输入记得的部分，不确定的部分不要输入\n",13);
    print("按下5开始测试",13);
    t=' ';
    clearin();
    while (t!='5') t=getch();
    ready();
    for(int k=3;k>=1;k--)
    {
        clrscr(); gotoxy(1,1);
        print("请记住数字",9);
        char a[66];
        for (int i=1;i<=20;i++) a[i]='1'+(rand()%9);
        a[11]='\0';
        gotoxy(2,1);
        for (int i=1;i<=20;i++) print("*",7);
        wait(200);
        gotoxy(2,1);
        for (int i=1;i<=20;i++) printf("%c",a[i]);
        wait(1300*k);
        gotoxy(2,1); printf("            ");
        clrscr(); gotoxy(1,1);
        print("请输入你的答案：\n",9);
        char ts[66];
        clearin();
        scanf("%s",&ts);
        for (int i=1;i<=((strlen(ts)<20)?strlen(ts):20);i++)
        {
            if (ts[i-1]==a[i]) {jiyis[4-k]++; score+=5;}
        }
    }
    memo-=(score-150)/5;
}

inline void jisuan()
{
    double score=0;
    clrscr(); gotoxy(1,1);
    print("2,快速口算选择\n",9);
    print("对于每道题分别有左中右三个选项，按数字键盘的左中右各三个按键进行选择\n",13);
    print("请务必将输入法切换至数字9键盘进行本测试\n",1);
    print("按下5开始测试",13);
    t=' ';
    clearin();
    while (t!='5') t=getch();
    ready();
    for (int k=1;k<=5;k++)
    {
        clrscr(); gotoxy(1,1);
        print("请在左中右三个选项中选择：\n",13);
        print("题目： ",9);
        print(" ***********",9);
        wait(900);
        gotoxy(3,1); print("题目： ",9);
        int t1,t2,t3,t4,t5,t6;
        t1=1+rand()%20; t2=1+rand()%10;
        t3=1+rand()%10; t4=1+rand()%20;
        t5=rand()%2;
        if (t5==0) t6=t1*t2+t3*t4; else t6=t1*t2-t3*t4;
        if (t5==0) printf("\e[1;36m %d*%d+%d*%d=? \e[0m",t1,t2,t3,t4);
        else printf("\e[1;36m %d*%d-%d*%d=? \e[0m",t1,t2,t3,t4);
        int sel[3];
        int cor=rand()%3;
        sel[cor]=t6;
        int tt=rand()%2;
        if (tt==0) sel[(cor+1)%3]=t6+(1+rand()%2)*10; else sel[(cor+1)%3]=t6-(1+rand()%2)*10;
        do   
        {
            int tt=rand()%2;
            if (tt==0) sel[(cor+2)%3]=t6+(1+rand()%2)*10; else sel[(cor+2)%3]=t6-(1+rand()%2)*10;
        }while (sel[(cor+1)%3]==sel[(cor+2)%3]);
        gotoxy(5,3);
        printf("\e[1;33m %d     %d     %d \e[0m",sel[0],sel[1],sel[2]);
        clock_t ts=clock();
        int users=0;
        clearin();
        while ((t<='1')||(t>='9')) t=getch();
        switch (t)
        {
            case '1':case '4':case '7':users=0; break;
            case '2':case '5':case '8':users=1; break;
            case '3':case '6':case '9':users=2; break;
        }
        t=' ';
        clock_t te=clock();
        if (users==cor) score+=max(0,3*(5+0.7*(3.5-(te-ts)/1000)));
    }
    calc=120-score;
}

inline void seming()
{
    int colmapid[6]={1,3,5,10,13,15};
    char *colmapname[6]={"红色","绿色","蓝色","紫色","黄色","白色"};
    double score=0;
    clrscr(); gotoxy(1,1);
    print("3,色名判断测试\n",9);
    print("对于每道题分别有左中右三个选项，按数字键盘的左中右各三个按键进行选择\n",13);
    print("请务必将输入法切换至数字9键盘进行本测试\n",1);
    print("按下5开始测试",13);
    t=' ';
    clearin();
    while (t!='5') t=getch();
    ready();
    for (int k=1;k<=5;k++)
    {
        clrscr(); gotoxy(1,1);
        print("请在左中右三个选项中选择出色名对应正确的那一项：\n",13);
        print("题目： ",9);
        print(" ***********",9);
        wait(900);
        gotoxy(3,1); print("题目： ",9);
        int selcol[3];
        int selnam[3];
        int cor=rand()%3;
        selcol[cor]=selnam[cor]=rand()%6;
        int tt=rand()%2;
        for (int i=cor+1;i<=cor+2;i++)
        {
            int t1=rand()%6;
            int t2;
            do
            {
                t2=rand()%6;
            }while(t1==t2);
            selcol[i%3]=t1; selnam[i%3]=t2;
        }
        gotoxy(5,3);
        printf(" "); print(colmapname[selnam[0]],colmapid[selcol[0]]);
        printf("     "); print(colmapname[selnam[1]],colmapid[selcol[1]]);
        printf("     "); print(colmapname[selnam[2]],colmapid[selcol[2]]);
//        printf("\e[1;33m %d     %d     %d \e[0m",sel[0],sel[1],sel[2]);
        clock_t ts=clock();
        int users=0;
        clearin();
        while ((t<='1')||(t>='9')) t=getch();
        switch (t)
        {
            case '1':case '4':case '7':users=0; break;
            case '2':case '5':case '8':users=1; break;
            case '3':case '6':case '9':users=2; break;
        }
        t=' ';
        clock_t te=clock();
        if (users==cor) score+=max(0,8*(1+1.0*(0.9-(te-ts)/1000)));
    }
    colo=120-score;
}

int main()
{
#ifdef WIN
	system("cls");
#endif
    setbuf(stdin,NULL);
    setbuf(stdout,NULL);
    srand((unsigned int)time(NULL));
    char cc='A';
    cout<<--cc;
    init();
//    for (int i=1;i<=8;i++)
//        printf("%lf\n",survw[i]);
    surv=reac=memo=calc=colo=hold=60;
    print("建议在右上方设置处将显示界面字体大小调成20或16，调整后请重启应用\n",1);
    print("建议将输入法调成数字9键面板模式(仅输入数字)\n",1);
    zhuguan();
    puts("二：客观测试");
//    fanying();
    jiyi();
    jisuan();
    seming();

    ans=surv*ww[1]+memo*ww[2]+calc*ww[3]+colo*ww[4];
    clrscr(); gotoxy(1,1);
    time_t tt = time(NULL);
    struct tm *t= localtime(&tt);
    printf("%d-%02d-%02d %02d:%02d:%02d\n", 
    t->tm_year + 1900,
    t->tm_mon + 1,
    t->tm_mday,
    t->tm_hour,
    t->tm_min,
    t->tm_sec);
    printf("您的短时学习疲劳指数：%.2lf\n",ans);
    printf("您的主观学习疲劳指数：%.2lf\n",surv);
    printf("您的短时记忆疲劳指数：%.2lf\n",memo);
    printf("您的运算疲劳指数：%.2lf\n",calc);
    printf("您的观察注意疲劳指数：%.2lf\n",colo);
    print("请截图保存测试结果~ \n",13);
    print("Version 0.7 By loww科学与社会研讨课题组",6);
    while (1) getch();
    getch();
}
