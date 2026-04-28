#!/usr/bin/env python3

import sys
import datetime

def main():
    for line in sys.stdin:
        line = line.strip()
        if line == "date":
            x = datetime.datetime.now()
            print(x) 
        elif line == "version":
            print("{}.{}.{}".format(sys.version_info[0],sys.version_info[1],sys.version_info[2]))

if __name__ == '__main__':
    main()
