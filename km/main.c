/**
 Project: Kookie Monster, a cookie file eating program for Apple macOS
 File: main.c
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

#include <string.h>
#include "km.h"

int main(int argc, const char * argv[]) {
    if (argc != 2 || strlen(argv[1]) > 2) {
        show_help();
        exit(1);
    }
    
    const char option = argv[1][1];
    
    if (option == 'h') {
        show_help();
    } else if (option == 'f') {
        find_cookie_files();
    } else if (option == 'e') {
        delete_cookie_files();
    } else if (option == 'v') {
        show_version();
    } else {
        show_help();
    }
    
    return 0;
}
