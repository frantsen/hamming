/* Rachel Frantsen, 17. Oct 2015
   For Hamming Code Programming Project, CS241: Hardware Design at St. Olaf College
   Converts a hexidecimal, octal, or decimal number to its Hamming-encoded equivalent
   Corrects a Hamming-encoded number with a single-bit error
*/


#include <iostream>
using namespace std;

// Return true if index points to a parity bit
bool isParityIdx(short i) {
  if (i==0 || i==1 || i==3 || i==7 || i==15 || i==31) // Indeces of parity bits
    return true;
  else
    return false;
}

// Accepts long int value, returns size of data bit array
short bitsData(long val) {
  short res = 0;
  int i = 1;
  while (i < val+1) {
    res++;
    i *= 2;
  }
  return res;
}

// Accepts long int value, returns size of Hamming code array
short bitsHamming(long val) {
  short db = bitsData(val);
  short pb;
  
  if (db < 65) pb = 7;
  if (db < 50) pb = 6;
  if (db < 28) pb = 5;
  if (db < 12) pb = 4;
  return db + pb;
}

// Value from array of bit values
long binValue(bool bin[], short len) {
  long res = 0;
  int pow = 1;
  
  for (int i = len-1; i >= 0; i--) {
    res += bin[i] * pow;
    pow *= 2;
  }
  return res;
}

// Converts long int to binary array
void binConvert(long val, bool bin [], int len) {
  for (int i = 0; i < len; i++) {
    bin[i] = (val >> (len-1-i)) & 1;
  }
}


// Accepts long int value, returns its Hamming code equivalent as long int
long hamming(long val) {
  short dl = bitsData(val);
  short hl = bitsHamming(val);

  // Store data bits for better access
  bool aval[dl];
  binConvert(val, aval, dl);

  // Create hamming code with empty parity bits
  bool code[hl];
  int vi = 0;
  for (int i = 0; i < hl; i++) {
    if (isParityIdx(i))  // It's a parity bit!
      code[i] = 0;
    else {  // It's a data bit!
      code[i] = aval[vi];
      vi++;
    }
  }

  // Calculate parity for each parity bit
  for (int i = 1; i < hl; i *= 2) {
    bool p = false;  // Flag for bit parity
    int pjump = 0;
    for (int ic = i-1; ic < hl; ic++) {
      if (code[ic] == 1)
	p = !p;
      if (++pjump == i) {
	ic += pjump;  // Skip to next group of bits
	pjump = 0;
      }
    }
    if (i < hl)
      code[i-1] = (short int)p;  // Insert parity bit
  }

  long res = binValue(code, hl);
  return res;
}


// Accepts long int (Hamming-encoded value with single-bit error) and returns the corrected value
long hammingCorrect(long err) {
  short l = bitsData(err);
  bool code[l];
  binConvert(err, code, l);  // Fill code array with binary version of err

  // For each parity bit, calculate correct parity and compare (comments indicate differences from Hamming encoder)
  short ierr = 0;  // Contains i'th position of the error
  for (int i = 1; i < l; i *= 2) {
    bool p = false;
    int pjump = 0;
    for (int id = i-1; id < l; id++) {
      if (code[id] == 1) {
	if (not isParityIdx(id)) // Don't count the parity bits!
	  p = !p;
      }
      if (++pjump == i) {
	id += pjump;
	pjump = 0;
      }
    }
    // Calculate the i'th position of the error 
    if (code[i-1] != p) {  // Parity bit doesn't match predicted value
      if (i-1 == 0) ierr++;
      if (i-1 == 1) ierr += 2;
      if (i-1 == 3) ierr += 4;
      if (i-1 == 7) ierr += 8;
      if (i-1 == 15) ierr += 16;
    }
  }

  if (ierr > 0)  // If there is an error, then...
    code[ierr-1] = !code[ierr-1];  // ...flip bit where the error lies
  
  long res = binValue(code, l);
  return res;
}



// Simple user interface allows multiple conversions in one program execution; no return value
void ui() {

  short choice;
  unsigned long int doh;

  cout << "Would you like to encode a 1) decimal 2) octal or 3) hexidecimal value, or correct a 4) decimal 5) octal or 6) hexidecimal Hamming-encoded value?" << endl;
  cin >> choice;
  if (choice == 1) {
    cout << "\nEnter a valid, positive decimal value up to 67,108,863: ";
    cin >> dec >> doh;
    cout << "\nHamming-encoded equivalent: " << dec << hamming(doh) << endl;
  }
  else if (choice == 2) {
    cout << "\nEnter a valid, positive octal value up to 1777,7777: ";
    cin >> oct >> doh;
    cout << "\nHamming-encoded equivalent: " << oct << hamming(doh) << endl;
  }
  else if (choice == 3) {
    cout << "\nEnter a valid, positive hexidecimal value up to 3ff,ffff: ";
    cin >> hex >> doh;
    cout << "\nHamming-encoded equivalent: " << hex << hamming(doh) << endl;
  }
  else if (choice == 4) {
    cout << "\nEnter a valid, positive decimal value up to 4,294,967,295: ";
    cin >> dec >> doh;
    cout << "\nCorrected hamming code: " << dec << hammingCorrect(doh) << endl;
  }
  else if (choice == 5) {
    cout << "\nEnter a valid, positive octal value up to 377,7777,7777: ";
    cin >> oct >> doh;
    cout << "\nCorrected hamming code: " << oct << hammingCorrect(doh) << endl;
  }
  else if (choice == 6) {
    cout << "\nEnter a valid, positive hexidecimal value up to ffff,ffff: ";
    cin >> hex >> doh;
    cout << "\nCorrected hamming code: " << hex << hammingCorrect(doh) << endl;
  }
  else {
    cout << "\nPlease enter a valid option.\n" << endl;
    ui();
  }
  
  cout << "\nTry another value? 0) No, 1) Yes" << endl;
  cin >> choice;
  cout << endl;
  if (choice == 1)
    ui();
}



// Main function
int main() {
  
  cout << "\nWelcome to RFHamming 1.3.\n\n";
  ui();
  
  return 0;
}
