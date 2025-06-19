#include <time_and_rtc.h>
#include <stdint.h>
#include <processManager.h>

static unsigned long ticks = 0;
static int frequency = 18;

extern void outb(uint16_t port, uint8_t value);


#define SECONDS_REG 0x00
#define MINUTES_REG 0x02
#define HOURS_REG 0x04
#define DAY_REG 0x07
#define MONTH_REG 0x08
#define YEAR_REG 0x09
#define GMT_OFFSET -3
#define NMI_DISABLE_BIT 1
#define REG_NEEDED 6
#define PIT_FREQ 1193182

void timer_handler()
{
	ticks++;
}

int ticks_elapsed()
{
	return ticks;
}

int seconds_elapsed()
{
	return ticks / frequency;
}
void sleep(uint32_t ticksToWait)
{
	int ticksAtCallTime = ticks_elapsed();
	while (ticks - ticksAtCallTime < ticksToWait)
		;
}
void setTickFrequency(uint16_t freq)
{
	frequency = freq;
	int ticks = PIT_FREQ / freq;
	outb(0x43, 0xb6);
	outb(0x40, (uint8_t)(ticks));
	outb(0x40, (uint8_t)(ticks >> 8));
}

static uint8_t bcd_decimal(uint8_t hex)
{
	int dec = ((hex & 0xF0) >> 4) * 10 + (hex & 0x0F);
	return dec;
}

static int isLeap(uint16_t year)
{
	return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

// Recibe un TimeStamp* y debe dejar allí año, mes, día, hora, minutos, segundos
void getTime(TimeStamp *ts)
{
	uint8_t daysPerMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	uint8_t regNeeded[] = {SECONDS_REG, MINUTES_REG, HOURS_REG, DAY_REG, MONTH_REG, YEAR_REG};
	uint8_t data[REG_NEEDED];
	for (int i = 0; i < REG_NEEDED; i++)
	{
		outb(0x70, (NMI_DISABLE_BIT << 7) | (regNeeded[i]));
		data[i] = inb(0x71);
	}
	ts->seconds = bcd_decimal(data[0]);
	ts->minutes = bcd_decimal(data[1]);
	ts->hours = bcd_decimal(data[2]) + GMT_OFFSET;
	if (ts->hours < 0)
		ts->hours += 24;
	ts->day = bcd_decimal(data[3] - ((ts->hours > 21) ? 1 : 0));
	ts->month = bcd_decimal(data[4]);
	ts->year = bcd_decimal(data[5]);
	if (ts->day == 0)
	{
		if (ts->month == 1)
		{
			ts->month = 12;
			ts->year--;
		}
		else
		{
			ts->month--;
		}
		ts->day = daysPerMonth[ts->month - 1] + (ts->month == 2) * isLeap(ts->year);
	}
}