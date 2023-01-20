struct Clock {
    int month;
    int day;
    int year;
    int hour;
    int min;
    int sec;
}; 

// Command to read/write the current time
void time(char* args);

// Command to read/write the current date
void date(char* args);