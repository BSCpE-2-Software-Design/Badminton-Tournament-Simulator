#include <iostream>
#include "Court.h"

int main() {
    
    Court tennisCourt;

  
    std::cout << "Tennis Court Dimensions:" << std::endl;
    std::cout << "Left: " << tennisCourt.left << std::endl;
    std::cout << "Right: " << tennisCourt.right << std::endl;
    std::cout << "Top: " << tennisCourt.top << std::endl;
    std::cout << "Bottom: " << tennisCourt.bottom << std::endl;
    std::cout << "Net X position: " << tennisCourt.netX << std::endl;
    std::cout << "Net Height: " << tennisCourt.netHeight << std::endl;

    
    float width = tennisCourt.right - tennisCourt.left;
    float height = tennisCourt.bottom - tennisCourt.top;

    std::cout << "\nCourt size: " << width << "x" << height << std::endl;

    return 0;
}