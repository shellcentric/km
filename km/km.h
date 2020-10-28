/**
 Project: Kookie Monster, a cookie file eating program for Apple macOS
 File: km.h
 Author: Christopher Cover https://www.shellcentric.com/
 Copyright: Copyright (c) 2016 Christopher Cover.
 License: MIT
 Date: May 6, 2016
 Version: 0.9.3

 The MIT License (MIT)
 Copyright (c) 2016 Christopher Cover.

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to
 deal in the Software without restriction, including without limitation the
 rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 IN THE SOFTWARE.
 */

#ifndef km_h
#define km_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#include "glob_find.h"

/**
 Creates the ~/.config/km/ directory path and the ~/.config/km/cookies file.
 Exits with error code and message on failure.

 @brief Creates app data storage file and directories with intermediates.
 */
static void mkconfig()
{
    char * dir_home = getenv("HOME");   // ~
    char * dir_conf;                    // ~/.config/
    char * dir_self;                    // ~/.config/km/
    char * config_f;                    // ~/.config/km/cookies

    if (dir_home == NULL) err(1, "Environment contains no value for HOME.");

    asprintf(&dir_conf, "%s%s", dir_home, "/.config");
    asprintf(&dir_self, "%s%s", dir_conf, "/km");
    asprintf(&config_f, "%s%s", dir_self, "/cookies");

    if (access(config_f, R_OK|W_OK) != 0) {
        if (access(dir_conf, R_OK|X_OK) != 0) mkdir(dir_conf, 0755);
        if (access(dir_self, R_OK|X_OK) != 0) mkdir(dir_self, 0755);

        FILE *fp = fopen(config_f, "wb");
        fclose(fp);

        if (access(config_f, R_OK|W_OK) != 0)
            err(errno, "Could not create '%s'.", config_f);
    }
}

/**
 @brief strips commented entities from colon-delimited input "string"
 @remarks the recognized comment mark is hash or pound (#)
 @param input_string the "string" from which to remove commented entities
 @return a "string" of commentless, colon-delimited values or NULL
 */
static char * strip_comments(char * input_string)
{
    char * token;
    char * retval = "";
    int i = 0;

    while ((token = strsep(&input_string, ":"))) {
        if ((token[0] != '#') && (i == 0)) {
            asprintf(&retval, "%s:", token);
        } else if (token[0] != '#') {
            asprintf(&retval, "%s%s:", retval, token);
        }
        i++;
    }

    if (strlen(retval) == 1) retval = "";
    return retval;
}

/**
 @brief reads file and replaces occurances of new lines with colons
 @param int whether to strip comments
 @return a "string" of colon-delimited values or NULL
 */
static char * read_saved(int with_comments)
{
    char * dir_home = getenv("HOME");
    char * config_f;
    asprintf(&config_f, "%s%s", dir_home, "/.config/km/cookies");

    if (access(config_f, R_OK) != 0)
        err(1, "Cannot access '%s'.", config_f);

    FILE * fp = fopen(config_f, "rb");
    fseek(fp, 0, SEEK_END);
    long pos = ftell(fp);
    rewind(fp);

    char * contents = malloc(pos + 1);
    fread(contents, pos, 1, fp);
    fclose(fp);

    for (int i = 0; i < strlen(contents); i++) {
        if (contents[i] == '\n') {
            contents[i] = ':';
        }
    }

    if (with_comments == 1) return contents;
    return strip_comments(contents);
}

/**
 @brief Returns a "string" containing the differnces between "strings" a and b
 @remarks delimiter is colon
 @param lhs_saved a "string" representing contents of a file
 @param rhs_found a "string" representing contents of filesystem search
 @return a "string" containing elements from rhs_found not found in lhs_saved
 */
static char * difference(char * lhs_saved, char * rhs_found)
{
    char * token;
    char * d = ""; // difference

    while ((token = strsep(&rhs_found, ":"))) {
        if (! strstr(lhs_saved, token)) asprintf(&d, "%s%s:", d, token);
    }

    if (strlen(d) == 1) d = "";
    return d;
}

/**
 @param string a colon-delimited "string" of values
 @return number of colon characters in a "string"
 */
static int num_entities(char string[]) {
    unsigned long length = strlen(string);
    int c = 0, i = 0;

    for (i = 0; i < length; i++) {
        if (string[i] == ':') c++;
    }

    return c;
}

/**
 @brief replaces char a with char b in "string" string
 @param string a "string" to apply character replacement
 @param a the character to replace
 @param b the character to use in place of a
 @return transformed string
 */
static char * replace_delimiter(char * string, char a, char b)
{
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] == a) string[i] = b;
    }
    return string;
}

/**
 @brief appends path of newly found cookie files to app storage file
 @param string a "string" representing contents of a file
 */
static void append_new(char * found)
{
    char * dir_home = getenv("HOME");
    char * path;

    asprintf(&path, "%s%s", dir_home, "/.config/km/cookies");
    found = replace_delimiter(found, ':', '\n');

    FILE *fp;

    fp = fopen(path, "a");
    if (fp == NULL) err(errno, "Could not get file handle for '%s'.", path);
    fprintf(fp, "%s", found);
    fclose(fp);
}

/**
 @brief prints version to stdout
 */
void show_version()
{
    puts("This is Kookie Monster 0.9.3.");
}

/**
 @brief prints help to stdout
 */
void show_help()
{
    puts("Usage: km [-f, -d, -h, -v]");
    puts("    -f: find and remember cookie files in the home directory of the current user");
    puts("    -e: eat (delete) cookie files specified by a previous find ('-f') operation");
    puts("    -h: show (this) help");
    puts("    -v: show version");
    puts("If '-f' finds new cookie files, examine ~/.config/km/cookies and comment (#) any");
    puts("references to files that should not be deleted when run with '-e'.");
}

/**
 @brief find cookie files that do not exist in application storage file and
        write results to application storage file
 */
void find_cookie_files()
{
    mkconfig();

    char * dir_home = getenv("HOME");
    char * lhs_saved = read_saved(1);
    char * rhs_found = glob_find(dir_home, "*.binarycookies");
    char * new_files = difference(lhs_saved, rhs_found);

    int count = num_entities(new_files);
    if (count > 0) {
        append_new(new_files);

        char * config_f;
        asprintf(&config_f, "%s%s", dir_home, "/.config/km/cookies");
        printf("Found %i new files.\nPlease edit '%s'.\n", count, config_f);
        puts("Comment (#) files that should remain when run with '-e'.");
    } else {
        puts("None new found.");
    }
}

/**
 @brief delete files specified in app storage file
 */
void delete_cookie_files()
{
    char * dir_home = getenv("HOME");
    char * config_f;
    asprintf(&config_f, "%s%s", dir_home, "/.config/km/cookies");
    char * to_delete = read_saved(0);
    char * token;

    while ((token = strsep(&to_delete, ":"))) {
        if (strlen(token) > 0) remove(token);
    }

}

#endif /* km_h */
