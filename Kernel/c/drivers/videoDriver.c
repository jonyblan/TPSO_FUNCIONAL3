#include <stdint.h>
#include <font.h>
#include <videoDriver.h>

unsigned int position; // ver despues si prefiero x e y
unsigned int x = 0, y = 0;
uint8_t size = SMALL_FONT;

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);

struct vbe_mode_info_structure
{
	uint16_t attributes;  // deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;	  // deprecated
	uint8_t window_b;	  // deprecated
	uint16_t granularity; // deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr; // deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;		   // number of bytes per horizontal line
	uint16_t width;		   // width in pixels
	uint16_t height;	   // height in pixels
	uint8_t w_char;		   // unused...
	uint8_t y_char;		   // ...
	uint8_t planes;
	uint8_t bpp;   // bits per pixel in this mode
	uint8_t banks; // deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size; // deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;

	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;

	uint32_t framebuffer; // physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size; // size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__((packed));

typedef struct vbe_mode_info_structure *VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr)0x0000000000005C00;

static uint64_t getOffset(uint64_t x, uint64_t y)
{
	return (x * ((VBE_mode_info->bpp) / 8)) + (y * VBE_mode_info->pitch);
}

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y)
{
	uint8_t *framebuffer = (uint8_t *)VBE_mode_info->framebuffer;
	uint64_t offset = getOffset(x, y);
	framebuffer[offset] = (hexColor) & 0xFF;
	framebuffer[offset + 1] = (hexColor >> 8) & 0xFF;
	framebuffer[offset + 2] = (hexColor >> 16) & 0xFF;
}

void vdPrint(const char *string)
{
	while (*string)
		vdPrintChar(*string++);
}

void vdPrintChar(char character)
{
	vdPrintCharStyled(character, 0x00FFFFFF, 0);
}

/**
 * charWrite -->> recibe un caracter, printea as-is en la ubicacion correspondiente
 * como en las fuentes de 16x32 tengo 2 chars (representando 16 pixeles) para cada linea horizontal de la fuente
 * itero sobre ambas columnas de chars y a cada una le aplico el masking correspondiente.
 * es posible usar el size como modificador dado que LARGE_FONT tiene exactamente el doble de pixeles
 * de alto y de ancho que SMALL_FONT asi que los pixeles a dibujar seran el doble en cada dimension en cada caracter
 **/
uint64_t vdPrintCharStyled(char c, uint32_t color, uint32_t bgColor)
{ // faltan casos especiales de /n y eso -->> meter en putchar
	// int size = LARGE_FONT;
	if (c == '\n')
	{
		vdNewline();
		return 1;
	}
	if (c == '\b')
	{
		vdDelete();
		return -1;
	}
	if (c == '\t')
	{
		vdPrint("    ");
		return 4;
	}

	if (c <= 31)
		return 0;

	unsigned char *hexData;
	if (size == 1)
		hexData = getCharHexData(c);

	if (y > (SCREEN_HEIGHT_PIXELS / (size * HEIGHT_S) - 2 * MARGIN_SIZE))
	{
		vdScroll();
		y--;
	}

	for (int i = 0; i < WIDTH_S * size; i++)
	{
		for (int j = 0; j < HEIGHT_S * size; j++)
		{
			// 0x80 es un bit en el lugar mas significativo, voy corriendolo hacia la derecha para compararlo con cada pixel a dibujar
			// 1 representa que hay un pixel, 0 que no lo hay
			uint8_t isPresent = (hexData[size * j + i / 8] & (0x80 >> i % 8));
			putPixel(isPresent ? color : bgColor, (x + MARGIN_SIZE) * WIDTH_S * size + i, j + size * HEIGHT_S * (y) + size * MARGIN_SIZE * HEIGHT_S / 2);
		}
	}
	position++;
	// para delimitar correctamente el fin de linea
	if (++x == (SCREEN_WIDTH_PIXELS / (size * WIDTH_S) - 2 * MARGIN_SIZE))
	{
		y++;
		x = 0;
	}

	return 1;
}

void vdPrintStyled(char *string, uint32_t color, uint32_t bgColor)
{
	while (*string)
		vdPrintCharStyled(*string++, color, bgColor);
}

uint64_t vdNPrintStyled(const char *string, uint32_t color, uint32_t bgColor, uint64_t N)
{
	uint64_t i, printed = 0;
	for (i = 0; i < N && string[i] != 0; i++)
		printed += vdPrintCharStyled(string[i], color, bgColor);
	return printed;
}
// TODO CHEQUEO AL FINAL PARA WRAPAROUND?
void vdNewline()
{
	y++;
	x = 0;
}
void vdPrintDec(uint64_t value)
{
	vdPrintBase(value, 10);
}

void vdPrintHex(uint64_t value)
{
	vdPrintBase(value, 16);
}

void vdPrintBin(uint64_t value)
{
	vdPrintBase(value, 2);
}
// TODO VER SI ESTE BUFFER LO SACAMOS DE OTRO LADO
void vdPrintBase(uint64_t value, uint32_t base)
{
	char buffer[20];
	uintToBase(value, buffer, base);
	vdPrint(buffer);
}

void vdClear()
{
	for (int i = 0; i < SCREEN_HEIGHT_PIXELS * SCREEN_WIDTH_PIXELS; i++)
		putPixel(0, i % SCREEN_WIDTH_PIXELS, i / SCREEN_WIDTH_PIXELS);
	x = y = 0;
}
void vdScroll()
{
	uint8_t *framebuffer = (uint8_t *)VBE_mode_info->framebuffer;
	for (int i = SCREEN_WIDTH_PIXELS * (HEIGHT_S * BITS_PER_PIXEL * MARGIN_SIZE) / 2 * size, j = SCREEN_WIDTH_PIXELS * (HEIGHT_S * BITS_PER_PIXEL * (2 + MARGIN_SIZE)) / 2 * size; j < SCREEN_HEIGHT_PIXELS * SCREEN_WIDTH_PIXELS * BITS_PER_PIXEL; i++, j++)
		framebuffer[i] = framebuffer[j];
	for (int i = SCREEN_WIDTH_PIXELS * BITS_PER_PIXEL * (SCREEN_HEIGHT_PIXELS - size * (HEIGHT_S * (2 + MARGIN_SIZE)) / 2); i < SCREEN_HEIGHT_PIXELS * SCREEN_WIDTH_PIXELS * BITS_PER_PIXEL; i++)
		framebuffer[i] = 0;
}
void vdDelete()
{
	if (x == 0)
	{
		y--;
		x = (SCREEN_WIDTH_PIXELS / (WIDTH_S * size) - 2 * MARGIN_SIZE) - 1;
	}
	else
	{
		x--;
	}
	vdPrintChar(' ');
	if (x == 0)
	{
		y--;
		x = (SCREEN_WIDTH_PIXELS / (WIDTH_S * size) - 2 * MARGIN_SIZE) - 1;
	}
	else
	{
		x--;
	}
}

void drawRectangle(uint32_t color, uint16_t up_l_x, uint16_t up_l_y, uint16_t lo_r_x, uint16_t lo_r_y)
{
	// chequeos para no hacer boludeces
	for (int i = up_l_x; i < lo_r_x; i++)
	{
		for (int j = up_l_y; j < lo_r_y; j++)
		{
			putPixel(color, i, j);
		}
	}
}

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	// Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	// Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}
