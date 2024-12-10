/* This program uses inputs from buttons and reed switch sensors
 * and outputs to status and alarm LEDs to create an home
 * security system.
 */

// INSTRUCTIONS FOR 2-PERSON TEAM:
// Person A needs to type parts 1, 3, and 5.
// Person B needs to type parts 2, 4, and 6.

// INSTRUCTIONS FOR 3-PERSON TEAM:
// Person A needs to type parts 1 and 4.
// Person B needs to type parts 2 and 5.
// Person C needs to type parts 3 and 6.


// GPIO Port struct
typedef struct{
	unsigned int MODER;   // offset: 0x00
	unsigned int OTYPER;  // offset: 0x04
	unsigned int OSPEEDR; // offset: 0x08
	unsigned int PUPDR;   // offset: 0x0C
	unsigned int IDR;     // offset: 0x10
	unsigned int ODR;     // offset: 0x14
} Port;

// Address of the Advanced High-performance Bus 2 Enable Register
unsigned int* AHB2ENR = (unsigned int*) 0x4002104c;

// Base addresses of the GPIO Port control registers (SFRs)
Port* GPIOA = (Port*) 0x48000000; // base address of GPIOA
Port* GPIOB = (Port*) 0x48000400; // base address of GPIOB
Port* GPIOC = (Port*) 0x48000800; // base address of GPIOC

/* Part 1. Typed by John. */

// input pins
#define ArmButtonPin    13 // PC13
#define DisarmButtonPin 9  // PC9
#define FrontSensorPin  10  // PC10
#define BackSensorPin   11  // PC11
#define WindowSensorPin 12  // PC12

// output pins
#define ArmStatusPin    5  // PA5
#define FrontAlarmPin   6  // PA6
#define BackAlarmPin    7  // PA7
#define WindowAlarmPin  8  // PA8


int main(void)
{
	// Turn on GPIO clocks (Ports A, B, and C)
    *AHB2ENR |= 0b111;


    /* Part 2. Typed by Joshua. */

    // Configure button pin as inputs
    GPIOC->MODER &= ~(0b11 << (ArmButtonPin*2));    // configure ArmButtonPin as input
    GPIOC->MODER &= ~(0b11 << (DisarmButtonPin*2)); // configure DisarmButtonPin as input

    // Configure reed switch pins as inputs
    GPIOC->MODER &= ~(0b11 << (FrontSensorPin*2));; // configure FrontSensorPin as input
    GPIOC->MODER &= ~(0b11 << (BackSensorPin*2));; // configure BackSensorPin as input
    GPIOC->MODER &= ~(0b11 << (WindowSensorPin*2));; // configure WindowSensorPin as input

    /* Part 3. Typed by Brenden. */

    // Enable pull resistors
    GPIOC->PUPDR |= (0b01 << (DisarmButtonPin*2));
    GPIOC->PUPDR |= (0b01 << (FrontSensorPin*2));
    GPIOC->PUPDR |= (0b01 << (BackSensorPin*2));
    GPIOC->PUPDR |= (0b01 << (WindowSensorPin*2));

    /* Part 4. Typed by John. */

    // Configure LED pins as outputs
    GPIOA->MODER &= ~(0b10 << (ArmStatusPin*2)); // configure ArmStatusPin as output       WATCH
    GPIOA->MODER &= ~(0b10 << (FrontAlarmPin*2)); // configure FrontAlarmPin as output
    GPIOA->MODER &= ~(0b10 << (BackAlarmPin*2)); // configure BackAlarmPin as output
    GPIOA->MODER &= ~(0b10 << (WindowAlarmPin*2)); // configure WindowAlarmPin as output

    // Initialize Alarm Status (turn off all LEDs)
    GPIOA->ODR &= ~(1 << ArmStatusPin);
    GPIOA->ODR &= ~(1 << FrontAlarmPin);
    GPIOA->ODR &= ~(1 << BackAlarmPin);
    GPIOA->ODR &= ~(1 << WindowAlarmPin);

    // armed state (initialize to disarm)
    int armed = 0;

    // infinite loop
	while(1){

		/* Part 5. Typed by Joshua. */

		// check if ArmButton is pressed
		if ((GPIOC->IDR & (1 << ArmButtonPin)) == 0){
			// arm system
			armed = 1; // set arm state
			GPIOA->ODR |= (1 << ArmStatusPin); // turn on ArmStatus LED
		}

		// check if DisarmButton is pressed
		if ((GPIOC->IDR & (1 << DisarmButtonPin)) == 0){
			// disarm system
			armed = 0; // reset arm state
			GPIOA->ODR &= ~(1 << ArmStatusPin); // turn off ArmStatus LED
			GPIOA->ODR &= ~(1 << FrontAlarmPin); // turn off FrontAlarm LED
			GPIOA->ODR &= ~(1 << BackAlarmPin); // turn off BackAlarm LED
			GPIOA->ODR &= ~(1 << WindowAlarmPin); // turn off WindowAlarm LED
		}

		/* Part 6. Typed by __Brenden Read__. */

		// if system is armed, check sensors
		if (armed){

			// check the FrontSensor
			if ((GPIOC->IDR & (1 << FrontSensorPin)) != 0){
				// system armed & front door is open:
				GPIOA->ODR |= (1 << FrontAlarmPin); // turn on FrontAlarm LED
			}

			// check the BackSensor
			if ((GPIOC->IDR & (1 << BackSensorPin)) != 0){
				// system armed & front door is open:
				GPIOA->ODR |= (1 << BackAlarmPin); // turn on BackAlarm LED
			}

			// check the WindowSensor
			if ((GPIOC->IDR & (1 << WindowSensorPin)) != 0)
				// system armed & front door is open:
				GPIOA->ODR |= (1 << WindowAlarmPin);; // turn on WindowAlarm LED
			}
		}
	}
