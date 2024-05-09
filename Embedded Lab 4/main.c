#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <templateEMP.h>
#include <msp430.h>

#define ONE_THOUSAND 1000

// Note the enumeration is done in the reverse order as the msp converts the cannels from top down CH7 -> CH0 therfor they are stored in reverse in the adc arry akak channel 7 array index 0 and so on this is the chep workaround
typedef enum
{
	CHANNEL_7,
	CHANNEL_6,
	CHANNEL_5,
	CHANNEL_4,
	CHANNEL_3,
	CHANNEL_2,
	CHANNEL_1,
	CHANNEL_0,
} ADC_Channel;

typedef enum
{
	MODE_A, // LEFT aligned percentage
	MODE_B, // Binary Display
	MODE_C	//
} PRINT_MODE;

typedef enum
{
	LED_OFF,
	LED_ON,
} LED_STATE;

typedef enum
{
	BUTTON_NONE,
	BUTTON_1,
	BUTTON_2
} Button;

// Function prototypes

void initLEDs();
void setLEDState(uint8_t state);

void initADC(void);
void readADC(uint16_t *adcChannelValues);

void printAdcValues(uint8_t mode, uint16_t value);

uint16_t clampToMax(uint16_t value, uint16_t maxValue);
uint16_t convertToPercentage(uint16_t value);

void rightAlignIntToCharArray(uint16_t num, char *charArray);
void convertToGage(uint16_t value, char *gageStr);
void convertToCapColour(uint16_t value, char *colourStr);

uint16_t adcChannelValues[8];

int main(void)
{
	initMSP();

	initLEDs();
	initADC(); // Initialize ADC
	initButtons();

	while (1)
	{

		readADC(adcChannelValues);

		switch (getPressedButton())
		{
		case BUTTON_2 /* constant-expression */:
			setLEDState(LED_ON);
			printAdcValues(MODE_C, adcChannelValues[CHANNEL_6]);
			setLEDState(LED_OFF);
			break;
		case BUTTON_1 /* constant-expression */:
			printAdcValues(MODE_B, adcChannelValues[CHANNEL_7]);
			break;
		case BUTTON_NONE /* constant-expression */:
			printAdcValues(MODE_A, adcChannelValues[CHANNEL_7]);
			break;
		default:
			break;
		}
	}
}

void initButtons()
{
	// Configure P3.0 and P3.1 as inputs
	P3DIR &= ~(BIT0 | BIT1);

	// Enable pull-up resistors for P3.0 and P3.1
	P3REN |= BIT0 | BIT1;
	P3OUT |= BIT0 | BIT1;
}

uint8_t getPressedButton()
{
	if ((P3IN & BIT0) == 0)
	{
		return BUTTON_1;
	}
	else if ((P3IN & BIT1) == 0)
	{
		return BUTTON_2;
	}
	else
	{
		return BUTTON_NONE;
	}
}

void initLEDs()
{
	// Configure P3.2 and P3.3 as outputs
	P3DIR |= BIT2 + BIT3;

	// Disable both LEDs (P3.2 and P3.3)
	P3OUT &= ~(BIT2 + BIT3);
}

void setLEDState(uint8_t state)
{
	if (state == LED_ON)
	{
		P3OUT |= BIT2 + BIT3; // Enable both LEDs (P3.2 and P3.3)
	}
	else
	{
		P3OUT &= ~(BIT2 + BIT3); // Disable both LEDs (P3.2 and P3.3)
	}
}

// Function to initialize ADC
void initADC(void)
{
	ADC10CTL1 = INCH_7 + ADC10DIV_0 + CONSEQ_3 + SHS_0;
	ADC10CTL0 = SREF_0 + ADC10SHT_2 + MSC + ADC10ON; // ADC10IE
	ADC10AE0 = BIT7 + BIT6 + BIT5 + BIT4 + BIT3 + BIT0;
	ADC10DTC1 = 8;
}

void readADC(uint16_t *adcChannelValues)
{
	ADC10CTL0 &= ~ENC;
	while (ADC10CTL1 & BUSY)
		;
	ADC10CTL0 |= ENC + ADC10SC;

	ADC10SA = (uint16_t)adcChannelValues;
}

void printAdcValues(uint8_t mode, uint16_t value)
{
	char temp[25];
	char tempNumericStrn[25]; // Array to store right-aligned print value
	char tempDisplayStrn[25]; // Array to store right-aligned percent value
	uint16_t numericValue = clampToMax(value, ONE_THOUSAND);
	uint16_t percentValue = convertToPercentage(numericValue);

	switch (mode)
	{
	case MODE_A:
		// Right-align print value
		rightAlignIntToCharArray(numericValue, tempNumericStrn);
		// Right-align percent value
		rightAlignIntToCharArray(percentValue, tempDisplayStrn);
		// Print both values
		sprintf(temp, "ADC: %s --> %s%%", tempNumericStrn, tempDisplayStrn);
		serialPrintln(temp);
		break;
	case MODE_B:
		// Right-align print value
		rightAlignIntToCharArray(numericValue, tempNumericStrn);
		// Right-align percent value
		convertToGage(numericValue, tempDisplayStrn);
		// Print both values
		sprintf(temp, "ADC: %s --> %s", tempNumericStrn, tempDisplayStrn);
		serialPrintln(temp);
		break;
	case MODE_C:
		// Right-align print value
		rightAlignIntToCharArray(numericValue, tempNumericStrn);
		// Right-align percent value
		convertToCapColour(numericValue, tempDisplayStrn);
		// Print both values
		sprintf(temp, "ADC: %s --> %s", tempNumericStrn, tempDisplayStrn);
		serialPrintln(temp);
		break;
	default:
		break;
	}
}

uint16_t clampToMax(uint16_t value, uint16_t maxValue)
{
	if (value > maxValue)
	{
		return maxValue;
	}
	return value;
}

uint16_t convertToPercentage(uint16_t value)
{
	return (uint16_t)(((double)value / 1000.0) * 100.0);
}

void rightAlignIntToCharArray(uint16_t num, char *charArray)
{
	// Convert integer to string
	char numStr[5]; // Need space for the null terminator
	snprintf(numStr, sizeof(numStr), "%d", num);

	// Get the length of the integer string
	uint16_t numStrLen = strlen(numStr);

	// Calculate the number of spaces to add before the integer
	uint16_t numSpaces = 4 - numStrLen;

	// Fill the character array with spaces followed by the integer string
	uint16_t i = 0;
	while (i < numSpaces)
	{
		charArray[i++] = ' ';
	}

	uint16_t j = 0;
	while (j < numStrLen)
	{
		charArray[i++] = numStr[j++];
	}

	// Add null terminator
	charArray[4] = '\0';
}

void convertToGage(uint16_t value, char *gageStr)
{
	if (value > 800 && value <= 1000)
	{
		strcpy(gageStr, " 1111"); // Binary representation of 1 1 1 1
	}
	else if (value > 600 && value <= 800)
	{
		strcpy(gageStr, " 1110"); // Binary representation of 1 1 1 0
	}
	else if (value > 400 && value <= 600)
	{
		strcpy(gageStr, " 1100"); // Binary representation of 1 1 0 0
	}
	else if (value > 200 && value <= 400)
	{
		strcpy(gageStr, " 1000"); // Binary representation of 1 0 0 0
	}
	else if (value >= 0 && value <= 200)
	{
		strcpy(gageStr, " 0000"); // Binary representation of 0 0 0 0
	}
	else
	{
		// Handle out-of-range values
		strcpy(gageStr, ""); // Empty string
	}
}

void convertToCapColour(uint16_t value, char *colourStr)
{

	if (value > 200 && value <= 1000) // Red cap range (155 ± 5)
	{
		strcpy(colourStr, "None"); // No cap
	}
	else if (value > 100 && value <= 200) // White cap range (150 ± 5)
	{
		strcpy(colourStr, "White"); // White cap
	}
	else if (value > 0 && value <= 100) // Black cap range (60 ± 5)
	{
		strcpy(colourStr, "Black"); // Black cap
	}
	else // Error
	{
		strcpy(colourStr, "ERROR");
	}
}
