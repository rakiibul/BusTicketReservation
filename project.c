#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_USERS 100
#define MAX_BUSES 50
#define MAX_TICKETS 100
#define MAX_NAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 20
#define MAX_EMAIL_LENGTH 100
#define MAX_BUS_NAME 50
#define MAX_DEST_LENGTH 100
#define USER_DATA_FILE "users.dat"

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    int is_admin;
} User;

typedef struct {
    int bus_id;
    char bus_name[MAX_BUS_NAME];
    char departure_time[20];
    char arrival_time[20];
    char origin[MAX_DEST_LENGTH];
    char destination[MAX_DEST_LENGTH];
    float price;
    int seats_available;
} Bus;

typedef struct {
    int ticket_id;
    int user_id;
    int bus_id;
    int is_canceled;
    float amount_paid;
} Ticket;

// Declare global variables
User users[MAX_USERS];
Bus buses[MAX_BUSES];
Ticket tickets[MAX_TICKETS];
int user_count = 0;
int bus_count = 0;
int ticket_count = 0;

// Function to load user data from file
void load_users_from_file() {
    FILE *file = fopen(USER_DATA_FILE, "rb");
    if (file == NULL) {
        printf("No previous user data found. Starting fresh.\n");
        return;
    }

    fread(&user_count, sizeof(int), 1, file);
    fread(users, sizeof(User), user_count, file);

    fclose(file);
    printf("User data loaded successfully.\n");
}

// Function to save user data to file
void save_users_to_file() {
    FILE *file = fopen(USER_DATA_FILE, "wb");
    if (file == NULL) {
        printf("Error: Could not save user data.\n");
        return;
    }

    fwrite(&user_count, sizeof(int), 1, file);
    fwrite(users, sizeof(User), user_count, file);

    fclose(file);
    printf("User data saved successfully.\n");
}

void load_bus_schedules_from_file() {
    FILE *file = fopen("bus_schedule.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open bus_schedule.txt\n");
        return;
    }

    while (fscanf(file, "%d %49s %19s %19s %99s %99s %f %d",
                  &buses[bus_count].bus_id,
                  buses[bus_count].bus_name,
                  buses[bus_count].departure_time,
                  buses[bus_count].arrival_time,
                  buses[bus_count].origin,
                  buses[bus_count].destination,
                  &buses[bus_count].price,
                  &buses[bus_count].seats_available) == 8) {
        bus_count++;
    }

    fclose(file);
    printf("Bus schedules loaded successfully from file.\n");
}

void view_bus_schedule() {
    printf("\n \n");
    if (bus_count == 0) {
        printf("No bus schedules available.\n");
        return;
    }
    printf("Bus Schedule:\n");
    for (int i = 0; i < bus_count; i++) {
        printf("Bus ID: %d\n", buses[i].bus_id);
        printf("Bus Name: %s\n", buses[i].bus_name);
        printf("Departure Time: %s\n", buses[i].departure_time);
        printf("Arrival Time: %s\n", buses[i].arrival_time);
        printf("Origin: %s\n", buses[i].origin);
        printf("Destination: %s\n", buses[i].destination);
        printf("Price: %.2f\n", buses[i].price);
        printf("Seats Available: %d\n", buses[i].seats_available);
        printf("-----------------------------\n");
    }
}

void add_bus_schedule() {
    if (bus_count >= MAX_BUSES) {
        printf("Error: Cannot add more buses. Maximum limit reached.\n");
        return;
    }

    printf("Enter Bus ID: ");
    scanf("%d", &buses[bus_count].bus_id);

    printf("Enter Bus Name: ");
    scanf("%s", buses[bus_count].bus_name);

    printf("Enter Departure Time (e.g., 10:00 AM): ");
    scanf("%s", buses[bus_count].departure_time);

    printf("Enter Arrival Time (e.g., 12:00 PM): ");
    scanf("%s", buses[bus_count].arrival_time);

    printf("Enter Origin: ");
    scanf("%s", buses[bus_count].origin);

    printf("Enter Destination: ");
    scanf("%s", buses[bus_count].destination);

    printf("Enter Ticket Price: ");
    scanf("%f", &buses[bus_count].price);

    printf("Enter Number of Seats Available: ");
    scanf("%d", &buses[bus_count].seats_available);

    bus_count++;
    printf("Bus schedule added successfully!\n");

    save_bus_schedules_to_file();  // Save changes to the file
}

void update_bus_schedule() {
    int bus_id, found = 0;

    printf("Enter the Bus ID to update: ");
    scanf("%d", &bus_id);

    for (int i = 0; i < bus_count; i++) {
        if (buses[i].bus_id == bus_id) {
            found = 1;

            printf("Enter New Bus Name: ");
            scanf("%s", buses[i].bus_name);

            printf("Enter New Departure Time: ");
            scanf("%s", buses[i].departure_time);

            printf("Enter New Arrival Time: ");
            scanf("%s", buses[i].arrival_time);

            printf("Enter New Origin: ");
            scanf("%s", buses[i].origin);

            printf("Enter New Destination: ");
            scanf("%s", buses[i].destination);

            printf("Enter New Ticket Price: ");
            scanf("%f", &buses[i].price);

            printf("Enter New Number of Seats Available: ");
            scanf("%d", &buses[i].seats_available);

            printf("Bus schedule updated successfully!\n");

            save_bus_schedules_to_file();  // Save changes to the file
            return;
        }
    }

    if (!found) {
        printf("Error: Bus ID not found.\n");
    }
}

void delete_bus_schedule() {
    int bus_id, found = 0;

    printf("Enter the Bus ID to delete: ");
    scanf("%d", &bus_id);

    for (int i = 0; i < bus_count; i++) {
        if (buses[i].bus_id == bus_id) {
            found = 1;

            for (int j = i; j < bus_count - 1; j++) {
                buses[j] = buses[j + 1];
            }
            bus_count--;

            printf("Bus schedule deleted successfully!\n");

            save_bus_schedules_to_file();  // Save changes to the file
            return;
        }
    }

    if (!found) {
        printf("Error: Bus ID not found.\n");
    }
}

void save_bus_schedules_to_file() {
    FILE *file = fopen("bus_schedule.txt", "w");
    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    for (int i = 0; i < bus_count; i++) {
        fprintf(file, "%d %s %s %s %s %s %.2f %d\n",
                buses[i].bus_id,
                buses[i].bus_name,
                buses[i].departure_time,
                buses[i].arrival_time,
                buses[i].origin,
                buses[i].destination,
                buses[i].price,
                buses[i].seats_available);
    }

    fclose(file);
    printf("Bus schedules saved to file successfully.\n");
}


void view_all_users() {
    if (user_count == 0) {
        printf("No users found.\n");
        return;
    }

    printf("All Registered Users:\n");
    printf("---------------------------------------------------\n");
    printf("ID\tName\t\tEmail\t\t\tRole\n");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < user_count; i++) {
        printf("%d\t%s\t\t%s\t\t%s\n",
               users[i].id,
               users[i].name,
               users[i].email,
               users[i].is_admin == 0 ? "Customer" : (users[i].is_admin == 1 ? "Admin" : "Operator"));
    }
    printf("---------------------------------------------------\n");
}

void admin_delete_user() {
    int user_id;

    printf("Enter the User ID to delete: ");
    scanf("%d", &user_id);

    if (user_id < 0 || user_id >= user_count) {
        printf("Invalid User ID.\n");
        return;
    }

    // Shift users to overwrite the deleted user
    for (int i = user_id; i < user_count - 1; i++) {
        users[i] = users[i + 1];
    }
    user_count--;

    printf("User deleted successfully.\n");
    save_users_to_file();
}

void admin_update_user() {
    int user_id;
    char new_name[MAX_NAME_LENGTH], new_email[MAX_EMAIL_LENGTH];
    int new_role;

    printf("Enter the User ID to update: ");
    scanf("%d", &user_id);

    if (user_id < 0 || user_id >= user_count) {
        printf("Invalid User ID.\n");
        return;
    }

    printf("Enter New Name: ");
    scanf("%s", new_name);

    printf("Enter New Email: ");
    scanf("%s", new_email);

    while (!is_valid_email(new_email)) {
        printf("Enter a valid email: ");
        scanf("%s", new_email);
    }

    printf("Enter New Role (0 for Customer, 1 for Admin, 2 for Operator): ");
    scanf("%d", &new_role);

    strcpy(users[user_id].name, new_name);
    strcpy(users[user_id].email, new_email);
    users[user_id].is_admin = new_role;

    printf("User information updated successfully.\n");
    save_users_to_file();
}


void view_all_bookings() {
    printf("\n\n");
    if (ticket_count == 0) {
        printf("No tickets have been booked yet.\n");
        return;
    }

    printf("List of All Bookings:\n");
    printf("Ticket ID\tUser ID\tBus ID\tAmount Paid\tStatus\n");
    printf("--------------------------------------------------------\n");
    for (int i = 0; i < ticket_count; i++) {
        printf("%d\t\t%d\t%d\t%.2f\t\t%s\n",
               tickets[i].ticket_id,
               tickets[i].user_id,
               tickets[i].bus_id,
               tickets[i].amount_paid,
               tickets[i].is_canceled ? "Canceled" : "Active");
    }
    printf("--------------------------------------------------------\n");
}


int is_strong_password(char *pass) {
    int len = strlen(pass);
    int upper = 0, lower = 0, digit = 0, special = 0;

    if (len < 8) {
        printf("Password must be at least 8 characters long.\n");
        return 0;
    }

    for (int i = 0; i < len; i++) {
        if (pass[i] == ' ') {
            printf("Password contains spaces. Please try again without spaces.\n");
            return 0;
        }
        if (isupper(pass[i])) upper = 1;
        else if (islower(pass[i])) lower = 1;
        else if (isdigit(pass[i])) digit = 1;
        else if (ispunct(pass[i])) special = 1;
    }

    if (upper && lower && digit && special) {
        printf("Password is strong.\n");
        return 1;
    } else {
        printf("Password is not strong enough. It should have:\n");
        printf("- At least one uppercase character.\n");
        printf("- At least one lowercase character.\n");
        printf("- At least one digit.\n");
        printf("- At least one special character.\n");
        return 0;
    }
}

int is_valid_email(const char *email) {
    int len = strlen(email);
    if (len < 12) {
        printf("Invalid email: Too short.\n");
        return 0;
    }

    if (strncmp(&email[len - 11], "@diu.edu.bd", 11) != 0) {
        printf("Invalid email: Must end with @diu.edu.bd.\n");
        return 0;
    }

    if (!isalpha(email[0])) {
        printf("Invalid email: First character must be a letter.\n");
        return 0;
    }

    return 1;
}

void display_main_menu() {
    printf("\n \n");
    printf("Welcome to the Bus Ticket Reservation System\n");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("3. Exit\n");
    printf("Enter you choice: ");
}

void display_customer_menu() {
    printf("\n \n");
    printf("Customer Menu:\n");
    printf("1. View Bus Schedule\n");
    printf("2. Book Ticket\n");
    printf("3. Cancel Ticket\n");
    printf("4. View Booking History\n");
    printf("5. View Information\n");
    printf("6. Update Information\n");
    printf("7. Delete Information\n");
    printf("8. Logout\n");
}

void display_admin_menu() {
    printf("\n\n");
    printf("Admin Menu:\n");
    printf("1. View All Users\n");
    printf("2. Delete a User\n");
    printf("3. Update User Information\n");
    printf("4. Manage Bus Schedules\n");
    printf("5. Logout\n");
}

void admin_panel() {
    int choice;
    while (1) {
        display_admin_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                view_all_users();
                break;
            case 2:
                admin_delete_user();
                break;
            case 3:
                admin_update_user();
                break;
            case 4:
                while (1) {
                    printf("Manage Bus Schedules:\n");
                    printf("1. Add Bus Schedule\n");
                    printf("2. Update Bus Schedule\n");
                    printf("3. Delete Bus Schedule\n");
                    printf("4. Back to Admin Menu\n");
                    scanf("%d", &choice);

                    if (choice == 1) add_bus_schedule();
                    else if (choice == 2) update_bus_schedule();
                    else if (choice == 3) delete_bus_schedule();
                    else if (choice == 4) break;
                    else printf("Invalid choice. Try again.\n");
                }
                break;
            case 5:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}


void view_user_information(int user_id) {
    printf("\n \n");
    printf("User Information:\n");
    printf("User ID: %d\n", user_id);
    printf("Name: %s\n", users[user_id].name);
    printf("Email: %s\n", users[user_id].email);
    printf("Role: %s\n", users[user_id].is_admin == 0 ? "Customer" : (users[user_id].is_admin == 1 ? "Admin" : "Operator"));
}


int register_user() {
    char name[MAX_NAME_LENGTH], password[MAX_PASSWORD_LENGTH], email[MAX_EMAIL_LENGTH];
    int is_admin;

    printf("\n");
    printf("Enter your name: ");
    scanf("%s", name);

    getchar();

    printf("Enter your password: ");
    fgets(password, MAX_PASSWORD_LENGTH, stdin);
    password[strcspn(password, "\n")] = 0;

    while (!is_strong_password(password)) {
        printf("Enter your password again: ");
        fgets(password, MAX_PASSWORD_LENGTH, stdin);
        password[strcspn(password, "\n")] = 0;
    }

    printf("Enter your email: ");
    scanf("%s", email);

    while (!is_valid_email(email)) {
        printf("Enter a valid email: ");
        scanf("%s", email);
    }

    printf("Enter 0 for Customer\n");
    printf("Enter 1 for Admin\n");
    printf("Enter 2 for Operator\n");
    printf("Enter your choice: ");
    scanf("%d", &is_admin);

    users[user_count].id = user_count;
    strcpy(users[user_count].name, name);
    strcpy(users[user_count].password, password);
    strcpy(users[user_count].email, email);
    users[user_count].is_admin = is_admin;
    user_count++;

    printf("User registered successfully!\n");

    // Save user data
    save_users_to_file();

    return user_count - 1;
}

int login_user() {
    char name[MAX_NAME_LENGTH], password[MAX_PASSWORD_LENGTH];
    printf("\n\n");
    printf("Enter your username: ");
    scanf("%s", name);

    // Clear the input buffer
    getchar();

    printf("Enter your password: ");
    fgets(password, MAX_PASSWORD_LENGTH, stdin);
    password[strcspn(password, "\n")] = 0;  // Remove newline from the password`

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].name, name) == 0 && strcmp(users[i].password, password) == 0) {
            printf("Login successful!\n");
            return i;
        }
    }

    printf("Invalid username or password.\n");
    return -1;
}

void book_ticket(int user_id) {
    int bus_id, num_tickets;
    printf("\n \n");
    printf("Enter the bus ID to book tickets: ");
    scanf("%d", &bus_id);

    // Check for valid bus ID
    if (bus_id < 0 || bus_id >= bus_count) {
        printf("Invalid bus ID.\n");
        return;
    }

    // Display available seats
    printf("Seats available on bus ID %d: %d\n", bus_id, buses[bus_id].seats_available);

    if (buses[bus_id].seats_available <= 0) {
        printf("Sorry, no seats are available for this bus.\n");
        return;
    }

    // Input number of tickets to book
    printf("Enter the number of tickets to book: ");
    scanf("%d", &num_tickets);

    // Check seat availability
    if (num_tickets > buses[bus_id].seats_available) {
        printf("Sorry, only %d seats are available.\n", buses[bus_id].seats_available);
        return;
    }

    // Book tickets
    for (int i = 0; i < num_tickets; i++) {
        tickets[ticket_count].ticket_id = ticket_count;
        tickets[ticket_count].user_id = user_id;
        tickets[ticket_count].bus_id = bus_id;
        tickets[ticket_count].is_canceled = 0;
        tickets[ticket_count].amount_paid = buses[bus_id].price;

        ticket_count++;
        buses[bus_id].seats_available--;

        printf("Ticket ID %d booked successfully! Amount Paid: %.2f\n", tickets[ticket_count - 1].ticket_id, buses[bus_id].price);
    }

    printf("%d ticket(s) booked successfully! Remaining seats on bus ID %d: %d\n",
           num_tickets, bus_id, buses[bus_id].seats_available);
}

void cancel_ticket(int user_id) {
    int ticket_id;
    printf("\n\n");
    printf("Enter ticket ID to cancel: ");
    scanf("%d", &ticket_id);

    if (ticket_id < 0 || ticket_id >= ticket_count) {
        printf("Invalid ticket ID.\n");
        return;
    }

    if (tickets[ticket_id].user_id != user_id) {
        printf("You can only cancel your own tickets.\n");
        return;
    }

    if (tickets[ticket_id].is_canceled) {
        printf("Ticket is already canceled.\n");
        return;
    }

    tickets[ticket_id].is_canceled = 1;
    buses[tickets[ticket_id].bus_id].seats_available++;
    printf("Ticket canceled successfully.\n");
}

void view_booking_history(int user_id) {
    printf("\n\n");
    printf("Booking history for user ID %d:\n", user_id);
    for (int i = 0; i < ticket_count; i++) {
        if (tickets[i].user_id == user_id && !tickets[i].is_canceled) {
            printf("Ticket ID: %d, Bus ID: %d, Amount Paid: %.2f\n", tickets[i].ticket_id, tickets[i].bus_id, tickets[i].amount_paid);
        }
    }
}

int update_user_info(int user_id) {
    printf("\n\n");
    printf("Enter new email: ");
    scanf("%s", users[user_id].email);
    printf("User information updated successfully!\n");
    return 0;
}

void delete_user_info(int user_id) {
    for (int i = user_id; i < user_count - 1; i++) {
        users[i] = users[i + 1];
    }
    user_count--;
    printf("User information deleted successfully!\n");
}

 int main() {
    int choice, user_id;
    int role;

    load_users_from_file();
    load_bus_schedules_from_file();

    while (1) {
        display_main_menu();
        scanf("%d", &choice);

        if (choice == 1) {
            register_user();
        } else if (choice == 2) {
            user_id = login_user();
            if (user_id != -1) {
                role = users[user_id].is_admin;  // Get user role (Admin, Operator, Customer)

                if (role == 0) {  // Customer role
                    while (1) {
                        display_customer_menu();
                        scanf("%d", &choice);
                        if (choice == 1) view_bus_schedule();
                        else if (choice == 2) book_ticket(user_id);
                        else if (choice == 3) cancel_ticket(user_id);
                        else if (choice == 4) view_booking_history(user_id);
                        else if (choice == 5) view_user_information(user_id);
                        else if (choice == 6) update_user_info(user_id);
                        else if (choice == 7) delete_user_info(user_id);
                        else if (choice == 8) break;  // Log out
                    }
                }
                else if (role == 1) {  // Admin role
                    admin_panel();
            }

                else if (role == 2) {  // Operator role
                while (1) {
                    printf("\nOperator Panel:\n");
                    printf("1. View All Bookings\n");
                    printf("2. View Bus Schedule\n");
                    printf("3. Add Bus Schedule\n");
                    printf("4. Update Bus Schedule\n");
                    printf("5. Delete Bus Schedule\n");
                    printf("6. Log Out\n");
                    printf("Enter your choice: ");
                    scanf("%d", &choice);

                    if (choice == 1) {
                        view_all_bookings();
                    } else if (choice == 2) {
                        view_bus_schedule();
                    } else if (choice == 3) {
                        add_bus_schedule();
                    } else if (choice == 4) {
                        update_bus_schedule();
                    } else if (choice == 5) {
                        delete_bus_schedule();
                    } else if (choice == 6) {
                        break;
                    } else {
                        printf("Invalid choice. Please try again.\n");
                    }
                }
            }

            }
        } else if (choice == 3) {
            printf("Exiting program.\n");
            break;
        }
    }

    return 0;
}

