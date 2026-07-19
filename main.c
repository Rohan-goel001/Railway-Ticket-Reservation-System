#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TICKETS 100      // Maximum capacity of the system's memory
#define MAX_TRAINS 5         // Exactly 5 hardcoded trains
#define FILENAME "tickets.txt"

// Structure to store Train details
struct Train {
    int trainNumber;
    char trainName[50];
    char source[30];
    char destination[30];
    int totalSeats;
    int availableSeats;
};

// Structure to store Ticket/Passenger details
struct Ticket {
    int ticketNumber;        // Unique automatic ID (e.g. 1001, 1002...)
    char passengerName[50];
    int passengerAge;
    char passengerGender;    // 'M', 'F', or 'O'
    int trainNumber;         // Links to the selected train
    char travelDate[15];     // Date stored simply as a string (DD/MM/YYYY)
    int seatNumber;          // Assigned sequentially based on availability
    char status[15];         // "Confirmed" or "Cancelled"
};

// Global Arrays (Our "In-Memory Database")
struct Train trains[MAX_TRAINS];
struct Ticket tickets[MAX_TICKETS];

// Global Counters
int totalTicketsCount = 0;   // Number of tickets currently loaded in memory
int nextTicketNumber = 1001; // Next ID to assign for auto-incrementing tickets

// Function Prototypes (Declarations)
void initializeTrains(void);
int findTrainIndex(int trainNum);
int findTicketIndex(int ticketNum);
void displayTrains(char travelDate[]);
int getAvailableSeats(int trainNumber, char travelDate[]);
void bookTicket(void);
void searchTicket(void);
void displayAllTickets(void);
void updateTicket(void);
void cancelTicket(void);
void saveData(void);
void loadData(void);

// Main Function
int main(void) {
    int choice;

    // Phase 1 Setup: Initialize predefined trains and load saved data
    initializeTrains();
    loadData();
    displayTrains("");

    do {
        printf("\n=====================================================\n");
        printf("          RAILWAY TICKET RESERVATION SYSTEM\n");
        printf("=====================================================\n");
        printf("  1. Book Ticket\n");
        printf("  2. Search Ticket\n");
        printf("  3. Display All Tickets\n");
        printf("  4. Update Ticket\n");
        printf("  5. Cancel Ticket\n");
        printf("  6. Save Data\n");
        printf("  7. Load Data\n");
        printf("  8. Exit\n");
        printf("=====================================================\n");
        printf("Enter your choice (1-8): ");
        
        // Read choice
        if (scanf("%d", &choice) != 1) {
            // Clear input buffer on invalid input
            while (getchar() != '\n');
            printf("Invalid input! Please enter a number between 1 and 8.\n");
            continue;
        }
        // Consume leftover newline from choice input
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                bookTicket();
                break;
            case 2:
                searchTicket();
                break;
            case 3:
                displayAllTickets();
                break;
            case 4:
                updateTicket();
                break;
            case 5:
                cancelTicket();
                break;
            case 6:
                saveData();
                break;
            case 7:
                loadData();
                break;
            case 8:
                saveData();
                printf("Thank you for using Railway Ticket Reservation System!\n");
                break;
            default:
                printf("\nInvalid choice! Please choose between 1 and 8.\n");
        }
    } while (choice != 8);

    return 0;
}

// =========================================================================
// FUNCTION DEFINITIONS (STUBS FOR PHASE 1)
// =========================================================================

/*
 * Initializes 5 predefined trains with static data.
 * The user cannot add or remove trains.
 */
void initializeTrains(void) {
    // Train 1: Rajdhani Express
    trains[0].trainNumber = 12001;
    strcpy(trains[0].trainName, "Rajdhani Express");
    strcpy(trains[0].source, "New Delhi");
    strcpy(trains[0].destination, "Mumbai");
    trains[0].totalSeats = 100;
    trains[0].availableSeats = 100; // Initially, available seats equal total seats

    // Train 2: Shatabdi Express
    trains[1].trainNumber = 12002;
    strcpy(trains[1].trainName, "Shatabdi Express");
    strcpy(trains[1].source, "New Delhi");
    strcpy(trains[1].destination, "Chandigarh");
    trains[1].totalSeats = 80;
    trains[1].availableSeats = 80; // Initially, available seats equal total seats

    // Train 3: Vande Bharat Express
    trains[2].trainNumber = 22435;
    strcpy(trains[2].trainName, "Vande Bharat Express");
    strcpy(trains[2].source, "New Delhi");
    strcpy(trains[2].destination, "Varanasi");
    trains[2].totalSeats = 120;
    trains[2].availableSeats = 120; // Initially, available seats equal total seats

    // Train 4: Duronto Express
    trains[3].trainNumber = 12213;
    strcpy(trains[3].trainName, "Duronto Express");
    strcpy(trains[3].source, "New Delhi");
    strcpy(trains[3].destination, "Kolkata");
    trains[3].totalSeats = 90;
    trains[3].availableSeats = 90; // Initially, available seats equal total seats

    // Train 5: Garib Rath Express
    trains[4].trainNumber = 12909;
    strcpy(trains[4].trainName, "Garib Rath Express");
    strcpy(trains[4].source, "New Delhi");
    strcpy(trains[4].destination, "Jaipur");
    trains[4].totalSeats = 150;
    trains[4].availableSeats = 150; // Initially, available seats equal total seats

    printf("[System] Predefined trains initialized successfully.\n");
}

/*
 * Helper: Finds the index of a train in the global trains array using trainNumber.
 * Returns: Array index (0 to MAX_TRAINS-1) if found, or -1 if not found.
 */
int findTrainIndex(int trainNum) {
    int i;
    for (i = 0; i < MAX_TRAINS; i++) {
        if (trains[i].trainNumber == trainNum) {
            return i; // Return the matching index (0 to 4)
        }
    }
    return -1; // Return -1 if not found
}

/*
 * Helper: Calculates available seats for a train on a specific travel date.
 * Returns: Available seats (Total Seats - Booked Seats for that date).
 */
int getAvailableSeats(int trainNumber, char travelDate[]) {
    int i;
    int bookedSeats = 0;
    int trainIndex = findTrainIndex(trainNumber);

    if (trainIndex == -1) {
        return 0; // If train doesn't exist, availability is 0
    }

    // Loop through all tickets booked so far in memory
    for (i = 0; i < totalTicketsCount; i++) {
        // Match train number, travel date, and confirm the booking is active
        if (tickets[i].trainNumber == trainNumber && 
            strcmp(tickets[i].travelDate, travelDate) == 0 && 
            strcmp(tickets[i].status, "Confirmed") == 0) {
            bookedSeats++;
        }
    }

    return trains[trainIndex].totalSeats - bookedSeats;
}

/*
 * Helper: Finds the index of a ticket in the global tickets array using ticketNumber.
 * Returns: Array index (0 to totalTicketsCount-1) if found, or -1 if not found.
 */
int findTicketIndex(int ticketNum) {
    int i;
    for (i = 0; i < totalTicketsCount; i++) {
        if (tickets[i].ticketNumber == ticketNum) {
            return i; // Return matching array index
        }
    }
    return -1; // Return -1 if not found
}

/*
 * Displays all predefined trains in a neat table.
 */
void displayTrains(char travelDate[]) {
    int i;
    printf("\n========================================================================================================================\n");
    printf("%-11s%-30s%-19s%-20s%-16s%s\n", 
           "Train No", "Train Name", "Source", "Destination", "Total Seats", "Available Seats");
    printf("========================================================================================================================\n");
    for (i = 0; i < MAX_TRAINS; i++) {
        // Calculate date-wise available seats dynamically
        int availSeats = getAvailableSeats(trains[i].trainNumber, travelDate);
        printf("%-11d%-30s%-19s%-20s%-16d%d\n", 
               trains[i].trainNumber, 
               trains[i].trainName, 
               trains[i].source, 
               trains[i].destination, 
               trains[i].totalSeats, 
               availSeats);
    }
    printf("========================================================================================================================\n");
}

/*
 * Books a ticket and adds it to the database array.
 */
void bookTicket(void) {
    char travelDate[15];
    int trainNum;
    int trainIndex;
    size_t len;
    int availSeats;

    // 1. Check if the tickets database capacity has been reached
    if (totalTicketsCount >= MAX_TICKETS) {
        printf("\nSystem Error: Maximum booking capacity of %d tickets has been reached!\n", MAX_TICKETS);
        return;
    }

    // 2. Ask the user for the Travel Date first
    printf("Enter Travel Date (DD/MM/YYYY): ");
    fgets(travelDate, sizeof(travelDate), stdin);
    // Remove the trailing newline character
    len = strlen(travelDate);
    if (len > 0 && travelDate[len - 1] == '\n') {
        travelDate[len - 1] = '\0';
    }

    // 3. Display available trains for THAT specific date
    displayTrains(travelDate);

    // 4. Ask the user for the Train Number
    printf("Enter Train Number: ");
    if (scanf("%d", &trainNum) != 1) {
        // Clear input buffer on invalid input
        while (getchar() != '\n');
        printf("Invalid Input!\n");
        return;
    }
    // Consume the leftover newline from scanf buffer
    while (getchar() != '\n');

    // 5. Validate the Train Number
    trainIndex = findTrainIndex(trainNum);
    if (trainIndex == -1) {
        printf("Invalid Train Number!\n");
        return;
    }

    // 6. Check Seat Availability for that specific date dynamically
    availSeats = getAvailableSeats(trainNum, travelDate);
    if (availSeats <= 0) {
        printf("Sorry! No seats available.\n");
        return;
    }

    // 7. Gather passenger details
    // Read Passenger Name (allows spaces)
    printf("Enter Passenger Name: ");
    fgets(tickets[totalTicketsCount].passengerName, sizeof(tickets[totalTicketsCount].passengerName), stdin);
    // Remove the trailing newline character
    len = strlen(tickets[totalTicketsCount].passengerName);
    if (len > 0 && tickets[totalTicketsCount].passengerName[len - 1] == '\n') {
        tickets[totalTicketsCount].passengerName[len - 1] = '\0';
    }

    // Read Passenger Age
    printf("Enter Passenger Age: ");
    if (scanf("%d", &tickets[totalTicketsCount].passengerAge) != 1) {
        while (getchar() != '\n');
        printf("Invalid Age Input!\n");
        return;
    }
    // Consume leftover newline
    while (getchar() != '\n');

    // Read Passenger Gender ('M', 'F', or 'O')
    printf("Enter Passenger Gender (M/F/O): ");
    if (scanf("%c", &tickets[totalTicketsCount].passengerGender) != 1) {
        while (getchar() != '\n');
        printf("Invalid Gender Input!\n");
        return;
    }
    // Consume leftover newline
    while (getchar() != '\n');

    // 8. Store ticket information in the tickets array
    tickets[totalTicketsCount].ticketNumber = nextTicketNumber;
    tickets[totalTicketsCount].trainNumber = trainNum;
    strcpy(tickets[totalTicketsCount].travelDate, travelDate);
    
    // Assign seat number sequentially date-wise (e.g., first seat is 1, next is 2... for that date)
    tickets[totalTicketsCount].seatNumber = trains[trainIndex].totalSeats - availSeats + 1;
    strcpy(tickets[totalTicketsCount].status, "Confirmed");

    // 9. Increment the global ticket number and ticket count
    nextTicketNumber++;
    totalTicketsCount++;

    // 10. Display a booking confirmation
    printf("\n=====================================================\n");
    printf("          BOOKING SUCCESSFUL\n");
    printf("=====================================================\n\n");
    printf("Ticket Number : %d\n\n", tickets[totalTicketsCount - 1].ticketNumber);
    printf("Passenger     : %s\n\n", tickets[totalTicketsCount - 1].passengerName);
    printf("Train         : %s\n\n", trains[trainIndex].trainName);
    printf("Travel Date   : %s\n\n", tickets[totalTicketsCount - 1].travelDate);
    printf("Seat Number   : %d\n\n", tickets[totalTicketsCount - 1].seatNumber);
    printf("Remaining     : %d\n\n", getAvailableSeats(trainNum, travelDate));
    printf("=====================================================\n");
}

/*
 * Searches for a ticket by ticket number and displays details.
 */
void searchTicket(void) {
    int ticketNum;
    int ticketIndex;
    int trainIndex;

    printf("\nEnter Ticket Number to search: ");
    if (scanf("%d", &ticketNum) != 1) {
        // Clear input buffer on invalid input
        while (getchar() != '\n');
        printf("Invalid input!\n");
        return;
    }
    // Consume leftover newline
    while (getchar() != '\n');

    // Search the tickets array using helper function
    ticketIndex = findTicketIndex(ticketNum);
    if (ticketIndex == -1) {
        printf("Ticket not found!\n");
        return;
    }

    // Find the corresponding train name
    trainIndex = findTrainIndex(tickets[ticketIndex].trainNumber);

    printf("\n=====================================================\n");
    printf("            TICKET DETAILS\n");
    printf("=====================================================\n\n");
    printf("Ticket Number : %d\n\n", tickets[ticketIndex].ticketNumber);
    printf("Passenger     : %s\n\n", tickets[ticketIndex].passengerName);
    printf("Age           : %d\n\n", tickets[ticketIndex].passengerAge);
    printf("Gender        : %c\n\n", tickets[ticketIndex].passengerGender);
    printf("Train Number  : %d\n\n", tickets[ticketIndex].trainNumber);
    printf("Train Name    : %s\n\n", (trainIndex != -1) ? trains[trainIndex].trainName : "Unknown");
    printf("Travel Date   : %s\n\n", tickets[ticketIndex].travelDate);
    printf("Seat Number   : %d\n\n", tickets[ticketIndex].seatNumber);
    printf("Status        : %s\n\n", tickets[ticketIndex].status);
    printf("=====================================================\n");
}

/*
 * Displays details of all booked tickets.
 */
void displayAllTickets(void) {
    int i, j;
    struct Ticket tempTickets[MAX_TICKETS];

    if (totalTicketsCount == 0) {
        printf("No tickets booked yet.\n");
        return;
    }

    // Copy to temporary array to avoid modifying original tickets array
    for (i = 0; i < totalTicketsCount; i++) {
        tempTickets[i] = tickets[i];
    }

    // Simple Bubble Sort by: 1. Date, 2. Train Number, 3. Seat Number
    for (i = 0; i < totalTicketsCount - 1; i++) {
        for (j = 0; j < totalTicketsCount - i - 1; j++) {
            int swapNeeded = 0;
            int dateCmp = strcmp(tempTickets[j].travelDate, tempTickets[j + 1].travelDate);
            
            if (dateCmp > 0) {
                swapNeeded = 1;
            } else if (dateCmp == 0) {
                if (tempTickets[j].trainNumber > tempTickets[j + 1].trainNumber) {
                    swapNeeded = 1;
                } else if (tempTickets[j].trainNumber == tempTickets[j + 1].trainNumber) {
                    if (tempTickets[j].seatNumber > tempTickets[j + 1].seatNumber) {
                        swapNeeded = 1;
                    }
                }
            }

            if (swapNeeded) {
                struct Ticket temp = tempTickets[j];
                tempTickets[j] = tempTickets[j + 1];
                tempTickets[j + 1] = temp;
            }
        }
    }

    // Display sorted list in a clean table format
    printf("\n========================================================================================================================\n");
    printf("%-11s%-22s%-30s%-15s%-8s%-12s\n", "Ticket No", "Passenger Name", "Train Name", "Date", "Seat", "Status");
    printf("========================================================================================================================\n");
    for (i = 0; i < totalTicketsCount; i++) {
        int trainIndex = findTrainIndex(tempTickets[i].trainNumber);
        char *trainName = (trainIndex != -1) ? trains[trainIndex].trainName : "Unknown";
        printf("%-11d%-22s%-30s%-15s%-8d%-12s\n", 
               tempTickets[i].ticketNumber, 
               tempTickets[i].passengerName, 
               trainName, 
               tempTickets[i].travelDate, 
               tempTickets[i].seatNumber, 
               tempTickets[i].status);
    }
    printf("========================================================================================================================\n");
    printf("Total Tickets : %d\n", totalTicketsCount);
    printf("========================================================================================================================\n");
}

/*
 * Updates passenger name and age of a specific ticket.
 */
void updateTicket(void) {
    int ticketNum;
    int ticketIndex;
    int choice;
    size_t len;
    
    printf("\nEnter Ticket Number to update: ");
    if (scanf("%d", &ticketNum) != 1) {
        // Clear input buffer on invalid input
        while (getchar() != '\n');
        printf("Invalid input!\n");
        return;
    }
    // Consume leftover newline
    while (getchar() != '\n');

    // Check if ticket number exists
    ticketIndex = findTicketIndex(ticketNum);
    if (ticketIndex == -1) {
        printf("Ticket not found!\n");
        return;
    }

    // Display update submenu
    printf("\n=====================================================\n");
    printf("                 UPDATE TICKET MENU\n");
    printf("=====================================================\n");
    printf("  1. Update Passenger Name\n");
    printf("  2. Update Age\n");
    printf("  3. Update Gender\n");
    printf("  4. Update Travel Date\n");
    printf("  5. Update Train\n");
    printf("  6. Update Everything\n");
    printf("  7. Cancel\n");
    printf("=====================================================\n");
    printf("Enter choice (1-7): ");
    
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        printf("Invalid choice!\n");
        return;
    }
    while (getchar() != '\n'); // clear buffer

    if (choice == 7) {
        printf("Update cancelled.\n");
        return;
    }

    if (choice < 1 || choice > 6) {
        printf("Invalid choice!\n");
        return;
    }

    // Temporary variables to store details
    char newName[50];
    strcpy(newName, tickets[ticketIndex].passengerName);
    
    int newAge = tickets[ticketIndex].passengerAge;
    char newGender = tickets[ticketIndex].passengerGender;
    
    char newTravelDate[15];
    strcpy(newTravelDate, tickets[ticketIndex].travelDate);
    
    int newTrainNum = tickets[ticketIndex].trainNumber;

    int updateName = (choice == 1 || choice == 6);
    int updateAge = (choice == 2 || choice == 6);
    int updateGender = (choice == 3 || choice == 6);
    int updateDate = (choice == 4 || choice == 6);
    int updateTrain = (choice == 5 || choice == 6);

    // 1. Update Name
    if (updateName) {
        printf("Enter New Passenger Name: ");
        fgets(newName, sizeof(newName), stdin);
        len = strlen(newName);
        if (len > 0 && newName[len - 1] == '\n') {
            newName[len - 1] = '\0';
        }
        if (strlen(newName) == 0) {
            printf("Passenger name cannot be empty!\n");
            return;
        }
    }

    // 2. Update Age
    if (updateAge) {
        printf("Enter New Passenger Age (1-120): ");
        if (scanf("%d", &newAge) != 1) {
            while (getchar() != '\n');
            printf("Invalid Age Input!\n");
            return;
        }
        while (getchar() != '\n');
        if (newAge < 1 || newAge > 120) {
            printf("Age must be between 1 and 120!\n");
            return;
        }
    }

    // 3. Update Gender
    if (updateGender) {
        printf("Enter New Passenger Gender (M/F/O): ");
        if (scanf("%c", &newGender) != 1) {
            while (getchar() != '\n');
            printf("Invalid Gender Input!\n");
            return;
        }
        while (getchar() != '\n');
        if (newGender == 'm') newGender = 'M';
        if (newGender == 'f') newGender = 'F';
        if (newGender == 'o') newGender = 'O';
        if (newGender != 'M' && newGender != 'F' && newGender != 'O') {
            printf("Gender must be M, F, or O!\n");
            return;
        }
    }

    // 4. Update Travel Date
    if (updateDate) {
        printf("Enter New Travel Date (DD/MM/YYYY): ");
        fgets(newTravelDate, sizeof(newTravelDate), stdin);
        len = strlen(newTravelDate);
        if (len > 0 && newTravelDate[len - 1] == '\n') {
            newTravelDate[len - 1] = '\0';
        }
        if (strlen(newTravelDate) == 0) {
            printf("Travel date cannot be empty!\n");
            return;
        }
    }

    // 5. Update Train
    if (updateTrain) {
        printf("Enter New Train Number: ");
        if (scanf("%d", &newTrainNum) != 1) {
            while (getchar() != '\n');
            printf("Invalid Train Number Input!\n");
            return;
        }
        while (getchar() != '\n');
        if (findTrainIndex(newTrainNum) == -1) {
            printf("Invalid Train Number!\n");
            return;
        }
    }

    // If train number or travel date changes, recalculate seat number date-wise
    int oldTrainNum = tickets[ticketIndex].trainNumber;
    char oldTravelDate[15];
    strcpy(oldTravelDate, tickets[ticketIndex].travelDate);

    if (newTrainNum != oldTrainNum || strcmp(newTravelDate, oldTravelDate) != 0) {
        int newTrainIndex = findTrainIndex(newTrainNum);
        
        // Temporarily set status to "Cancelled" so getAvailableSeats doesn't count this ticket
        strcpy(tickets[ticketIndex].status, "Cancelled");
        int newAvail = getAvailableSeats(newTrainNum, newTravelDate);
        strcpy(tickets[ticketIndex].status, "Confirmed"); // restore original status

        if (newAvail <= 0) {
            printf("Sorry! No seats available on the selected train/date. Update cancelled.\n");
            return;
        }

        // Apply route updates and set new seat number
        tickets[ticketIndex].trainNumber = newTrainNum;
        strcpy(tickets[ticketIndex].travelDate, newTravelDate);
        tickets[ticketIndex].seatNumber = trains[newTrainIndex].totalSeats - newAvail + 1;
    }

    // Apply passenger details updates
    strcpy(tickets[ticketIndex].passengerName, newName);
    tickets[ticketIndex].passengerAge = newAge;
    tickets[ticketIndex].passengerGender = newGender;

    printf("Ticket Updated Successfully.\n");
}

/*
 * Cancels a ticket by changing its status to "Cancelled".
 */
void cancelTicket(void) {
    int ticketNum;
    int ticketIndex;
    char confirm;
    int i;

    printf("\nEnter Ticket Number to cancel: ");
    if (scanf("%d", &ticketNum) != 1) {
        // Clear input buffer on invalid input
        while (getchar() != '\n');
        printf("Invalid input!\n");
        return;
    }
    // Consume leftover newline
    while (getchar() != '\n');

    // Check if ticket exists
    ticketIndex = findTicketIndex(ticketNum);
    if (ticketIndex == -1) {
        printf("Ticket not found!\n");
        return;
    }

    printf("Are you sure? (Y/N): ");
    if (scanf("%c", &confirm) != 1) {
        while (getchar() != '\n');
        printf("Invalid input!\n");
        return;
    }
    while (getchar() != '\n'); // clear buffer

    if (confirm == 'y' || confirm == 'Y') {
        // Shift remaining records left
        for (i = ticketIndex; i < totalTicketsCount - 1; i++) {
            tickets[i] = tickets[i + 1];
        }
        totalTicketsCount--;
        printf("Ticket Cancelled Successfully.\n");
    } else {
        printf("Cancellation cancelled.\n");
    }
}

/*
 * Saves all tickets and counters from memory to tickets.txt.
 */
void saveData(void) {
    int i;
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Unable to save data.\n");
        return;
    }

    fprintf(file, "%d\n", totalTicketsCount);
    for (i = 0; i < totalTicketsCount; i++) {
        fprintf(file, "%d\n", tickets[i].ticketNumber);
        fprintf(file, "%s\n", tickets[i].passengerName);
        fprintf(file, "%d\n", tickets[i].passengerAge);
        fprintf(file, "%c\n", tickets[i].passengerGender);
        fprintf(file, "%d\n", tickets[i].trainNumber);
        fprintf(file, "%s\n", tickets[i].travelDate);
        fprintf(file, "%d\n", tickets[i].seatNumber);
        fprintf(file, "%s\n", tickets[i].status);
    }
    fclose(file);
    printf("Data saved successfully.\n");
}

void loadData(void) {
    int i;
    char buffer[100];
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("No saved data found.\n");
        return;
    }

    // Read totalTicketsCount
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        totalTicketsCount = 0;
        fclose(file);
        return;
    }
    totalTicketsCount = atoi(buffer);

    for (i = 0; i < totalTicketsCount; i++) {
        // Ticket Number
        if (fgets(buffer, sizeof(buffer), file) == NULL) break;
        tickets[i].ticketNumber = atoi(buffer);

        // Passenger Name
        if (fgets(tickets[i].passengerName, sizeof(tickets[i].passengerName), file) == NULL) break;
        size_t len = strlen(tickets[i].passengerName);
        if (len > 0 && tickets[i].passengerName[len - 1] == '\n') {
            tickets[i].passengerName[len - 1] = '\0';
        }

        // Age
        if (fgets(buffer, sizeof(buffer), file) == NULL) break;
        tickets[i].passengerAge = atoi(buffer);

        // Gender
        if (fgets(buffer, sizeof(buffer), file) == NULL) break;
        tickets[i].passengerGender = buffer[0];

        // Train Number
        if (fgets(buffer, sizeof(buffer), file) == NULL) break;
        tickets[i].trainNumber = atoi(buffer);

        // Travel Date
        if (fgets(tickets[i].travelDate, sizeof(tickets[i].travelDate), file) == NULL) break;
        len = strlen(tickets[i].travelDate);
        if (len > 0 && tickets[i].travelDate[len - 1] == '\n') {
            tickets[i].travelDate[len - 1] = '\0';
        }

        // Seat Number
        if (fgets(buffer, sizeof(buffer), file) == NULL) break;
        tickets[i].seatNumber = atoi(buffer);

        // Status
        if (fgets(tickets[i].status, sizeof(tickets[i].status), file) == NULL) break;
        len = strlen(tickets[i].status);
        if (len > 0 && tickets[i].status[len - 1] == '\n') {
            tickets[i].status[len - 1] = '\0';
        }
    }
    fclose(file);

    // Restore nextTicketNumber based on largest ticket number + 1
    int maxTicketNum = 1000;
    for (i = 0; i < totalTicketsCount; i++) {
        if (tickets[i].ticketNumber > maxTicketNum) {
            maxTicketNum = tickets[i].ticketNumber;
        }
    }
    nextTicketNumber = maxTicketNum + 1;

    printf("Data loaded successfully.\n");
}
