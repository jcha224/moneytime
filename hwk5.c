#include <stdio.h>
#include <string.h>

/**
 * Defines each item.
 */
struct item_tag {
    char label[30];             // name of item
    int number;                 // number of items
    float price;           // price of item
    float priceAll;         // price times the number
};
typedef struct item_tag Item;

/**
 * Defines each customer.
 */
struct customer_tag {
    char name[30];        // name of customer
	Item items[15];       // list of items
	int size;         // number of items
    float total;
};
typedef struct customer_tag Customer;

// global variable.
Customer myCustomers[20] = {0};    // list of customers
int mySize = 0;                     // number of customers

// prototypes
//int findItem(int theName, char *theItem);
void addItem(int theName, char *theItem, float thePrice, int theNum);
//void addName(char *theName);
int findName(char *theName);
void readFile(FILE* theFile);
void printTime(FILE* theFile);
void printMoney(FILE* theFile);
void sortItems();
void sortPurchase();

// main function.
int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    FILE *infile = fopen("hw5input.txt", "r");
    FILE *out1 = fopen("hw5Time.txt", "w+");
    FILE *out2 = fopen("hw5Money.txt", "w+");


    if(infile == NULL) {
        printf("File did not open.");
        return 1;
    }
    readFile(infile);
    printTime(out1);

    sortItems();
    sortPurchase();
    printMoney(out2);
    
    fclose(out1);
    fclose(out2);
    fclose(infile);
    return 0;
}

/**
 * Adds item to character.
 */
 void addItem(int theName, char *theItem, float thePrice, int theNum) {
     stpcpy(myCustomers[theName].items[myCustomers[theName].size].label, theItem);
     myCustomers[theName].items[myCustomers[theName].size].price = thePrice;
     myCustomers[theName].items[myCustomers[theName].size].number += theNum;
     myCustomers[theName].items[myCustomers[theName].size].priceAll = theNum * thePrice;
     myCustomers[theName].size++;
 }

/**
 * Finds and returns index of customer.
 */
int findName(char *theName) {
    int i = 0;
    int result = -1;
    while(i != mySize) {
        if(strcmp(theName, myCustomers[i].name) == 0) {
            result = i;
        }
    ++i;
    }
    if(result == -1) {
        strcpy(myCustomers[i].name, theName);
        mySize++;
        result = i;
    }
    return result;
}

/**
 * Reads information from file.
 */
void readFile(FILE* theFile) {
    char tempName[20];
    int cIdx = 0;
    int tempNum = 0;
    char tempItem[20];
    float tempPrice = 0.0;
    while(fscanf(theFile, "%s %d %s %*c%f", tempName, &tempNum, 
            tempItem, &tempPrice) != EOF) {
        
        cIdx = findName(tempName);
        addItem(cIdx, tempItem, tempPrice, tempNum);        
    }
    
}

/**
 * Creates the file named hw5Time.txt.
 */
void printTime(FILE* theFile) {
    int i = 0;
    int j = 0;
    char* var = "Total:";
    while(i != mySize) {
        float total = 0;
        fprintf(theFile, "Customer: %s \n\r        Orders:\n\r", myCustomers[i].name);
        while(j != myCustomers[i].size) {
            float total2 = myCustomers[i].items[j].priceAll;
            fprintf(theFile, "                %-10.10s%12d %10.2f %10.2f \n\r", 
            myCustomers[i].items[j].label, myCustomers[i].items[j].number,
            myCustomers[i].items[j].price, total2);
            ++j;
            total += total2;
        }
        myCustomers[i].total = total;
        fprintf(theFile,"%51s%9.2f\n\r",var,total);
        ++i;
        total = 0;
        j = 0;
    }

}

/**
 * Creates the file named hw5Money.txt.
 */
void printMoney(FILE* theFile) {
    int i = 0;
    int j = 0;
    char* var = "Total:";
    while(i != mySize) {
        fprintf(theFile, "%s, Total Order = $%.2f\n\r", myCustomers[i].name, myCustomers[i].total);
        while(j != myCustomers[i].size) {
            fprintf(theFile, "%s %d $%.2f, Item Value = $%.2f\n\r", 
            myCustomers[i].items[j].label, myCustomers[i].items[j].number,
            myCustomers[i].items[j].price, myCustomers[i].items[j].priceAll);
            ++j;
        }
        ++i;
        j = 0;
    }
}

/**
 * Sorts the Items for each customer based on total cost.
 */
void sortItems() {
    Item temp;
    for(int i = 0; i < mySize; i++) {
        int topBubble, bubbles, notSorted = 1;
        for (topBubble = 0; topBubble < myCustomers[i].size - 1 && notSorted; topBubble++) {
            for (bubbles = myCustomers[i].size - 1, notSorted = 0; bubbles > topBubble; bubbles--) {
                if (myCustomers[i].items[bubbles].priceAll > myCustomers[i].items[bubbles - 1].priceAll) {
                    temp = myCustomers[i].items[bubbles];
                    myCustomers[i].items[bubbles] = myCustomers[i].items[bubbles - 1];
                    myCustomers[i].items[bubbles - 1] = temp;
                    notSorted = 1;
                }
            }
        }
    }
}

/**
 * Sorts the customers based on total spent.
 */
void sortPurchase() {
    Customer temp;
    int topBubble, bubbles, notSorted = 1;
    for (topBubble = 0; topBubble < mySize - 1 && notSorted; topBubble++) {
        for (bubbles = mySize - 1, notSorted = 0; bubbles > topBubble; bubbles--) {
            if (myCustomers[bubbles].total > myCustomers[bubbles - 1].total) {
                temp = myCustomers[bubbles];
                myCustomers[bubbles] = myCustomers[bubbles - 1];
                myCustomers[bubbles - 1] = temp;
                notSorted = 1;
            }
        }
    }
}