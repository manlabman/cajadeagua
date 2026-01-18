/* 
 *  Hematological Counter for PIC16F877A
 *  8 Counting Keys, Reset, Function Mode, 3x7-Segment Displays
 */

// PIC16F877A Configuration Bit Settings
__CONFIG(FOSC_XT & WDTE_OFF & PWRTE_ON & BOREN_ON & LVP_OFF & CPD_OFF & WRT_OFF & CP_OFF);

// Clock frequency for delay functions
#define _XTAL_FREQ 4000000

// Display configuration - ADJUST based on your hardware (Common Cathode example)
const unsigned short SEGMENT_PATTERNS[] = {
  0x3F, // 0
  0x06, // 1
  0x5B, // 2
  0x4F, // 3
  0x66, // 4
  0x6D, // 5
  0x7D, // 6
  0x07, // 7
  0x7F, // 8
  0x67  // 9
};

// Global Variables
unsigned int keyCount[8] = {0};      // Counts for keys 0-7
unsigned long totalPulses = 0;       // Total pulses across all keys
unsigned char displayMode = 0;       // 0=Show counts, 1=Show percentages
unsigned char displayDigits[3];      // Digits to display (hundreds, tens, units)

// Function Prototypes
void initPorts(void);
unsigned char scanKeys(void);
void updateDisplayData(unsigned char keyIndex);
void displayNumber(unsigned int number);
void multiplexDisplays(void);
void calculatePercentage(unsigned char keyIndex);

// Main Function
void main() {
  unsigned char keyPressed;
  unsigned char lastKey = 0xFF;      // For debounce tracking
  
  initPorts();                       // Configure I/O ports
  
  while(1) {
    keyPressed = scanKeys();         // Scan the key matrix
    
    if(keyPressed != 0xFF && keyPressed != lastKey) {
      __delay_ms(20);                // Debounce delay [citation:6]
      
      if(keyPressed == scanKeys()) { // Verify key is still pressed
        if(keyPressed < 8) {         // It's a counting key (0-7)
          keyCount[keyPressed]++;    // Increment specific counter
          totalPulses++;             // Increment total pulses
          updateDisplayData(keyPressed); // Prepare data for display
        }
        else if(keyPressed == 8) {   // RESET key (assigned index 8)
          // Reset all counts
          for(unsigned char i=0; i<8; i++) keyCount[i] = 0;
          totalPulses = 0;
          displayDigits[0] = 0;      // Show 000 on reset
          displayDigits[1] = 0;
          displayDigits[2] = 0;
        }
        else if(keyPressed == 9) {   // FUNCTION key (assigned index 9)
          // Toggle between counts and percentages
          displayMode = !displayMode;
          if(displayMode == 0) {
            // When switching to count mode, show first key's count
            updateDisplayData(0);
          } else {
            // When switching to percentage mode, show first key's percentage
            calculatePercentage(0);
          }
        }
      }
      lastKey = keyPressed;
    }
    else if(keyPressed == 0xFF) {
      lastKey = 0xFF;                // Reset when no key is pressed
    }
    
    multiplexDisplays();             // Continuously refresh displays
  }
}

// Initialize I/O Ports
void initPorts(void) {
  TRISA = 0x00;      // PORTA as output (display selection)
  TRISB = 0x00;      // PORTB as output (segment data)
  TRISC = 0xF0;      // PORTC: RC0-RC3 as output (rows), RC4-RC7 as input (cols) [citation:1]
  TRISD = 0x03;      // PORTD: RD0-RD1 as input (optional for separate reset/function keys)
  
  PORTA = 0x00;
  PORTB = 0x00;
  PORTC = 0x00;
  PORTD = 0x00;
}

// Scan 4x3 Key Matrix (8 counting + 2 control keys)
unsigned char scanKeys(void) {
  unsigned char row, col, keyValue = 0;
  
  for(row=0; row<3; row++) {           // Scan 3 rows
    PORTC = ~(1 << row);               // Activate one row at a time
    
    __delay_us(100);                   // Small stabilization delay
    
    // Read columns
    col = (PORTC >> 4) & 0x0F;
    
    // Check which column is active (low)
    if(!(col & 0x01)) return keyValue;     // Col 0
    keyValue++;
    if(!(col & 0x02)) return keyValue;     // Col 1
    keyValue++;
    if(!(col & 0x04)) return keyValue;     // Col 2
    keyValue++;
    if(!(col & 0x08)) return keyValue;     // Col 3
    keyValue++;
  }
  
  return 0xFF;  // No key pressed
}

// Prepare display data based on current mode
void updateDisplayData(unsigned char keyIndex) {
  if(displayMode == 0) {
    // Show raw count for the pressed key
    displayNumber(keyCount[keyIndex]);
  } else {
    // Show percentage for the pressed key
    calculatePercentage(keyIndex);
  }
}

// Format a number into three digits
void displayNumber(unsigned int number) {
  if(number > 999) number = 999;  // Cap at 999 for 3 digits
  
  displayDigits[0] = number / 100;                // Hundreds
  displayDigits[1] = (number % 100) / 10;         // Tens
  displayDigits[2] = number % 10;                 // Units
}

// Calculate and format percentage for a key
void calculatePercentage(unsigned char keyIndex) {
  unsigned int percentage = 0;
  
  if(totalPulses > 0) {
    // Calculate percentage: (count/total)*100
    percentage = (unsigned int)(((float)keyCount[keyIndex] / totalPulses) * 100.0);
  }
  
  displayNumber(percentage);  // Show as 3-digit number (0-100)
}

// Multiplex three 7-segment displays
void multiplexDisplays(void) {
  unsigned char displayIndex;
  
  for(displayIndex=0; displayIndex<3; displayIndex++) {
    PORTA = (1 << displayIndex);        // Select current display [citation:7]
    PORTB = SEGMENT_PATTERNS[displayDigits[displayIndex]];  // Send segment pattern [citation:6]
    
    __delay_ms(3);                      // Display ON time
    
    PORTB = 0x00;                       // Clear segments before switching display
  }
}
