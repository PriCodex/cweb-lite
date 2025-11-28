#include <stdio.h>
#include <string.h>


const char* DEFAULT_FILE = "index.html";

char *to_path(char *req) {
    char *start, *end;

    // Advance `start` to the first space
    for (start = req; start[0] != ' '; start++) {
        if (!start[0]) {
            return NULL;
        }
    }

    start++; // Skip over the space

    // 👉 FIX #1: Skip the leading '/' if present
    if (start[0] == '/') {
        start++;
    }

    // Advance `end` to the second space
    for (end = start; end[0] != ' '; end++) {
        if (!end[0]) {
            return NULL;
        }
    }

    // 👉 FIX #2: Check if request is too short for "index.html"
    // We need space for: current path + "/" + "index.html" + null terminator
    size_t current_len = end - start;
    size_t needed_len = current_len + 1 + strlen(DEFAULT_FILE) + 1;
    size_t available_len = strlen(req) - (start - req);
    
    if (available_len < needed_len) {
        return NULL;  // Not enough space! 
    }

    // Ensure there's a '/' right before where we're about to copy in "index.html"
    if (end[-1] == '/') {
        end--;
    } else {
        end[0] = '/';
    }

    // Copy in "index. html"
    memcpy(
        end + 1,
        DEFAULT_FILE,
        strlen(DEFAULT_FILE) + 1
    );

    return start;
}

int main() {
    char req1[] = "GET /blog HTTP/1.1\nHost: example.com";
    printf("Should be \"blog/index.html\": \"%s\"\n", to_path(req1));

    char req2[] = "GET /blog/ HTTP/1. 1\nHost: example.com";
    printf("Should be \"blog/index. html\": \"%s\"\n", to_path(req2));

    char req3[] = "GET / HTTP/1.1\nHost: example.com";
    printf("Should be \"index.html\": \"%s\"\n", to_path(req3));

    char req4[] = "GET /blog ";
    printf("Should be \"(null)\": \"%s\"\n", to_path(req4));

    return 0;
}