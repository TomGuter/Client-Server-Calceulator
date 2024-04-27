#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/random.h>

// Name: noam elisha ID: 206919896	
// Name: Tom Gute    ID: 316487230



int myAtoi(const char *str);
char *myStrcat(char *dest, const char *src);
void intToString(int num, char *str);
int length(const char str[]);

// pid of the server, num1, operator , num2
// 1 == +
// 2 == -
// 3 == *
// 4 == /
int respone = 0;

void TimerHendler(int sig)
{
    int flag = 0;
    if (respone = 0)
    {
        printf("Client closed because no reponse was received from the server for 30 secounds");
        exit(1);
    }
}

void handler(int sig)
{
    alarm(0);
    char buff1[20];
    int j;
    for (j = 0; j < sizeof(buff1) - 1; j++)
    {
        buff1[j] = '\0';
    }

    pid_t pid1 = getpid();

    char str[100] = "to_client_";
    char str2[20];
    int pid1Int = (int)pid1;
    intToString(pid1Int, str2);
    myStrcat(str, str2);
    myStrcat(str, ".txt");

    int check = open(str, O_RDONLY);
    if (check < 0)
    {
        printf("ERROR_FROM_EX2");
        exit(1);
    }
    int rd, i = 0;
    while ((rd = read(check, &buff1[i], 1)) > 0)
    {
        if (buff1[i] == '\0')
        {
            break;
        }
        i++;
    }
    if (rd < 0)
    {
        printf("ERROR_FROM_EX2");
        exit(1);
    }

    int status;
    pid_t pid2 = fork();
    if (pid2 < 0)
    {
        printf("ERROR_FROM_EX2");
        exit(1);
    }
    if (pid2 == 0)
    {
        char *args[] = {"rm", str, NULL};
        int check2 = execvp(args[0], args);
        if (check2 < 0)
        {
            printf("ERROR_FROM_EX2");
            exit(1);
        }

        exit(1);
    }
    else
    {
        wait(&status);
        printf("end of stage 10\n");
        buff1[length(buff1)] = '\0';
        printf("%s", buff1);
        buff1[0] = '\0';
        respone = 1;
    }
}

int main(int argc, char *argv[])
{
	
    if (argc != 5)
    {
        printf("ERROR_FROM_EX2");
        exit(1);
    }
    pid_t pidServer = myAtoi(argv[1]); // convert char to pid

    int fd = open("to_srv.txt", O_RDWR | O_CREAT, 0666); // toServer is common
    // between all the clients
    //  if the open fail, random number between 1-5 to sleep secound
    //
    if (fd < 0)
    {

        int i;
        for (i = 0; i < 10; i++)
        {
            fd = open("to_srv.txt", O_RDWR | O_CREAT, 0666);
            if (fd < 0)
            {
                int random;
                int p;
                int result = getrandom(&random, sizeof(random), 0);
                if (result == sizeof(random))
                {
                    p = (random % 5) + 1;
                    sleep(p);
                }
                else
                {
                    printf("ERROR_FROM_EX2");
                    exit(1);
                }
            }
            else
            {
                break;
            }
        }
        printf("end of stage 5\n");
        if (i == 10)
        {
            printf("ERROR_FROM_EX2");
            exit(1);
        }
    }
    pid_t pid = getpid();
    char pidstr[30];
    int pidInt = (int)pid;
    intToString(pidInt, pidstr);
    pidstr[length(pidstr)] = '\0';

    argv[1] = pidstr;
    int j = 1;

    while (j < 5)
    {
        char *buffer = argv[1];

        buffer[length(buffer)] = '\0';
        int rd = write(fd, buffer, length(buffer));
        if (rd < 0)
        {
            printf("ERROR_FROM_EX2");
            exit(1);
        }
        rd = write(fd, "\n", 1);
        buffer[0] = '\0';
        argv[1] = argv[j + 1];

        j++;
    }

    close(fd);
    printf("end of stage 4\n");
    pid_t timer = fork();
    if (timer < 0)
    {
        printf("ERROR_FROM_EX2");
        exit(1);
    }
    if (timer == 0)
    {
        int e = kill(pidServer, 1);
        if (e < 0)
        {
            printf("ERROR_FROM_EX2");
        }
        exit(0);
    }
    if (timer > 0)
    {
        int time = 30;
        signal(SIGALRM, TimerHendler);
        alarm(time);
        signal(2, handler);
        pause(); // wait the respond of the server
    }
}

// custom functions:

int length(const char str[])
{
    int i = 0;
    while (str[i] != '\0')
    {
        i++;
    }
    return i;
}

int myAtoi(const char *str)
{
    int result = 0;
    int sign = 1;
    int i = 0;

    while (str[i] == ' ')
    {
        i++;
    }

    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
        {
            sign = -1;
        }
        else
        {
            sign = 1;
        }
        i++;
    }

    while (str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return sign * result;
}

char *myStrcat(char *dest, const char *src)
{
    char *ptr = dest;
    while (*ptr != '\0')
    {
        ptr++;
    }
    while ((*ptr++ = *src++) != '\0')
        ;
    return dest;
}

void intToString(int num, char *str)
{
    int i = 0;
    int sign = 1;

    if (num < 0)
    {
        sign = -1;
        num = -num;
    }

    do
    {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0);

    if (sign == -1)
    {
        str[i++] = '-';
    }

    int start = 0;
    int end = i - 1;
    while (start < end)
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }

    str[i] = '\0';
}
