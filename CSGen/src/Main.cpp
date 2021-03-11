
#include "Common.hpp"
#include "GeneratorBase.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace CSGen {

class Generator : public GeneratorBase {
public:
  Generator(PZInt aWidth_mm, PZInt aHeight_mm, PZInt aPixelsPerMillimetre)
    : GeneratorBase{aWidth_mm, aHeight_mm, aPixelsPerMillimetre}
  {
  }

  void generatePage(PZInt aPageOrdinal) {
    putRect(0, 0, 5, 5, 3);
    putRect(7, 0, 5, 5, 3);
    putRect(0, 7, 5, 5, 3);

    putHorizontal1pxLine(0, 10, 20);

    finalizeCanvas();
  }

  void* getTexture(); // Save to File?

private:

};

} // namespace CSGen

#define CATCH_ALL_EXCEPTIONS_FROM_MAIN 1

int main(int argc, char* argv[])
#if CATCH_ALL_EXCEPTIONS_FROM_MAIN
try
#endif
{
  CSGen::Generator generator{
    // Canvas width in mm
    100,
    // Canvas height in mm
    100,
    // Pixels per mm (12 => 304.8 ppi)
    12
  };

  generator.generatePage(0);
  generator.dumpCanvasToFile("out.png");

  return EXIT_SUCCESS;
}
#if CATCH_ALL_EXCEPTIONS_FROM_MAIN
catch (const std::exception& ex) {
  std::cerr << "Exception caught: " << ex.what() << std::endl;
  return EXIT_FAILURE;
}
catch (...) {
  std::cerr << "Unknown exception caught." << std::endl;
  return EXIT_FAILURE;
}
#endif
