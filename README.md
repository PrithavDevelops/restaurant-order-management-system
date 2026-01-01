Restaurant Order Management System

A console-based restaurant order management system developed in C programming language as part of Grade XII Computer Science project.

📋 Project Description

The Restaurant Order Management System is designed to streamline restaurant operations by managing menu items, processing customer orders, and generating bills automatically. The system uses file handling techniques for persistent data storage.

✨ Features

Admin Features

    Add, update, and delete menu items
    View all menu items with availability status
    View all orders
    Filter orders by status (Pending/Completed/Cancelled)
    Secure admin login system

Customer Features

    View available menu items
    Place orders with multiple items
    Automatic bill calculation
    Order confirmation with unique Order ID

🛠️ Technical Specifications

    Language: C Programming
    Storage: File-based (Text Files)
    Interface: Command Line Interface (CLI)
    Compiler: GCC / Dev C++ / Turbo C

📁 File Structure

restaurant-order-management/
├── src/
│   └── main.c              # Main source code
├── data/
│   ├── menu.txt            # Menu items storage
│   └── orders.txt          # Order records storage
├── docs/
│   └── Project_Report.pdf  # Detailed project documentation
├── README.md
├── .gitignore
└── LICENSE

🚀 How to Run

Prerequisites

    C Compiler (GCC recommended)
    Windows/Linux/macOS

Compilation and Execution

On Windows (using GCC):

bash
gcc src/main.c -o restaurant_system.exe
restaurant_system.exe

On Linux/macOS:

bash
gcc src/main.c -o restaurant_system
./restaurant_system

Using Dev C++:

    Open src/main.c in Dev C++
    Press F9 to compile and run

👥 Default Login Credentials

Admin Access:

    Username: admin
    Password: admin123

⚠️ Note: Change these credentials in production use.

📊 Data Files

The system uses two main data files:

    menu.txt - Stores menu items in format: ID|Name|Category|Price|Availability
    orders.txt - Stores order records and order items

🎯 Key Features Implementation

Menu Management

    Dynamic menu with categories (Appetizer, Main Course, Dessert, Beverage)
    Real-time availability tracking
    Price management

Order Processing

    Multi-item order support
    Automatic subtotal and total calculation
    Unique order ID generation
    Date/time stamping

Data Persistence

    File-based storage ensures data survives program restarts
    Structured data format for easy parsing
    Automatic file initialization with sample data

📝 Sample Menu Items

ID	Item Name	Category	Price
101	Momo (Veg)	Appetizer	Rs. 150
102	Momo (Chicken)	Appetizer	Rs. 180
103	Chowmein	Main Course	Rs. 120
104	Fried Rice	Main Course	Rs. 140
105	Dal Bhat	Main Course	Rs. 200

🔧 System Requirements

Minimum Requirements

    Processor: Intel Pentium IV or higher
    RAM: 2 GB (4 GB recommended)
    Storage: 100 MB free space
    Display: Standard VGA or higher

Software Requirements

    OS: Windows 7/10/11, Linux, or macOS
    Compiler: Any C compiler (GCC, Turbo C, Dev C++)

📚 Project Structure

The project follows modular programming approach with the following modules:

    Main Module - Menu navigation and program flow
    Admin Module - Administrative functions
    Customer Module - Customer-facing operations
    Menu Management - CRUD operations for menu items
    Order Processing - Order placement and bill generation
    File Handling - Data persistence operations

🎓 Educational Context

This project was developed as part of the Grade XII Computer Science curriculum (NEB, Nepal) to demonstrate:

    File handling in C
    Structure usage
    Modular programming
    Input validation
    Menu-driven interface design

🚧 Known Limitations

    Single-user system (no concurrent access)
    Text-based interface only
    No database integration
    Limited to local machine usage
    Basic authentication system

🔮 Future Enhancements

    Graphical User Interface (GUI)
    Database integration (MySQL/SQLite)
    Multi-user support
    Advanced reporting features
    Receipt printing functionality
    Table management system
    Employee management
    Sales analytics and reports

📄 License

This project is created for educational purposes. Feel free to use and modify for learning.

👨‍💻 Authors

Prithav JHa

    Symbol No: 812830
    Class: Grade XII - D2
    School: Kathmandu Model Secondary School, Bagbazar, kathmandu

🙏 Acknowledgments

    Computer Science Department for project guidance
    National Examination Board (NEB) for curriculum framework
    My parents for supervision and support

📞 Contact

For questions or suggestions, please reach out:

    Email: prith4v@proton.me
    GitHub: @PrithavDevelops

Project Status: ✅ Completed and Submitted

Submission Date: Poush 04, 2082 (Nepali Calendar - Bikram Sambat)

Made with ❤️ for Grade XII Computer Science Project
