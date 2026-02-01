/*
 * Dr SP Sithungu
 * P4 Solution
 * 15 August 2025
 */
module;

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

export module LibUtility;

export struct UJPixel {
    int intRed;
    int intGreen;
    int intBlue;
};

export enum ExitCode {
    SUCCESS,
    ERROR_RANGE,
    ERROR_ARGS,
    ERROR_CONV,
    ERROR
};
