//
//  mian.c
//  TicketOptim
//
//  Created by 橋本悠樹 on 2022/05/11.
//

#include <stdio.h>
#include <stdlib.h>

#define MAX_REQUIRED 99999
#define NUM_BUNCHES 8
#define BUF_SIZE 256
#define INF 999999

typedef struct {
    int num;
    int cost;
} t_bunch;

typedef struct {
    int cost;
    int pre_index;
} t_dpelem;

int input_required(void);
void do_dp(int req, t_dpelem table[][MAX_REQUIRED+1]);
void print_dp_table(int req, t_dpelem table[][MAX_REQUIRED+1]);
void print_result(int req, t_dpelem table[][MAX_REQUIRED+1]);

const t_bunch ticket_bunches[NUM_BUNCHES] = {
    {60,120}, {306,610}, {492,980}, {924,1840}, {1226,2440}, {1850,3680}, {2470,4900}, {5050,10000}
};


int main()
{
    int required;
    // dp_table[a][b] : チケット束aまでを用いてb枚以上のチケットを買うための最低金額
    t_dpelem dp_table[NUM_BUNCHES][MAX_REQUIRED+1];
    
    required = input_required();
    do_dp(required, dp_table);
    print_result(required, dp_table);
}

int input_required()
{
    char buf[BUF_SIZE];
    int req;
    do {
        printf("購入したい枚数 : ");
        if(fgets(buf, BUF_SIZE, stdin) == NULL) {
            fprintf(stderr, "error: fgets()\n");
            exit(1);
        }
        req = 0;
        int i = 0;
        while(buf[i] != '\n') {
            if('0' <= buf[i] && buf[i] <= '9') {
                req = req*10 + (buf[i]-'0');
                i++;
            } else {
                req = 0;
                break;
            }
        }
    } while(req <= 0 || MAX_REQUIRED < req);

    return req;
}

void do_dp(int req, t_dpelem table[][MAX_REQUIRED+1])
{
    table[0][0].cost = 0;
    table[0][0].pre_index = 0;
    for(int i = 1; i <= req; i++) {
        table[0][i].cost = ((i-1)/ticket_bunches[0].num + 1) * ticket_bunches[0].cost;
        table[0][i].pre_index = 0;
    }
    
    for(int j = 1; j < NUM_BUNCHES; j++) {
        table[j][0].cost = 0;
        table[j][0].pre_index = 0;
        for(int i = 1; i <= req; i++) {
            table[j][i].cost = table[j-1][i].cost;
            table[j][i].pre_index = i;
            int num_tb_j = 1;
            int rem;
            do {
                rem = i - num_tb_j * ticket_bunches[j].num;
                int tmp_cost = table[j-1][rem<0 ? 0 : rem].cost + num_tb_j * ticket_bunches[j].cost;
                if(tmp_cost <= table[j][i].cost) {
                    table[j][i].cost = tmp_cost;
                    table[j][i].pre_index = (rem<0 ? 0 : rem);
                }
                num_tb_j++;
            } while(rem > 0);
        }
    }
}

void print_dp_table(int req, t_dpelem table[][MAX_REQUIRED+1])
{
    for(int i = req; i >= 0; i--) {
        printf("%4d: ", i);
        for(int j = 0; j < NUM_BUNCHES; j++) {
            printf("%4d(%4d) ", table[j][i].cost, table[j][i].pre_index);
        }
        printf("\n");
    }
}

void print_result(int req, t_dpelem table[][MAX_REQUIRED+1])
{
    printf("*** 計算結果 ***\n");
    int sum_tickets = 0;
    int rem = req;
    for(int j = NUM_BUNCHES-1; j >= 0; j--) {
        if(rem != table[j][rem].pre_index) {
            int num_tb_j = (rem - table[j][rem].pre_index - 1) / ticket_bunches[j].num + 1;
            sum_tickets += ticket_bunches[j].num * num_tb_j;
            printf("%4d枚 x %d\n", ticket_bunches[j].num, num_tb_j);
        }
        rem = table[j][rem].pre_index;
    }
    printf("(%d枚)\n\n", sum_tickets);
    printf("合計金額：¥%d\n", table[NUM_BUNCHES-1][req].cost);
}
     
