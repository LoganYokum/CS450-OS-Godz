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

int dtoh(int dec);

int htod(int hex);

int isnum(unsigned char args);
/**
*/
int println(const char *message);
#endif
