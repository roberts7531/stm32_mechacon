#ifndef CONFIG_H
#define CONFIG_H
extern int VERBOSE;
extern char* mgkeys;

/**
 * @brief returns path to ps3mca inside `$HOME` or `%USERPROFILE%`
 */
extern
const char* get_globalconf();

/**
 * @brief loads and parses config for ps3mca
 */
extern
int loadconfig(const char* source);

#endif
