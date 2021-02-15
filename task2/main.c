#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define SUM_OF_PURCHASE 200

int shops[5] = {1000, 1050, 1100, 1150, 1200};

// mutexes for every shop
pthread_mutex_t mutexes[5];

typedef struct customer {
    int id;
    int full_need;
} t_customer;

void    *load_products(void *args) {
    int id_loading_shop;

    while (1) {
        id_loading_shop = rand() % 5;
        if (pthread_mutex_trylock(&mutexes[id_loading_shop]) == 0) {
            shops[id_loading_shop] += 200;
            printf("Load in shop %d +200 -> shop balance: %d\n", id_loading_shop, shops[id_loading_shop]);
            sleep(1);
            pthread_mutex_unlock(&mutexes[id_loading_shop]);
        }
    }
}

void    *buy_products(void *arg) {
    t_customer *customer = (t_customer *)arg;
    int id_shop = 0;

     while (1) {
        // check if shop is empty
        if (pthread_mutex_trylock(&mutexes[id_shop]) == 0) {
            if (shops[id_shop] > SUM_OF_PURCHASE) {
                shops[id_shop] -= SUM_OF_PURCHASE;
                if (customer->full_need - SUM_OF_PURCHASE < 0)
                    customer->full_need = 0;
                else
                    customer->full_need -= SUM_OF_PURCHASE;
            }
            
            printf("Customer: %d BUYING in shop %d; left for need = %d \n", customer->id, id_shop, customer->full_need);
            if (customer->full_need <= 0) {
                printf("Customer: %d is satisfied\n", customer->id);
                break ;
            }
            sleep(3);
            pthread_mutex_unlock(&mutexes[id_shop]);
        }
        id_shop++;
        if (id_shop == 5)
            id_shop = 0;
    }
}

void    init_customer(t_customer *cust, int id, int need) {
    cust->id = id;
    cust->full_need = need;
}

void    print_customers(t_customer custms[3]) {
    for (int i = 0; i < 3; i++) {
        printf("id = %d\n", custms[i].id);
        printf("full_need = %d\n", custms[i].full_need);
        printf("========================\n");
    }
}

int     main(int argc, char *argv) {
    pthread_t thread_loader;
    pthread_t thread_customer[3];
    t_customer customers[3];
    int start_need = 3000;
    int status_addr;
    int status_addr2[3];
    int status_end;
    
    for (int i = 0; i < 3; i++) {
        init_customer(&customers[i], i, start_need);
        start_need += 150;
    }
    print_customers(customers);

    for (int i = 0; i < 5; i++)
        pthread_mutex_init(&mutexes[i], NULL);

    int status = pthread_create(&thread_loader, NULL, load_products, NULL);
    for (int i = 0; i < 3; i++) {
        pthread_create(&thread_customer[i], NULL, buy_products, (void *) &customers[i]);
    }
    
    for (int i = 0; i < 3; i++) 
        pthread_join(thread_customer[i], (void **)&status_addr2[i]);
    // when all customer is sutisfied, loader closed
    pthread_detach(thread_loader);

    pthread_join(thread_loader, (void**)&status_addr);

    for (int i = 0; i < 5; i++)
        pthread_mutex_destroy(&mutexes[i]);

    return (0);
}