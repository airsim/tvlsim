// STL
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <streambuf>
// Boost


// ////////////////// M A I N ////////////////////////
int main (int argc, char* argv[]) {

  // Input file name
  const std::string lInputFilename ("filetoberead.csv");

  // Open the input file
  std::ifstream lInputFile (lInputFilename.c_str());

  // Get a handler (pointer) on the underlying buffer stream
  std::streambuf* lInputFileBuffer_ptr = lInputFile.rdbuf();

  // Get the number of characters within the input file
  const unsigned long nbChars = lInputFileBuffer_ptr->in_avail();

  // DEBUG
  std::cout << "Number of characters in the file '" << lInputFilename << "': "
            << nbChars << std::endl;

  // Instantiate a character buffer for that number of characters
  char lBufferChar[nbChars];

  // Read that number of characters from the input file, and put it directly into the
  // character buffer
  lInputFileBuffer_ptr->sgetn (lBufferChar, nbChars);

  // Instantiate an input stream with the character buffer
  std::istringstream iStr (lBufferChar);
  // std::ostringstream iStr (lBufferChar);

  // DEBUG
  // std::cout << "The file contains: " << iStr.str() << std::endl;
  
  return 0;
}

