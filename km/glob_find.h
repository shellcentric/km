/**
 Project: Kookie Monster, a cookie file eating program for Apple macOS
 File: glob_find.h
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

#include <err.h>
#include <fnmatch.h>
#include <fts.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#ifndef glob_find_h
#define glob_find_h

/**
 @brief a user-defined function for fts_open() called from glob_find()
 @param a the fts structure that represents file, a
 @param b the fts structure that represents file, b
 @return the integer returned from strcmp()
 */
static int entcmp(const FTSENT **a, const FTSENT **b)
{
    return strcmp((*a)->fts_name, (*b)->fts_name);
}

/**
 @brief finds files that match a shell-style glob
 @remarks colon delimiter is an illegal character on the filesystem
 @param directory the directory from which to start the search
 @param glob the shell-style glob or search pattern
 @return a "string" of colon-delimited files found or NULL
 */
char * glob_find(char * directory, char * glob)
{
    if (access(directory, R_OK|X_OK) != 0)
        err(errno, "Cannot read '%s'.", directory);
    
    char * d[] = {directory, NULL};
    char * retval = NULL;
    
    FTS * tree;
    FTSENT * f;
    tree = fts_open(d, FTS_LOGICAL|FTS_NOSTAT, entcmp);
    
    printf("Searching for files matching '%s' ...\n", glob);
    
    while ((f = fts_read(tree))) {
        if (fnmatch(glob, f->fts_name, FNM_PERIOD) == 0) {
            if (&retval[0] == NULL) {
                asprintf(&retval, "%s", f->fts_path);
            } else {
                asprintf(&retval, "%s%c%s", retval, ':', f->fts_path);
            }
        }
    }
    
    fts_close(tree);

    return retval;
}

#endif /* glob_find_h */
