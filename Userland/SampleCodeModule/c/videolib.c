#include <videolib.h>
#include <stdint.h>
void drawRectangle(uint32_t color, uint16_t up_l_x, uint16_t up_l_y, uint16_t lo_r_y, uint16_t lo_r_x)
{
    Rectangle rect = {up_l_x, up_l_y, lo_r_x, lo_r_y, color};
    syscall(5, &rect, 0, 0);
}

void clearScreen(){
    syscall(8,0,0,0);
}