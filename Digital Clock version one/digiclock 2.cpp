#include <iostream>      // Allows us to use input and output like printing text to the screen
#include <conio.h>       // Provides functions to interact with the console (like detecting key presses)
#include <windows.h>     // Includes Windows-specific functions like controlling the cursor
#include <time.h>        // Helps with working with time

using namespace std;  // So we don't have to type "std::" every time we use something from the standard library

// These are patterns to display the digits 0-9 on the screen using characters like blocks and spaces
unsigned char d0[5][3]  =  {  // This is the pattern for the digit '0'
    178, 178, 178,      // Top row is all filled with a block character (this forms the top of the digit '0')
    178, ' ', 178,      // Second row, spaces inside and blocks on the sides
    178, ' ', 178,      // Third row, same as second
    178, ' ', 178,      // Fourth row, same as second
    178, 178, 178       // Last row is all blocks, forming the bottom of the '0'
};

unsigned char d1[5][3] = {  // This is the pattern for the digit '1'
    ' ', ' ', 178,       // The first row has spaces with a block on the far right
    ' ', ' ', 178,       // Same for second, third, fourth, and fifth rows
    ' ', ' ', 178,
    ' ', ' ', 178,
    ' ', ' ', 178
};

// Patterns for digits 2-9 are similar to the ones above, just with different block placements
unsigned char d2[5][3] = {  // Digit '2' pattern
    178, 178, 178,
    ' ', ' ', 178,
    178, 178, 178,
    178, ' ', ' ',
    178, 178, 178
};

// And so on for digits 3 to 9...
unsigned char d3[5][3] = { /* Digit '3' pattern */ };
unsigned char d4[5][3] = { /* Digit '4' pattern */ };
unsigned char d5[5][3] = { /* Digit '5' pattern */ };
unsigned char d6[5][3] = { /* Digit '6' pattern */ };
unsigned char d7[5][3] = { /* Digit '7' pattern */ };
unsigned char d8[5][3] = { /* Digit '8' pattern */ };
unsigned char d9[5][3] = { /* Digit '9' pattern */ };

// A pattern for the separator ':' symbol (used to separate hours, minutes, and seconds)
unsigned char sep[5][3] = {
    ' ', ' ', ' ',
    ' ', 178, ' ',
    ' ', ' ', ' ',
    ' ', 178, ' ',
    ' ', ' ', ' '
};

// This is a special variable to control the cursor position on the screen
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);  // Get the console's output handle
COORD CursorPosition;  // Holds the position of the cursor

// Function to move the cursor to a specific (x, y) position on the screen
void gotoxy(int x, int y) {
    CursorPosition.X = x;  // Set X coordinate (left-right position)
    CursorPosition.Y = y;  // Set Y coordinate (up-down position)
    SetConsoleCursorPosition(console, CursorPosition);  // Actually move the cursor to the new position
}

// Function to hide or show the cursor and set its size
void setcursor(bool visible, DWORD size) {
    if (size == 0) size = 20;  // If no size is given, set it to 20 (a good default)

    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;  // Whether to show or hide the cursor
    lpCursor.dwSize = size;      // Size of the cursor (usually between 1 and 100)
    SetConsoleCursorInfo(console, &lpCursor);  // Set the cursor properties
}

// Function to print a digit on the screen at a given position
void printDigit(int no, int x, int y) {
    // Loop over each row and column in the 5x3 grid for each digit
    for (int i = 0; i < 5; i++) {  
        for (int j = 0; j < 3; j++) {
            // For each digit, print the corresponding block pattern
            switch (no) {
                case 0: gotoxy(x + j, y + i); cout << d0[i][j]; break;
                case 1: gotoxy(x + j, y + i); cout << d1[i][j]; break;
                case 2: gotoxy(x + j, y + i); cout << d2[i][j]; break;
                // Repeat for all digits 3-9 and the separator
                case 10: gotoxy(x + j, y + i); cout << sep[i][j]; break;
            }
        }
    }
}

// Function to draw a border around the clock area on the screen
void drawBorder(int x = 0, int y = 0) {
    char v = 186;  // Vertical line character
    char h = 205;  // Horizontal line character
    char tr = 187; // Top-right corner
    char br = 188; // Bottom-right corner
    char tl = 201; // Top-left corner
    char bl = 200; // Bottom-left corner
    int width = 50; // Width of the border
    int height = 11; // Height of the border

    gotoxy((width / 2) + x - 11, y);  // Position the title of the clock
    cout << "C U R R E N T   T I M E";   // Print the title

    // Draw the border line by line
    for (int i = 1; i <= height; i++) {
        for (int j = 1; j <= width; j++) {
            gotoxy(j + x, i + y);  // Move the cursor to the correct position
            if (i == 1 && j == 1) cout << tl;  // Top-left corner
            else if (i == height && j == 1) cout << bl;  // Bottom-left corner
            else if (i == 1 && j == width) cout << tr;  // Top-right corner
            else if (i == height && j == width) cout << br;  // Bottom-right corner
            else if (i == 1 || i == height) cout << h;  // Top or bottom horizontal lines
            else if (j == 1 || j == width) cout << v;  // Left or right vertical lines
        }
    }
}

int main() {
    system("cls");  // Clear the screen
    setcursor(0, 0);  // Hide the cursor for a clean output
    srand((unsigned)time(NULL));  // Seed the random number generator (used for random stuff later)

    // Get the current time
    time_t now = time(0);  // Get the current time in seconds since 1970
    char* dt = ctime(&now);  // Convert to a human-readable string
    tm* lotm = localtime(&now);  // Convert to a structured format with hours, minutes, etc.
    dt = asctime(lotm);  // Convert it to a more readable format

    // Extract the current hour, minute, and second from the current time
    int hour = lotm->tm_hour;
    int min = lotm->tm_min;
    int sec = lotm->tm_sec;

    int gap = 5;  // Space between the digits when printing them
    int posX;     // The X position where to start printing digits
    int posY = 8; // The Y position where to print the digits

    while (1) {  // Run the loop forever (until you press ESC)
        system("cls");  // Clear the screen each time to refresh the time display
        drawBorder(9, 4);  // Draw the border around the clock

        posX = 15;  // Set the starting X position

        if (kbhit()) {  // Check if a key was pressed
            char ch = getch();  // Get the key that was pressed
            if (ch == 27) {  // If the key is ESC, stop the loop
                break;
            }
        }

        // Print the hours, minutes, and seconds as digits
        if (hour < 10) {  // If the hour is less than 10, print a leading zero
            printDigit(0, posX, posY);  // Print '0'
            printDigit(hour, posX += gap, posY);  // Print the hour
        } else {
            printDigit(hour / 10, posX, posY);  // Print the first digit of the hour
            printDigit(hour % 10, posX += gap, posY);  // Print the second digit of the hour
        }

        printDigit(10, posX += gap, posY);  // Print the separator ":"

        if (min < 10) {  // Same for the minutes
            printDigit(0, posX += gap, posY);
            printDigit(min, posX += gap, posY);
        } else {
            printDigit(min / 10, posX += gap, posY);
            printDigit(min % 10, posX += gap, posY);
        }

        printDigit(10, posX += gap, posY);  // Separator ":"

        if (sec < 10) {  // Same for the seconds
            printDigit(0, posX += gap, posY);
            printDigit(sec, posX += gap, posY);
        } else {
            printDigit(sec / 10, posX += gap, posY);
            printDigit(sec % 10, posX += gap, posY);
        }

        Sleep(1000);  // Wait for 1 second before updating the time
        sec++;  // Increase the second
        if (sec >= 60) {  // If 60 seconds passed, reset and increase minute
            sec = 0;
            min++;
        }
        if (min >= 60) {  // If 60 minutes passed, reset and increase hour
            min = 0;
            hour++;
        }
        if (hour >= 24) {  // If 24 hours passed, reset to 0
            sec = 0;
            min = 0;
            hour = 0;
        }
    }

    return 0;  // End of the program
}
