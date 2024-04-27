#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

// Name: noam elisha ID: 206919896	
// Name: Tom Gute    ID: 316487230




int myAtoi(const char *str);
char *myStrcat(char *dest, const char *src);
void intToString(int num, char *str);
int length(const char str[]);

void TimerHendler(int sig)
{
    printf("The server was closed becuse no service, request was received for the last 60 secound \n");
    exit(0);
}

void handler1(int sig)
{
    printf("end of stage 1\n");

    int fd = open("to_srv.txt", O_RDWR | O_CREAT, 0666);
    if (fd < 0)
    {
        printf("ERROR_FROM_EX2");
        exit(1);
    }
    printf("end of stage 2\n");
    char buff[100], args1[100], args2[100], args3[100], args4[100];

    printf("end of stage 3\n");

    printf("end of stage 6\n");
    int status;
    pid_t pid = fork();
    if (pid < 0)
    {
        printf("ERROR_FROM_EX2");
        exit(1);
    }
    if (pid == 0)
    {
        int fd1 = read(fd, &buff, 100);
        buff[length(buff)] = '\0';
        if (fd1 < 0)
        {
            printf("ERROR_FROM_EX2");
            exit(1);
        }

        if (remove("to_srv.txt") == -1)
        {
            printf("ERROR_FROM_EX2");
            exit(1);
        }
        else
        {
            printf("end of stage 7\n");
            int i = 0, flag1 = 0, rd;
            int sum;
            int arg1;
            int arg2;
            int arg3;
            int arg4;

            int l = lseek(fd, 0, SEEK_SET);
            if (l < 0)
            {
                printf("ERROR_FROM_EX2");
                exit(1);
            }
            while ((rd = read(fd, &buff[i], 1)) > 0)
            {
                if (buff[i] == '\n' && flag1 == 0)
                {
                    buff[i] = '\0';
                    arg1 = myAtoi(buff);
                    buff[0] = '\0';
                    i = 0;
                    flag1 = 1;
                }
                else if (buff[i] == '\n' && flag1 == 1)
                {
                    buff[i] = '\0';
                    arg2 = myAtoi(buff);
                    buff[0] = '\0';
                    i = 0;
                    flag1 = 2;
                }
                else if (buff[i] == '\n' && flag1 == 2)
                {
                    buff[i] = '\0';
                    arg3 = myAtoi(buff);
                    buff[0] = '\0';
                    i = 0;
                    flag1 = 3;
                }
                else if (buff[i] == '\n' && flag1 == 3)
                {
                    buff[i] = '\0';
                    arg4 = myAtoi(buff);
                    buff[0] = '\0';
                    i = 0;
                    flag1 = 0;
                    break;
                }
                else
                {
                    i++;
                }
            }
            if (rd < 0)
            {
                printf("ERROR_FROM_EX2");
                exit(1);
            }

            char str1[100] = "to_client_";
            char str2[20];
            intToString(arg1, str2);
            myStrcat(str1, str2);
            myStrcat(str1, ".txt");

            int fd4 = open(str1, O_RDWR | O_CREAT, 0666); // is unique for each client
            if (fd4 < 0)
            {
                printf("ERROR_FROM_EX2");
                exit(1);
            }
            printf("end of stage 8\n");

            if (arg3 == 1)
            {
                sum = arg2 + arg4;
            }
            if (arg3 == 2)
            {
                sum = arg2 - arg4;
            }
            if (arg3 == 3)
            {
                sum = arg2 * arg4;
            }
            if (arg3 == 4)
            {
                sum = arg2 / arg4;
            }

            char sum2[20];
            int j;
            for (j = 0; j < sizeof(sum2) - 1; j++)
            {
                sum2[j] = '\0';
            }

            intToString(sum, sum2);
            sum2[length(sum2)] = '\n';

            int check6 = write(fd4, &sum2, length(sum2));
            if (check6 < 0)
            {
                printf("ERROR_FROM_EX2");
                exit(1);
            }
            close(fd4);
            printf("end of stage 9\n");
            pid_t clientPid;
            clientPid = (pid_t)arg1;
            int w = kill(clientPid, 2); // the kill send signal to the pause of the main client
            if (w < 0)
            {
                printf("ERROR_FROM_EX2");
                exit(1);
            }
        }
        exit(0);
    }
    if (pid > 0)
    {
        pid_t childPid = waitpid(pid, &status, 0);
        if (childPid == -1)
        {
            printf("ERROR_FROM_EX2");
            exit(1);
        }
        signal(1, handler1);
    }
}

int main()
{
    signal(1, handler1);
    signal(SIGALRM, TimerHendler);
    while (1)
    {
        int time = 60;
        alarm(time);
        pause(); // go to sleep, wait for client, every time take care of client and go
                 // to sleep
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
