typedef struct alarm_t {
    int hour;
    int minute;
    int second;
    char *message;
    struct alarm_t *next;
} alarm_t;

extern alarm_t *alarm_list;

/**
 * Inserts an alarm into the alarm list based on the time
 * @param a alarm to insert
 */
void alarm_insert(alarm_t *a);

/**
 * Removes the first alarm from the alarm list
 * @return 0 if the alarm was removed, 1 if the alarm list was empty
 */
int alarm_remove();

/**
 * Creates an alarm to be executed at the specified time
 * @param time the time to execute the alarm in the format hh:mm:ss
 * @param message the message to output when the alarm is executed
 */
void alarm_setup(char* time, char *message);

/**
 * Executes all alarms that are scheduled between the current time and the last time this function was called
 */
void alarm_exec();

void print_alarm();
