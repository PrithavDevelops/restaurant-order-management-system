#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// File paths
#define MENU_FILE "menu.txt"
#define ORDER_FILE "orders.txt"
#define TEMP_FILE "temp.txt"

// Structure definitions
struct MenuItem {
  int item_id;
  char item_name[50];
  char category[30];
  float price;
  int available; // 1 = available, 0 = not available
};

struct OrderItem {
  int item_id;
  char item_name[50];
  float price;
  int quantity;
  float subtotal;
};

struct Order {
  int order_id;
  char customer_name[50];
  char date[40];
  struct OrderItem items[20];
  int item_count;
  float total_amount;
  char status[20]; // Pending, Completed, Cancelled
};

// Function prototypes
void mainMenu();
int adminLogin();
void adminMenu();
void customerMenu();

// Admin functions
void addMenuItem();
void viewMenu();
void updateMenuItem();
void deleteMenuItem();
void viewAllOrders();
void viewOrdersByStatus();
void updateOrderStatus();
void generateSalesReport();

// Customer functions
void placeOrder();
void viewCustomerMenu();
void viewMenuByCategory();

// Utility functions
void calculateBill(struct Order *order);
void displayOrder(struct Order order);
void pressEnterToContinue();
void clearScreen();
int generateOrderID();
void initializeMenuFile();

// Global variables
struct Order currentOrder;

int main() {
  int choice;

  // Initialize menu file if it doesn't exist
  initializeMenuFile();

  while (1) {
    clearScreen();
    mainMenu();

    printf("\nEnter your choice: ");
    if (scanf("%d", &choice) != 1) {
      while (getchar() != '\n')
        ;
      printf("\nInvalid input! Please enter a number.\n");
      pressEnterToContinue();
      continue;
    }
    while (getchar() != '\n')
      ;

    switch (choice) {
    case 1:
      if (adminLogin()) {
        adminMenu();
      }
      break;
    case 2:
      customerMenu();
      break;
    case 3:
      clearScreen();
      printf("\n========================================\n");
      printf("   Thank you for using our system!\n");
      printf("========================================\n\n");
      exit(0);
    default:
      printf("\nInvalid choice! Please try again.\n");
      pressEnterToContinue();
    }
  }

  return 0;
}

// ==================== MAIN MENU ====================
void mainMenu() {
  printf("\n========================================\n");
  printf("  RESTAURANT ORDER MANAGEMENT SYSTEM\n");
  printf("========================================\n");
  printf("\n         MAIN MENU\n");
  printf("----------------------------------------\n");
  printf("  1. Admin Login\n");
  printf("  2. Customer Mode\n");
  printf("  3. Exit\n");
  printf("----------------------------------------\n");
}

// ==================== ADMIN LOGIN ====================
int adminLogin() {
  char username[30], password[30];
  int attempts = 0;

  clearScreen();
  printf("\n========================================\n");
  printf("           ADMIN LOGIN\n");
  printf("========================================\n");

  while (attempts < 3) {
    printf("\nEnter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", password);
    while (getchar() != '\n')
      ;

    if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
      printf("\n✓ Login Successful!\n");
      printf("Welcome, Administrator!\n");
      pressEnterToContinue();
      return 1;
    } else {
      attempts++;
      printf("\n✗ Invalid credentials! ");
      if (attempts < 3) {
        printf("Attempts remaining: %d\n", 3 - attempts);
      }
    }
  }

  printf("\n✗ Too many failed attempts! Returning to main menu.\n");
  pressEnterToContinue();
  return 0;
}

// ==================== ADMIN MENU ====================
void adminMenu() {
  int choice;

  while (1) {
    clearScreen();
    printf("\n========================================\n");
    printf("           ADMIN PANEL\n");
    printf("========================================\n");
    printf("\n  1. View Menu\n");
    printf("  2. Add Menu Item\n");
    printf("  3. Update Menu Item\n");
    printf("  4. Delete Menu Item\n");
    printf("  5. View All Orders\n");
    printf("  6. View Orders by Status\n");
    printf("  7. Update Order Status\n");
    printf("  8. Generate Sales Report\n");
    printf("  9. Logout\n");
    printf("----------------------------------------\n");
    printf("\nEnter your choice: ");

    if (scanf("%d", &choice) != 1) {
      while (getchar() != '\n')
        ;
      printf("\nInvalid input!\n");
      pressEnterToContinue();
      continue;
    }
    while (getchar() != '\n')
      ;

    switch (choice) {
    case 1:
      viewMenu();
      pressEnterToContinue();
      break;
    case 2:
      addMenuItem();
      break;
    case 3:
      updateMenuItem();
      break;
    case 4:
      deleteMenuItem();
      break;
    case 5:
      viewAllOrders();
      pressEnterToContinue();
      break;
    case 6:
      viewOrdersByStatus();
      pressEnterToContinue();
      break;
    case 7:
      updateOrderStatus();
      break;
    case 8:
      generateSalesReport();
      pressEnterToContinue();
      break;
    case 9:
      printf("\n✓ Logged out successfully!\n");
      pressEnterToContinue();
      return;
    default:
      printf("\nInvalid choice!\n");
      pressEnterToContinue();
    }
  }
}

// ==================== CUSTOMER MENU ====================
void customerMenu() {
  int choice;

  while (1) {
    clearScreen();
    printf("\n========================================\n");
    printf("         CUSTOMER MODE\n");
    printf("========================================\n");
    printf("\n  1. View Full Menu\n");
    printf("  2. View Menu by Category\n");
    printf("  3. Place Order\n");
    printf("  4. Return to Main Menu\n");
    printf("----------------------------------------\n");
    printf("\nEnter your choice: ");

    if (scanf("%d", &choice) != 1) {
      while (getchar() != '\n')
        ;
      printf("\nInvalid input!\n");
      pressEnterToContinue();
      continue;
    }
    while (getchar() != '\n')
      ;

    switch (choice) {
    case 1:
      viewCustomerMenu();
      pressEnterToContinue();
      break;
    case 2:
      viewMenuByCategory();
      pressEnterToContinue();
      break;
    case 3:
      placeOrder();
      break;
    case 4:
      return;
    default:
      printf("\nInvalid choice!\n");
      pressEnterToContinue();
    }
  }
}

// ==================== ADD MENU ITEM ====================
void addMenuItem() {
  clearScreen();

  // First display current menu
  printf("\n========================================\n");
  printf("        CURRENT MENU\n");
  printf("========================================\n");

  FILE *fp_read = fopen(MENU_FILE, "r");
  if (fp_read) {
    struct MenuItem item;
    int count = 0;

    printf("\n%-6s %-30s %-20s %-12s %-10s\n", "ID", "Item Name", "Category",
           "Price", "Status");
    printf("-------------------------------------------------------------------"
           "-------------\n");

    while (fscanf(fp_read, "%d|%49[^|]|%29[^|]|%f|%d\n", &item.item_id,
                  item.item_name, item.category, &item.price,
                  &item.available) != EOF) {
      printf("%-6d %-30s %-20s Rs. %-8.2f %-10s\n", item.item_id,
             item.item_name, item.category, item.price,
             item.available ? "Available" : "Not Available");
      count++;
    }

    printf("-------------------------------------------------------------------"
           "-------------\n");
    printf("Total Items: %d\n\n", count);
    fclose(fp_read);
  } else {
    printf("\nNo existing menu items.\n\n");
  }

  // Now add new item
  FILE *fp = fopen(MENU_FILE, "a");
  if (!fp) {
    printf("\n✗ Error opening menu file!\n");
    pressEnterToContinue();
    return;
  }

  struct MenuItem item;

  printf("========================================\n");
  printf("        ADD NEW MENU ITEM\n");
  printf("========================================\n");

  int id_exists;
  do {
    id_exists = 0;
    printf("\nEnter Item ID: ");
    scanf("%d", &item.item_id);
    while (getchar() != '\n')
      ;

    // Check if ID exists
    FILE *check_fp = fopen(MENU_FILE, "r");
    if (check_fp) {
      struct MenuItem temp_item;
      while (fscanf(check_fp, "%d|%49[^|]|%29[^|]|%f|%d\n", &temp_item.item_id,
                    temp_item.item_name, temp_item.category, &temp_item.price,
                    &temp_item.available) != EOF) {
        if (temp_item.item_id == item.item_id) {
          id_exists = 1;
          printf(
              "Error: Item ID %d already exists! Please enter a unique ID.\n",
              item.item_id);
          break;
        }
      }
      fclose(check_fp);
    }
  } while (id_exists);

  printf("Enter Item Name: ");
  fgets(item.item_name, sizeof(item.item_name), stdin);
  item.item_name[strcspn(item.item_name, "\n")] = '\0';

  printf("Enter Category (Appetizer/Main Course/Dessert/Beverage): ");
  fgets(item.category, sizeof(item.category), stdin);
  item.category[strcspn(item.category, "\n")] = '\0';

  printf("Enter Price: Rs. ");
  scanf("%f", &item.price);

  item.available = 1;

  fprintf(fp, "%d|%s|%s|%.2f|%d\n", item.item_id, item.item_name, item.category,
          item.price, item.available);

  fclose(fp);

  printf("\n✓ Menu item added successfully!\n");
  pressEnterToContinue();
}

// ==================== VIEW MENU ====================
void viewMenu() {
  clearScreen();
  FILE *fp = fopen(MENU_FILE, "r");
  if (!fp) {
    printf("\n✗ Menu file not found!\n");
    return;
  }

  struct MenuItem item;
  int count = 0;

  printf("\n==================================================================="
         "=============\n");
  printf("                              RESTAURANT MENU\n");
  printf("====================================================================="
         "===========\n");
  printf("%-6s %-30s %-20s %-12s %-10s\n", "ID", "Item Name", "Category",
         "Price", "Status");
  printf("---------------------------------------------------------------------"
         "-----------\n");

  while (fscanf(fp, "%d|%49[^|]|%29[^|]|%f|%d\n", &item.item_id, item.item_name,
                item.category, &item.price, &item.available) != EOF) {
    printf("%-6d %-30s %-20s Rs. %-8.2f %-10s\n", item.item_id, item.item_name,
           item.category, item.price,
           item.available ? "Available" : "Not Available");
    count++;
  }

  printf("---------------------------------------------------------------------"
         "-----------\n");
  printf("Total Items: %d\n", count);
  printf("====================================================================="
         "===========\n");

  fclose(fp);
}

// ==================== UPDATE MENU ITEM ====================
void updateMenuItem() {
  viewMenu();
  int id, found = 0, choice;

  printf("\n========================================\n");
  printf("        UPDATE MENU ITEM\n");
  printf("========================================\n");

  printf("\nEnter Item ID to update: ");
  scanf("%d", &id);

  FILE *fp = fopen(MENU_FILE, "r");
  FILE *temp = fopen(TEMP_FILE, "w");

  if (!fp || !temp) {
    printf("\n✗ Error opening file!\n");
    pressEnterToContinue();
    return;
  }

  struct MenuItem item;

  while (fscanf(fp, "%d|%49[^|]|%29[^|]|%f|%d\n", &item.item_id, item.item_name,
                item.category, &item.price, &item.available) != EOF) {

    if (item.item_id == id) {
      found = 1;
      printf("\nCurrent Details:\n");
      printf("Name: %s\n", item.item_name);
      printf("Category: %s\n", item.category);
      printf("Price: Rs. %.2f\n", item.price);
      printf("Status: %s\n", item.available ? "Available" : "Not Available");

      printf("\nWhat do you want to update?\n");
      printf("1. Name\n");
      printf("2. Category\n");
      printf("3. Price\n");
      printf("4. Availability\n");
      printf("5. All Details\n");
      printf("\nEnter choice: ");
      scanf("%d", &choice);
      while (getchar() != '\n')
        ;

      switch (choice) {
      case 1:
        printf("Enter new name: ");
        fgets(item.item_name, sizeof(item.item_name), stdin);
        item.item_name[strcspn(item.item_name, "\n")] = '\0';
        break;
      case 2:
        printf("Enter new category: ");
        fgets(item.category, sizeof(item.category), stdin);
        item.category[strcspn(item.category, "\n")] = '\0';
        break;
      case 3:
        printf("Enter new price: Rs. ");
        scanf("%f", &item.price);
        break;
      case 4:
        printf("Available? (1=Yes, 0=No): ");
        scanf("%d", &item.available);
        break;
      case 5:
        printf("Enter new name: ");
        fgets(item.item_name, sizeof(item.item_name), stdin);
        item.item_name[strcspn(item.item_name, "\n")] = '\0';

        printf("Enter new category: ");
        fgets(item.category, sizeof(item.category), stdin);
        item.category[strcspn(item.category, "\n")] = '\0';

        printf("Enter new price: Rs. ");
        scanf("%f", &item.price);

        printf("Available? (1=Yes, 0=No): ");
        scanf("%d", &item.available);
        break;
      default:
        printf("\nInvalid choice! No changes made.\n");
      }
    }

    fprintf(temp, "%d|%s|%s|%.2f|%d\n", item.item_id, item.item_name,
            item.category, item.price, item.available);
  }

  fclose(fp);
  fclose(temp);

  remove(MENU_FILE);
  rename(TEMP_FILE, MENU_FILE);

  if (found) {
    printf("\n✓ Menu item updated successfully!\n");
  } else {
    printf("\n✗ Item ID not found!\n");
  }

  pressEnterToContinue();
}

// ==================== DELETE MENU ITEM ====================
void deleteMenuItem() {
  viewMenu();
  int id, found = 0;
  char confirm;

  printf("\n========================================\n");
  printf("        DELETE MENU ITEM\n");
  printf("========================================\n");

  printf("\nEnter Item ID to delete: ");
  scanf("%d", &id);

  FILE *fp = fopen(MENU_FILE, "r");
  FILE *temp = fopen(TEMP_FILE, "w");

  if (!fp || !temp) {
    printf("\n✗ Error opening file!\n");
    pressEnterToContinue();
    return;
  }

  struct MenuItem item;

  while (fscanf(fp, "%d|%49[^|]|%29[^|]|%f|%d\n", &item.item_id, item.item_name,
                item.category, &item.price, &item.available) != EOF) {

    if (item.item_id == id) {
      found = 1;
      printf("\nItem Found:\n");
      printf("ID: %d\n", item.item_id);
      printf("Name: %s\n", item.item_name);
      printf("Price: Rs. %.2f\n", item.price);

      printf("\nAre you sure you want to delete? (y/n): ");
      scanf(" %c", &confirm);

      if (confirm == 'y' || confirm == 'Y') {
        printf("\n✓ Item deleted successfully!\n");
        continue; // Skip writing this item
      } else {
        printf("\n✗ Deletion cancelled!\n");
      }
    }

    fprintf(temp, "%d|%s|%s|%.2f|%d\n", item.item_id, item.item_name,
            item.category, item.price, item.available);
  }

  fclose(fp);
  fclose(temp);

  remove(MENU_FILE);
  rename(TEMP_FILE, MENU_FILE);

  if (!found) {
    printf("\n✗ Item ID not found!\n");
  }

  pressEnterToContinue();
}

// ==================== PLACE ORDER ====================
void placeOrder() {
  clearScreen();

  // Initialize order
  currentOrder.order_id = generateOrderID();
  currentOrder.item_count = 0;
  currentOrder.total_amount = 0;
  strcpy(currentOrder.status, "Pending");

  // Get current date
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  sprintf(currentOrder.date, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1,
          tm.tm_year + 1900);

  printf("\n========================================\n");
  printf("          PLACE NEW ORDER\n");
  printf("========================================\n");

  printf("\nEnter Customer Name: ");
  fgets(currentOrder.customer_name, sizeof(currentOrder.customer_name), stdin);
  currentOrder.customer_name[strcspn(currentOrder.customer_name, "\n")] = '\0';

  // Display menu
  viewCustomerMenu();

  int item_id, quantity;
  char more;

  do {
    printf("\nEnter Item ID: ");
    if (scanf("%d", &item_id) != 1) {
      while (getchar() != '\n')
        ;
      printf("Invalid input!\n");
      continue;
    }

    printf("Enter Quantity: ");
    if (scanf("%d", &quantity) != 1 || quantity <= 0) {
      while (getchar() != '\n')
        ;
      printf("Invalid quantity!\n");
      continue;
    }
    while (getchar() != '\n')
      ;

    // Search for item in menu
    FILE *fp = fopen(MENU_FILE, "r");
    if (!fp) {
      printf("\n✗ Error opening menu file!\n");
      pressEnterToContinue();
      return;
    }

    struct MenuItem item;
    int found = 0;

    while (fscanf(fp, "%d|%49[^|]|%29[^|]|%f|%d\n", &item.item_id,
                  item.item_name, item.category, &item.price,
                  &item.available) != EOF) {

      if (item.item_id == item_id && item.available == 1) {
        found = 1;

        // Add to order
        int idx = currentOrder.item_count;
        currentOrder.items[idx].item_id = item.item_id;
        strcpy(currentOrder.items[idx].item_name, item.item_name);
        currentOrder.items[idx].price = item.price;
        currentOrder.items[idx].quantity = quantity;
        currentOrder.items[idx].subtotal = item.price * quantity;

        currentOrder.item_count++;

        printf("\n✓ Added: %s x %d = Rs. %.2f\n", item.item_name, quantity,
               item.price * quantity);
        break;
      }
    }

    fclose(fp);

    if (!found) {
      printf("\n✗ Item not found or not available!\n");
    }

    printf("\nAdd more items? (y/n): ");
    scanf(" %c", &more);
    while (getchar() != '\n')
      ;

  } while ((more == 'y' || more == 'Y') && currentOrder.item_count < 20);

  if (currentOrder.item_count == 0) {
    printf("\n✗ No items added! Order cancelled.\n");
    pressEnterToContinue();
    return;
  }

  // Calculate bill
  calculateBill(&currentOrder);

  // Display order summary
  clearScreen();
  displayOrder(currentOrder);

  // Save order to file
  FILE *fp = fopen(ORDER_FILE, "a");
  if (!fp) {
    printf("\n✗ Error saving order!\n");
    pressEnterToContinue();
    return;
  }

  fprintf(fp, "%d|%s|%s|%d|%.2f|%s\n", currentOrder.order_id,
          currentOrder.customer_name, currentOrder.date,
          currentOrder.item_count, currentOrder.total_amount,
          currentOrder.status);

  // Save order items
  for (int i = 0; i < currentOrder.item_count; i++) {
    fprintf(fp, "ITEM|%d|%s|%.2f|%d|%.2f\n", currentOrder.items[i].item_id,
            currentOrder.items[i].item_name, currentOrder.items[i].price,
            currentOrder.items[i].quantity, currentOrder.items[i].subtotal);
  }

  fclose(fp);

  printf("\n✓ Order placed successfully!\n");
  printf("Your Order ID: %d\n", currentOrder.order_id);
  pressEnterToContinue();
}

// ==================== VIEW CUSTOMER MENU ====================
void viewCustomerMenu() {
  FILE *fp = fopen(MENU_FILE, "r");
  if (!fp) {
    printf("\n✗ Menu not available!\n");
    return;
  }

  struct MenuItem item;

  printf("\n==================================================================="
         "=============\n");
  printf("                              OUR MENU\n");
  printf("====================================================================="
         "===========\n");
  printf("%-6s %-35s %-20s %-10s\n", "ID", "Item Name", "Category", "Price");
  printf("---------------------------------------------------------------------"
         "-----------\n");

  while (fscanf(fp, "%d|%49[^|]|%29[^|]|%f|%d\n", &item.item_id, item.item_name,
                item.category, &item.price, &item.available) != EOF) {
    if (item.available == 1) {
      printf("%-6d %-35s %-20s Rs. %.2f\n", item.item_id, item.item_name,
             item.category, item.price);
    }
  }

  printf("====================================================================="
         "===========\n");

  fclose(fp);
}

// ==================== VIEW MENU BY CATEGORY ====================
void viewMenuByCategory() {
  clearScreen();
  FILE *fp = fopen(MENU_FILE, "r");
  if (!fp) {
    printf("\n✗ Menu not available!\n");
    return;
  }

  int choice;
  char selected_category[30];

  printf("\n========================================\n");
  printf("      SELECT CATEGORY\n");
  printf("========================================\n");
  printf("\n  1. Appetizer\n");
  printf("  2. Main Course\n");
  printf("  3. Dessert\n");
  printf("  4. Beverage\n");
  printf("----------------------------------------\n");
  printf("\nEnter your choice: ");

  if (scanf("%d", &choice) != 1) {
    while (getchar() != '\n')
      ;
    printf("\n✗ Invalid input!\n");
    fclose(fp);
    return;
  }
  while (getchar() != '\n')
    ;

  switch (choice) {
  case 1:
    strcpy(selected_category, "Appetizer");
    break;
  case 2:
    strcpy(selected_category, "Main Course");
    break;
  case 3:
    strcpy(selected_category, "Dessert");
    break;
  case 4:
    strcpy(selected_category, "Beverage");
    break;
  default:
    printf("\n✗ Invalid choice!\n");
    fclose(fp);
    return;
  }

  struct MenuItem item;
  int count = 0;

  printf("\n==================================================================="
         "=============\n");
  printf("                         MENU - %s\n", selected_category);
  printf("====================================================================="
         "===========\n");
  printf("%-6s %-35s %-20s %-10s\n", "ID", "Item Name", "Category", "Price");
  printf("---------------------------------------------------------------------"
         "-----------\n");

  rewind(fp);

  while (fscanf(fp, "%d|%49[^|]|%29[^|]|%f|%d\n", &item.item_id, item.item_name,
                item.category, &item.price, &item.available) != EOF) {
    if (item.available == 1 && strcmp(item.category, selected_category) == 0) {
      printf("%-6d %-35s %-20s Rs. %.2f\n", item.item_id, item.item_name,
             item.category, item.price);
      count++;
    }
  }

  if (count == 0) {
    printf("No items available in this category.\n");
  }

  printf("---------------------------------------------------------------------"
         "-----------\n");
  printf("Total Items in %s: %d\n", selected_category, count);
  printf("====================================================================="
         "===========\n");

  fclose(fp);
}

// ==================== GENERATE SALES REPORT ====================
void generateSalesReport() {
  clearScreen();
  FILE *fp = fopen(ORDER_FILE, "r");
  if (!fp) {
    printf("\n✗ No orders found! Cannot generate report.\n");
    return;
  }

  printf("\n==================================================================="
         "=============\n");
  printf("                           SALES REPORT\n");
  printf("====================================================================="
         "===========\n");

  struct Order order;
  char line[200];

  int total_orders = 0;
  int pending_orders = 0;
  int completed_orders = 0;
  int cancelled_orders = 0;
  float total_revenue = 0.0;
  float completed_revenue = 0.0;

  // Item sales tracking
  struct ItemSales {
    int item_id;
    char item_name[50];
    int total_quantity;
    float total_sales;
  } item_sales[100];
  int item_count = 0;

  // Read all orders
  while (fgets(line, sizeof(line), fp)) {
    if (sscanf(line, "%d|%49[^|]|%39[^|]|%d|%f|%19[^\n]", &order.order_id,
               order.customer_name, order.date, &order.item_count,
               &order.total_amount, order.status) == 6) {

      total_orders++;
      total_revenue += order.total_amount;

      if (strcmp(order.status, "Pending") == 0) {
        pending_orders++;
      } else if (strcmp(order.status, "Completed") == 0) {
        completed_orders++;
        completed_revenue += order.total_amount;
      } else if (strcmp(order.status, "Cancelled") == 0) {
        cancelled_orders++;
      }

      // Read item details for this order
      for (int i = 0; i < order.item_count; i++) {
        char item_line[200];
        int item_id, quantity;
        char item_name[50];
        float price, subtotal;

        if (fgets(item_line, sizeof(item_line), fp)) {
          if (sscanf(item_line, "ITEM|%d|%49[^|]|%f|%d|%f", &item_id, item_name,
                     &price, &quantity, &subtotal) == 5) {

            // Only count completed orders for item sales
            if (strcmp(order.status, "Completed") == 0) {
              // Check if item already exists in tracking
              int found = 0;
              for (int j = 0; j < item_count; j++) {
                if (item_sales[j].item_id == item_id) {
                  item_sales[j].total_quantity += quantity;
                  item_sales[j].total_sales += subtotal;
                  found = 1;
                  break;
                }
              }

              // If not found, add new item
              if (!found && item_count < 100) {
                item_sales[item_count].item_id = item_id;
                strcpy(item_sales[item_count].item_name, item_name);
                item_sales[item_count].total_quantity = quantity;
                item_sales[item_count].total_sales = subtotal;
                item_count++;
              }
            }
          }
        }
      }
    }
  }

  fclose(fp);

  // Display Summary
  printf("\n--- ORDER SUMMARY ---\n");
  printf("Total Orders: %d\n", total_orders);
  printf("  - Pending: %d\n", pending_orders);
  printf("  - Completed: %d\n", completed_orders);
  printf("  - Cancelled: %d\n", cancelled_orders);

  printf("\n--- REVENUE SUMMARY ---\n");
  printf("Total Revenue (All Orders): Rs. %.2f\n", total_revenue);
  printf("Completed Orders Revenue: Rs. %.2f\n", completed_revenue);

  if (completed_orders > 0) {
    printf("Average Order Value: Rs. %.2f\n",
           completed_revenue / completed_orders);
  }

  // Display Item Sales
  if (item_count > 0) {
    printf("\n================================================================="
           "===============\n");
    printf("                         ITEM-WISE SALES REPORT\n");
    printf("==================================================================="
           "=============\n");
    printf("%-6s %-35s %-15s %-15s\n", "ID", "Item Name", "Qty Sold",
           "Revenue");
    printf("-------------------------------------------------------------------"
           "-------------\n");

    // Sort items by quantity sold (bubble sort - simple implementation)
    for (int i = 0; i < item_count - 1; i++) {
      for (int j = 0; j < item_count - i - 1; j++) {
        if (item_sales[j].total_quantity < item_sales[j + 1].total_quantity) {
          struct ItemSales temp = item_sales[j];
          item_sales[j] = item_sales[j + 1];
          item_sales[j + 1] = temp;
        }
      }
    }

    // Display sorted items
    for (int i = 0; i < item_count; i++) {
      printf("%-6d %-35s %-15d Rs. %.2f\n", item_sales[i].item_id,
             item_sales[i].item_name, item_sales[i].total_quantity,
             item_sales[i].total_sales);
    }

    printf("==================================================================="
           "=============\n");

    // Display Top 5 Best Sellers
    printf("\n--- TOP 5 BEST SELLING ITEMS ---\n");
    int top_count = (item_count < 5) ? item_count : 5;
    for (int i = 0; i < top_count; i++) {
      printf("%d. %s - %d units sold (Rs. %.2f)\n", i + 1,
             item_sales[i].item_name, item_sales[i].total_quantity,
             item_sales[i].total_sales);
    }
  } else {
    printf("\n--- ITEM-WISE SALES REPORT ---\n");
    printf("No completed orders to generate item sales report.\n");
  }

  printf("\n==================================================================="
         "=============\n");
  printf("                         END OF REPORT\n");
  printf("====================================================================="
         "===========\n");
}

// ==================== VIEW ALL ORDERS ====================
void viewAllOrders() {
  clearScreen();
  FILE *fp = fopen(ORDER_FILE, "r");
  if (!fp) {
    printf("\n✗ No orders found!\n");
    return;
  }

  printf("\n==================================================================="
         "=============\n");
  printf("                           ALL ORDERS\n");
  printf("====================================================================="
         "===========\n");

  struct Order order;
  char line[200];
  int order_count = 0;

  while (fgets(line, sizeof(line), fp)) {
    if (sscanf(line, "%d|%49[^|]|%39[^|]|%d|%f|%19[^\n]", &order.order_id,
               order.customer_name, order.date, &order.item_count,
               &order.total_amount, order.status) == 6) {

      order_count++;
      printf("\n--- Order #%d ---\n", order.order_id);
      printf("Customer: %s\n", order.customer_name);
      printf("Date: %s\n", order.date);
      printf("No. of Items: %d\n", order.item_count);

      // Read and display item details
      printf("Items: ");
      for (int i = 0; i < order.item_count; i++) {
        char item_line[200];
        char item_name[50];
        int item_id, quantity;
        float price, subtotal;

        if (fgets(item_line, sizeof(item_line), fp)) {
          if (sscanf(item_line, "ITEM|%d|%49[^|]|%f|%d|%f", &item_id, item_name,
                     &price, &quantity, &subtotal) == 5) {
            if (i > 0)
              printf(", ");
            printf("%s(x%d) (Rs.%.2f)", item_name, quantity, subtotal);
          }
        }
      }
      printf("\n");

      printf("Total: Rs. %.2f\n", order.total_amount);
      printf("Status: %s\n", order.status);

      printf("-----------------------------------------------------------------"
             "---------------\n");
    }
  }

  printf("\nTotal Orders: %d\n", order_count);
  printf("====================================================================="
         "===========\n");

  fclose(fp);
}

// ==================== VIEW ORDERS BY STATUS ====================
void viewOrdersByStatus() {
  clearScreen();
  char search_status[20];

  printf("\n========================================\n");
  printf("      VIEW ORDERS BY STATUS\n");
  printf("========================================\n");
  printf("\nEnter Status (Pending/Completed/Cancelled): ");
  fgets(search_status, sizeof(search_status), stdin);
  search_status[strcspn(search_status, "\n")] = '\0';

  FILE *fp = fopen(ORDER_FILE, "r");
  if (!fp) {
    printf("\n✗ No orders found!\n");
    return;
  }

  struct Order order;
  char line[200];
  int found = 0;

  printf("\n==================================================================="
         "=============\n");
  printf("Orders with Status: %s\n", search_status);
  printf("====================================================================="
         "===========\n");

  while (fgets(line, sizeof(line), fp)) {
    if (sscanf(line, "%d|%49[^|]|%39[^|]|%d|%f|%19[^\n]", &order.order_id,
               order.customer_name, order.date, &order.item_count,
               &order.total_amount, order.status) == 6) {

      if (strcmp(order.status, search_status) == 0) {
        found++;
        printf("\n--- Order #%d ---\n", order.order_id);
        printf("Customer: %s\n", order.customer_name);
        printf("Date: %s\n", order.date);
        printf("No. of Items: %d\n", order.item_count);

        // Read and display item details
        printf("Items: ");
        for (int i = 0; i < order.item_count; i++) {
          char item_line[200];
          char item_name[50];
          int item_id, quantity;
          float price, subtotal;

          if (fgets(item_line, sizeof(item_line), fp)) {
            if (sscanf(item_line, "ITEM|%d|%49[^|]|%f|%d|%f", &item_id,
                       item_name, &price, &quantity, &subtotal) == 5) {
              if (i > 0)
                printf(", ");
              printf("%s(x%d) (Rs.%.2f)", item_name, quantity, subtotal);
            }
          }
        }
        printf("\n");

        printf("Total: Rs. %.2f\n", order.total_amount);
        printf("---------------------------------------------------------------"
               "-----------------\n");
      } else {
        // Skip item lines for non-matching orders
        for (int i = 0; i < order.item_count; i++) {
          fgets(line, sizeof(line), fp);
        }
      }
    }
  }

  if (found == 0) {
    printf("\nNo orders found with status: %s\n", search_status);
  } else {
    printf("\nTotal Orders Found: %d\n", found);
  }

  printf("====================================================================="
         "===========\n");

  fclose(fp);
}

// ==================== UPDATE ORDER STATUS ====================
void updateOrderStatus() {
  viewAllOrders();
  int order_id, found = 0;
  char new_status[20];

  printf("\n========================================\n");
  printf("      UPDATE ORDER STATUS\n");
  printf("========================================\n");

  printf("\nEnter Order ID to update: ");
  if (scanf("%d", &order_id) != 1) {
    while (getchar() != '\n')
      ;
    printf("\n✗ Invalid input!\n");
    pressEnterToContinue();
    return;
  }
  while (getchar() != '\n')
    ;

  FILE *fp = fopen(ORDER_FILE, "r");
  FILE *temp = fopen(TEMP_FILE, "w");

  if (!fp || !temp) {
    printf("\n✗ Error opening file!\n");
    pressEnterToContinue();
    return;
  }

  struct Order order;
  char line[200];

  while (fgets(line, sizeof(line), fp)) {
    if (sscanf(line, "%d|%49[^|]|%39[^|]|%d|%f|%19[^\n]", &order.order_id,
               order.customer_name, order.date, &order.item_count,
               &order.total_amount, order.status) == 6) {

      if (order.order_id == order_id) {
        found = 1;
        printf("\nCurrent Order Details:\n");
        printf("Order ID: %d\n", order.order_id);
        printf("Customer: %s\n", order.customer_name);
        printf("Date: %s\n", order.date);
        printf("No. of Items: %d\n", order.item_count);

        // Read and display item details
        printf("Items: ");
        char item_lines[20][200]; // Store item lines temporarily
        for (int i = 0; i < order.item_count; i++) {
          char item_name[50];
          int item_id, quantity;
          float price, subtotal;

          if (fgets(item_lines[i], sizeof(item_lines[i]), fp)) {
            if (sscanf(item_lines[i], "ITEM|%d|%49[^|]|%f|%d|%f", &item_id,
                       item_name, &price, &quantity, &subtotal) == 5) {
              if (i > 0)
                printf(", ");
              printf("%s(x%d) (Rs.%.2f)", item_name, quantity, subtotal);
            }
          }
        }
        printf("\n");

        printf("Total: Rs. %.2f\n", order.total_amount);
        printf("Current Status: %s\n", order.status);

        printf("\nSelect New Status:\n");
        printf("1. Pending\n");
        printf("2. Completed\n");
        printf("3. Cancelled\n");
        printf("\nEnter choice: ");

        int choice;
        if (scanf("%d", &choice) != 1) {
          while (getchar() != '\n')
            ;
          printf("\n✗ Invalid input!\n");
          fprintf(temp, "%s", line);
          // Copy remaining item lines that were already read
          for (int i = 0; i < order.item_count; i++) {
            fprintf(temp, "%s", item_lines[i]);
          }
          found = 1; // Mark as found to avoid "not found" message
          break;
        }
        while (getchar() != '\n')
          ;

        switch (choice) {
        case 1:
          strcpy(new_status, "Pending");
          break;
        case 2:
          strcpy(new_status, "Completed");
          break;
        case 3:
          strcpy(new_status, "Cancelled");
          break;
        default:
          printf("\n✗ Invalid choice! No changes made.\n");
          strcpy(new_status, order.status);
        }

        // Write updated order
        fprintf(temp, "%d|%s|%s|%d|%.2f|%s\n", order.order_id,
                order.customer_name, order.date, order.item_count,
                order.total_amount, new_status);

        // Write item lines
        for (int i = 0; i < order.item_count; i++) {
          fprintf(temp, "%s", item_lines[i]);
        }

        printf("\n✓ Order status updated to: %s\n", new_status);
      } else {
        // Write unchanged order
        fprintf(temp, "%s", line);
      }

      // Copy item lines
      for (int i = 0; i < order.item_count; i++) {
        fgets(line, sizeof(line), fp);
        fprintf(temp, "%s", line);
      }
    } else {
      // Write any other lines as-is
      fprintf(temp, "%s", line);
    }
  }

  fclose(fp);
  fclose(temp);

  remove(ORDER_FILE);
  rename(TEMP_FILE, ORDER_FILE);

  if (!found) {
    printf("\n✗ Order ID not found!\n");
  }

  pressEnterToContinue();
}

// ==================== CALCULATE BILL ====================
void calculateBill(struct Order *order) {
  order->total_amount = 0;
  for (int i = 0; i < order->item_count; i++) {
    order->total_amount += order->items[i].subtotal;
  }
}

// ==================== DISPLAY ORDER ====================
void displayOrder(struct Order order) {
  printf("\n==================================================================="
         "=============\n");
  printf("                              ORDER BILL\n");
  printf("====================================================================="
         "===========\n");
  printf("Order ID: %d\n", order.order_id);
  printf("Customer Name: %s\n", order.customer_name);
  printf("Date: %s\n", order.date);
  printf("====================================================================="
         "===========\n");
  printf("%-6s %-30s %-10s %-10s %-12s\n", "ID", "Item", "Price", "Qty",
         "Subtotal");
  printf("---------------------------------------------------------------------"
         "-----------\n");

  for (int i = 0; i < order.item_count; i++) {
    printf("%-6d %-30s Rs. %-7.2f %-10d Rs. %.2f\n", order.items[i].item_id,
           order.items[i].item_name, order.items[i].price,
           order.items[i].quantity, order.items[i].subtotal);
  }

  printf("====================================================================="
         "===========\n");
  printf("                                           TOTAL: Rs. %.2f\n",
         order.total_amount);
  printf("====================================================================="
         "===========\n");
}

// ==================== GENERATE ORDER ID ====================
int generateOrderID() {
  FILE *fp = fopen(ORDER_FILE, "r");
  if (!fp) {
    return 1001; // First order ID
  }

  int max_id = 1000;
  char line[200];
  int id;

  while (fgets(line, sizeof(line), fp)) {
    if (sscanf(line, "%d|", &id) == 1) {
      if (id > max_id) {
        max_id = id;
      }
    }
  }

  fclose(fp);
  return max_id + 1;
}

// ==================== INITIALIZE MENU FILE ====================
void initializeMenuFile() {
  FILE *fp = fopen(MENU_FILE, "r");
  if (fp) {
    fclose(fp);
    return; // File exists
  }

  // Create file with sample items
  fp = fopen(MENU_FILE, "w");
  if (!fp)
    return;

  fprintf(fp, "101|Momo (Veg)|Appetizer|150.00|1\n");
  fprintf(fp, "102|Momo (Chicken)|Appetizer|180.00|1\n");
  fprintf(fp, "103|Chowmein|Main Course|120.00|1\n");
  fprintf(fp, "104|Fried Rice|Main Course|140.00|1\n");
  fprintf(fp, "105|Dal Bhat|Main Course|200.00|1\n");
  fprintf(fp, "106|Pizza|Main Course|350.00|1\n");
  fprintf(fp, "107|Burger|Main Course|180.00|1\n");
  fprintf(fp, "108|Ice Cream|Dessert|100.00|1\n");
  fprintf(fp, "109|Gulab Jamun|Dessert|80.00|1\n");
  fprintf(fp, "110|Coca Cola|Beverage|60.00|1\n");
  fprintf(fp, "111|Fresh Juice|Beverage|100.00|1\n");

  fclose(fp);
}

// ==================== UTILITY FUNCTIONS ====================
void pressEnterToContinue() {
  printf("\nPress Enter to continue...");
  getchar();
}

void clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}