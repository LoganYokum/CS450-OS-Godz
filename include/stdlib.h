#ifndef MPX_STDLIB_H
#define MPX_STDLIB_H

/**
 @file stdlib.h
 @brief A subset of standard C library functions.
*/

/**
 Convert an ASCII string to an integer
 @param s A NUL-terminated string
 @return The value of the string converted to an integer
*/
int atoi(const char *s);

char *itoa(int n) ;

/**
 Convert an integer to an ASCII string
 @param n An arbitrary integer
 @return A pointer to a NULL-terminated string
 */
char *itoa(int n);

/**
 * Converts an integer into a BCD integer
 * @param dec 
 * @return BCD integer of dec 
 */
int dtoBCD(int dec);

/**
 * Converts a BCD integer into an integer
 * @param bcd 
 * @return integer of bcd 
 */
int BCDtod(int bcd);

/**
 * Converts an integer into a hexidecial string
 * @param dec 
 * @return hex string of dec
 */
char *dtoh(int dec);

/**
 * Converts hexidecimal string into an integer
 * @param hex 
 * @return integer of hex 
 */
int htod(const char *hex);

int isnum(unsigned char args);

/**
 * Determines if a string is a valid number
 * @param s A NUL-terminated string
 * @return 1 if the string is a valid number, 0 otherwise
 */
int validnum(const char *s);

/**
 * Outputs an error string to the console in red
 * @param message the error message to print
 */
void error(const char *message);

/**
 * Outputs a success string to the console in green
 * @param message the success message to print
 */
void success(const char *message);

/**
 * Outputs an alarm message to the console in yellow
 * @param message the alarm message to print
 */
void alarm_output(const char *message);
#endif
