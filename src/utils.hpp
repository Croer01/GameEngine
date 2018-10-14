//
// Created by adria on 18/08/2018.
//

#ifndef ROOT_UTILS_HPP
#define ROOT_UTILS_HPP

void _CheckSDLError(int line = -1);

void _CheckGlError(const char *file, int line);

#define CheckGlError() _CheckGlError(__FILE__, __LINE__)
#define CheckSDLError() _CheckSDLError(__LINE__)

#endif //ROOT_UTILS_HPP
