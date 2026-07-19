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
        printf("\n=============================================\n");
        printf("    RAILWAY TICKET RESERVATION SYSTEM\n");
        printf("=============================================\n");
        printf("1. Book Ticket\n");
        printf("2. Search Ticket\n");
        printf("3. Display All Tickets\n");
        printf("4. Update Ticket\n");
        printf("5. Cancel Ticket\n");
        printf("6. Save Data\n");
        printf("7. Load Data\n");
        printf("8. Exit\n");
        printf("---------------------------------------------\n");
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
                // Auto-save data before exiting
                saveData();
                printf("\nSaving data and exiting. Thank you for using the system!\n");
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
    printf("\n------------------------------------------------------------------------------------------\n");
    printf("%-8s | %-24s | %-11s | %-11s | %-11s | %s\n", 
           "Train No", "Train Name", "Source", "Destination", "Total Seats", "Available");
    printf("------------------------------------------------------------------------------------------\n");
    for (i = 0; i < MAX_TRAINS; i++) {
        // Calculate date-wise available seats dynamically
        int availSeats = getAvailableSeats(trains[i].trainNumber, travelDate);
        printf("%-8d | %-24s | %-11s | %-11s | %-11d | %d\n", 
               trains[i].trainNumber, 
               trains[i].trainName, 
               trains[i].source, 
               trains[i].destination, 
               trains[i].totalSeats, 
               availSeats);
    }
    printf("------------------------------------------------------------------------------------------\n");
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
    printf("\n----------------------------------------\n");
    printf("Booking Successful!\n");
    printf("Ticket Number : %d\n", tickets[totalTicketsCount - 1].ticketNumber);
    printf("Passenger Name: %s\n", tickets[totalTicketsCount - 1].passengerName);
    printf("Train         : %s\n", trains[trainIndex].trainName);
    printf("Travel Date   : %s\n", tickets[totalTicketsCount - 1].travelDate);
    printf("Seat Number   : %d\n", tickets[totalTicketsCount - 1].seatNumber);
    printf("Remaining Seats: %d\n", getAvailableSeats(trainNum, travelDate));
    printf("----------------------------------------\n");
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

    printf("\n----------------------------------------\n");
    printf("Ticket Number : %d\n", tickets[ticketIndex].ticketNumber);
    printf("Passenger Name: %s\n", tickets[ticketIndex].passengerName);
    printf("Age           : %d\n", tickets[ticketIndex].passengerAge);
    printf("Gender        : %c\n", tickets[ticketIndex].passengerGender);
    printf("Train Number  : %d\n", tickets[ticketIndex].trainNumber);
    printf("Train Name    : %s\n", (trainIndex != -1) ? trains[trainIndex].trainName : "Unknown");
    printf("Travel Date   : %s\n", tickets[ticketIndex].travelDate);
    printf("Seat Number   : %d\n", tickets[ticketIndex].seatNumber);
    printf("Status        : %s\n", tickets[ticketIndex].status);
    printf("----------------------------------------\n");
}

/*
 * Displays details of all booked tickets.
 */
void displayAllTickets(void) {
    // Stub implementation
    printf("\n--- All Tickets List (Stub: Feature Coming Soon) ---\n");
}

/*
 * Updates passenger name and age of a specific ticket.
 */
void updateTicket(void) {
    // Stub implementation
    printf("\n--- Update Ticket (Stub: Feature Coming Soon) ---\n");
}

/*
 * Cancels a ticket by changing its status to "Cancelled".
 */
void cancelTicket(void) {
    // Stub implementation
    printf("\n--- Cancel Ticket (Stub: Feature Coming Soon) ---\n");
}

/*
 * Saves all tickets and counters from memory to tickets.txt.
 */
void saveData(void) {
    // Stub implementation
    printf("[System] Ticket data saved to %s (Stub).\n", FILENAME);
}

/*
 * Loads tickets and counters from tickets.txt into memory.
 */
void loadData(void) {
    // Stub implementation
    printf("[System] Ticket data loaded from %s (Stub).\n", FILENAME);
}
