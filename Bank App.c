
// This program will essetially creat a banking app. giving the user abilities to create an account,
// deposit, withdraw, and sort multiple accounts.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXIMUM_ACC 10
#define NAMELENG 30

typedef struct {
    int acNumb;
    char acType_;
    char acHolder_Name[NAMELENG + 1];
    char acOpendate[11];
    double balnceAM;
} bank_Acc;

bank_Acc arr_Accs[MAXIMUM_ACC];
int numOfacc = 0;

// function prototypes
int findACC(int acNumb);
void creatACC(void);
void depMoney(void);
void takeOut_money(void);
void clean_displayAcc(void);
int cmp_acNumb(const void *a, const void *b);
int cmp_acHolder_Name(const void *a, const void *b);
int cmp_acOpendate(const void *a, const void *b);
int cmp_balnceAM(const void *a, const void *b);
int aprvDate(const char *date);

int main() {
    char operationInp;

    for (;;) {
        printf("******* Main menu *******\n");
        printf("c(reate an account)\n");
        printf("d(eposit money)\n");
        printf("w(ithdraw money)\n");
        printf("s(sort and print accounts)\n");
        printf("q(uit program)\n");
        printf("*************************\n");
        printf("Enter operation code (c, d, w, s, q): ");
        scanf(" %c", &operationInp);
        
        switch (tolower(operationInp)) {
            case 'c': creatACC(); break;
            case 'd': depMoney(); break;
            case 'w': takeOut_money(); break;
            case 's': clean_displayAcc(); break;
            case 'q': return 0;
            default: printf("Invalid code. Try again.\n");
        }
        printf("\n");
    }
}

// Function to find index of an account
int findACC(int acNumb) {
    for (int i = 0; i < numOfacc; i++) {
        if (arr_Accs[i].acNumb == acNumb) {
            return i;
        }
    }
    return -1;
}

// Function to create a new account
void creatACC(void) {
    bank_Acc nEw_uSer;
    char inpNewacc[100];

    if (numOfacc >= MAXIMUM_ACC) {
        printf("Cannot create more accounts. Maximum limit reached.\n");
        return;
    }

    while (1) {
        printf("Enter account number (4 digits): ");
        scanf("%s", inpNewacc);
        nEw_uSer.acNumb = atoi(inpNewacc);
        if (nEw_uSer.acNumb < 1000 || nEw_uSer.acNumb > 9999) {
            printf("Enter a number between 1000 and 9999.\n");
        } else if (findACC(nEw_uSer.acNumb) != -1) {
            printf("Account number already exists. Choose a different number.\n");
        } else {
            break;
        }
    }

    while (1) {
        printf("Enter account type: c(hecking), s(avings)\nc or s: ");
        scanf(" %c", &nEw_uSer.acType_);
        nEw_uSer.acType_ = tolower(nEw_uSer.acType_);
        if (nEw_uSer.acType_ != 'c' && nEw_uSer.acType_ != 's') {
            printf("Wrong account type. Enter c or s.\n");
        } else {
            break;
        }
    }

    printf("Enter account holder's name: ");
    scanf(" %[^\n]", nEw_uSer.acHolder_Name);
    nEw_uSer.acHolder_Name[NAMELENG] = '\0';

    while (1) {
        printf("Enter open date (YYYY-MM-DD): ");
        scanf("%s", nEw_uSer.acOpendate);
        if (!aprvDate(nEw_uSer.acOpendate)) {
            printf("%s Wrong date format.\n", nEw_uSer.acOpendate);
        } else {
            break;
        }
    }

    printf("Enter balance (number only): $ ");
    scanf("%lf", &nEw_uSer.balnceAM);

    arr_Accs[numOfacc++] = nEw_uSer;
    printf("Account %d is created.\n", nEw_uSer.acNumb);
}

// function to deposit money
void depMoney(void) {
    int acNumb, indx;
    double mnyCnt;

    printf("Enter account number: ");
    scanf("%d", &acNumb);

    indx = findACC(acNumb);
    if (indx == -1) {
        printf("Account not found.\n");
        return;
    }

    while (1) {
        printf("Enter amount (> 0, number only): $ ");
        scanf("%lf", &mnyCnt);
        if (mnyCnt <= 0) {
            printf("Amount must be greater than 0.\n");
        } else {
            break;
        }
    }

    arr_Accs[indx].balnceAM += mnyCnt;
    printf("Remaining balance: $ %-15.2f\n", arr_Accs[indx].balnceAM);
}

// function that enables withdrawing money 
void takeOut_money(void) {
    int acNumb, indx;
    double mnyCnt;

    printf("Enter account number: ");
    scanf("%d", &acNumb);

    indx = findACC(acNumb);
    if (indx == -1) {
        printf("Account not found.\n");
        return;
    }

    while (1) {
        printf("Enter amount (> 0, number only): $ ");
        scanf("%lf", &mnyCnt);
        if (mnyCnt <= 0) {
            printf("Amount must be greater than 0.\n");
        } else if (mnyCnt > arr_Accs[indx].balnceAM) {
            printf("Insufficient balance.\n");
            return;
        } else {
            break;
        }
    }

    arr_Accs[indx].balnceAM -= mnyCnt;
    printf("Remaining balance: $ %-15.2f\n", arr_Accs[indx].balnceAM);
}

// function that sorts and display the acounts based on user input
void clean_displayAcc(void) {
    char sortChoice;
    int (*cmp_func)(const void*, const void*);

    printf("Enter the sorting field: a(ccount number), h(older name), o(pen date), b(alance)\n");
    printf("a, h, o or b: ");
    scanf(" %c", &sortChoice);

    switch (tolower(sortChoice)) {
        case 'a': cmp_func = cmp_acNumb; break;
        case 'h': cmp_func = cmp_acHolder_Name; break;
        case 'o': cmp_func = cmp_acOpendate; break;
        case 'b': cmp_func = cmp_balnceAM; break;
        default: 
            printf("Invalid sorting field.\n");
            return;
    }

    qsort(arr_Accs, numOfacc, sizeof(bank_Acc), cmp_func);

    printf("#### Type Holder name                     Open date   Balance\n");
    printf("--------------------------------------------------------------------\n");
    for (int i = 0; i < numOfacc; i++) {
        printf("%-4d %c    %-30s %-10s %15.2f\n", 
               arr_Accs[i].acNumb,
               toupper(arr_Accs[i].acType_),
               arr_Accs[i].acHolder_Name,
               arr_Accs[i].acOpendate,
               arr_Accs[i].balnceAM);
    }
    printf("--------------------------------------------------------------------\n");
}

// compare functions
int cmp_acNumb(const void *x, const void *y) {
    return ((bank_Acc *)x)->acNumb - ((bank_Acc *)y)->acNumb;
}

int cmp_acHolder_Name(const void *x, const void *y) {
    return strcmp(((bank_Acc *)x)->acHolder_Name, ((bank_Acc *)y)->acHolder_Name);
}

int cmp_acOpendate(const void *x, const void *y) {
    return strcmp(((bank_Acc *)x)->acOpendate, ((bank_Acc *)y)->acOpendate);
}

int cmp_balnceAM(const void *x, const void *y) {
    if (((bank_Acc *)x)->balnceAM < ((bank_Acc *)y)->balnceAM) return -1;
    if (((bank_Acc *)x)->balnceAM > ((bank_Acc *)y)->balnceAM) return 1;
    return 0;
}

// Function to validate dates entered
int aprvDate(const char *date) {
    int ye_ar, mon_th, d_ay;
    return (strlen(date) == 10 &&
            sscanf(date, "%d-%d-%d", &ye_ar, &mon_th, &d_ay) == 3 &&
            ye_ar >= 1900 && ye_ar <= 9999 &&
            mon_th >= 1 && mon_th <= 12 &&
            d_ay >= 1 && d_ay <= 31);
}
