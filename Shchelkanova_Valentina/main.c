#include <stdio.h>
#include <math.h>

int main() {
    //Привет, Валя! Спасибо за помощь, желаю хорошего дня!
    float pr = 0.06;
    int tr_A = 40000;
    int zp_A = 150000;
    int i;
    int ak_A = 1000000;
    for(i=1;i<=240;i++) {
        if (i>=36) {
            pr = 0.08;
        }
        ak_A = ((ak_A + zp_A - tr_A) * (1 + pr/12)) ;
        //printf("Alice %d-%d\n", i,ak_A);//
    }
    int pz_B = 300000;
    int ak_B = 700000;
    int tr_B = 10000;
    int pk_B = 18364;
    int kv_B = 2000000;
    int zp_B = 150000;
    float price = 0.01;
    int k_B;
    float nalog = 0.001;
    pr = 0.06;
    for(i=1;i<=240;i++) {
        if (i>=36){
            pr = 0.08;
        }
        if (i=60){
            kv_B=kv_B*1.5;
        }
        kv_B = kv_B * (1+price/12);

        ak_B = ((ak_B+zp_B-tr_B-pk_B-(kv_B*nalog/12))*(1+pr/12));
       // printf("Bob %d-%d\n",i,ak_B);//
       // printf("%d",kv_B);//
    }
    printf("Alice %d\n Bob %d\n",ak_A,ak_B+kv_B);
    printf("Okay");
    return 0;
}
