
#include "Framework.hpp"
#include "TurboPragmaticus.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

#if 0

#define KCSGEN_PAGE_WIDTH_MM 180.f
#define KCSGEN_PAGE_HEIGHT_MM 270.f
#define KCSGEN_DEFAULT_THICKNESS_PX 3
#define KCSGEN_SECTION_TITLE_HEIGHT_MM 5.f
#define KCSGEN_PAGE_HEADER_HEIGHT_MM 5.f
#define KCSGEN_WIDE_HORIZONTAL_PADDING_MM 2.f
#define KCSGEN_NARROW_HORIZONTAL_PADDING_MM 1.f

#define KCSGEN_PG1_LOGO_WIDTH_MM 44.f
#define KCSGEN_PG1_LOGO_HEIGHT_MM ?
#define KCSGEN_PG1_ATTR_PANEL_WIDTH_MM 20.5
#define KCSGEN_PG1_ATTR_PANEL_HEIGHT_MM 27.5
#define KCSGEN_PG1_SKILL_PANEL_WIDTH_MM 80.f
#define KCSGEN_PG1_SKILL_PANEL_TOTAL_HEIGHT_MM 6.5f
#define KCSGEN_PG1_SKILL_PANEL_FIRST_ROW_HEIGHT_MM 4.f
#define KCSGEN_PG1_SKILL_PANEL_VERTICAL_SPACING_MM 0.5f
#define KCSGEN_PG1_SKILL_PANEL_NAMEPLATE_WIDTH_MM 27.f
#define KCSGEN_PG1_SKILL_PANEL_SGA_NAMEPLATE_WIDTH_MM 11.f

namespace CSGen {

class Generator : public GeneratorBase {
public:
  Generator(PZInt aWidth_mm, PZInt aHeight_mm, PZInt aPixelsPerMillimetre)
    : GeneratorBase{aWidth_mm, aHeight_mm, aPixelsPerMillimetre}
  {
  }

  void generatePage(PZInt aPageOrdinal) {
    switch (aPageOrdinal) {
    case 0: generateFirstPage(); break;
    case 1: break;
    case 2: break;
    case 3: break;
    default: {}
    }

    finalizeCanvas();
  }

private:
  void putPageHeader(float aX_mm, float aWidth_mm) {
    const sf::Color col{64, 64, 64, 255}; // Dark grey
    setOutlineColour(col);
    setFillColour(col);
    putRect(aX_mm, 0, aWidth_mm, KCSGEN_PAGE_HEADER_HEIGHT_MM, 1);
  }

  void putSectionHeader(float aX_mm, float aY_mm, float aWidth_mm, const std::string& aSectionName) {
    setOutlineColour(sf::Color::Black);
    setFillColour(sf::Color::Transparent);

    putRect(aX_mm, 
            aY_mm,
            KCSGEN_SECTION_TITLE_HEIGHT_MM,
            KCSGEN_SECTION_TITLE_HEIGHT_MM,
            KCSGEN_DEFAULT_THICKNESS_PX);

    putRect(aX_mm + (KCSGEN_SECTION_TITLE_HEIGHT_MM + 1),
            aY_mm,
            aWidth_mm - (KCSGEN_SECTION_TITLE_HEIGHT_MM + 1),
            KCSGEN_SECTION_TITLE_HEIGHT_MM,
            KCSGEN_DEFAULT_THICKNESS_PX);

    // Text
    // TODO
  }

  void putHalvesDivider() {
    setOutlineColour(sf::Color::Black);
    setFillColour(sf::Color(200, 200, 200, 255));
    putRect(0,
            KCSGEN_PAGE_HEIGHT_MM / 2 - 1,
            KCSGEN_PAGE_WIDTH_MM,
            2,
            KCSGEN_DEFAULT_THICKNESS_PX);
  }

  void putSkillPanel(float aX_mm, float aY_mm, const std::string& aSkillName) {
    setOutlineColour(sf::Color::Black);
    setFillColour(sf::Color::Transparent);

    // Main rectangle
    putRect(aX_mm, aY_mm,
            KCSGEN_PG1_SKILL_PANEL_WIDTH_MM,
            KCSGEN_PG1_SKILL_PANEL_FIRST_ROW_HEIGHT_MM,
            KCSGEN_DEFAULT_THICKNESS_PX);

    // SGA nameplate
    putRect(aX_mm + KCSGEN_PG1_SKILL_PANEL_NAMEPLATE_WIDTH_MM, aY_mm,
            KCSGEN_PG1_SKILL_PANEL_SGA_NAMEPLATE_WIDTH_MM,
            KCSGEN_PG1_SKILL_PANEL_FIRST_ROW_HEIGHT_MM,
            KCSGEN_DEFAULT_THICKNESS_PX);

    // Exponents
    // TODO

    // ETP
    // TODO

    // PROGs
    // TODO
  }

  void putSkillGroup(float aX_mm, float aY_mm) {
    for (PZInt i = 0; i < 4; i += 1) {
      putSkillPanel(aX_mm,
                    aY_mm + i * (KCSGEN_PG1_SKILL_PANEL_TOTAL_HEIGHT_MM +
                                 KCSGEN_PG1_SKILL_PANEL_VERTICAL_SPACING_MM),
                    "placeholder");
    }
  }

  void generateFirstPage() {
    // TOP HALF /////////////////////////////////
    // Put Logo - TODO
    
    // Page Header
    putPageHeader(KCSGEN_PG1_LOGO_WIDTH_MM + KCSGEN_WIDE_HORIZONTAL_PADDING_MM,
                  180 - (KCSGEN_PG1_LOGO_WIDTH_MM + KCSGEN_WIDE_HORIZONTAL_PADDING_MM));

    // Put Under-logo stuff - TODO

    // Attributes & Skills
    {
      const float startX_mm = KCSGEN_PG1_LOGO_WIDTH_MM + KCSGEN_WIDE_HORIZONTAL_PADDING_MM;
      const float startY_mm = KCSGEN_PAGE_HEADER_HEIGHT_MM + 1;

      // Section header
      putSectionHeader(startX_mm,
                       startY_mm,
                       KCSGEN_PG1_ATTR_PANEL_WIDTH_MM + KCSGEN_NARROW_HORIZONTAL_PADDING_MM
                       + KCSGEN_PG1_SKILL_PANEL_WIDTH_MM,
                       "Attributes & Skills");
      // Table
      for (PZInt i = 0; i < 4; i += 1) {
        const float localY =
          startY_mm + KCSGEN_SECTION_TITLE_HEIGHT_MM + 5 /* custom */ +
          i * (4 * (KCSGEN_PG1_SKILL_PANEL_TOTAL_HEIGHT_MM +
                    KCSGEN_PG1_SKILL_PANEL_VERTICAL_SPACING_MM)
               - KCSGEN_PG1_SKILL_PANEL_VERTICAL_SPACING_MM /* last PROG panel doesn't have v. spacing */
               + 2 /* spacing between skill groups */);

        // Attribute panel
        putRect(startX_mm, 
                localY, 
                KCSGEN_PG1_ATTR_PANEL_WIDTH_MM, 
                KCSGEN_PG1_ATTR_PANEL_HEIGHT_MM, 
                KCSGEN_DEFAULT_THICKNESS_PX);
        
        // Skill group
        putSkillGroup(startX_mm + KCSGEN_PG1_ATTR_PANEL_WIDTH_MM + KCSGEN_NARROW_HORIZONTAL_PADDING_MM,
                      localY);
      }

      // Specializations
      {
        const float startX_mm = 
          KCSGEN_PG1_LOGO_WIDTH_MM + KCSGEN_WIDE_HORIZONTAL_PADDING_MM           // Logo   + padding
          + KCSGEN_PG1_ATTR_PANEL_WIDTH_MM + KCSGEN_NARROW_HORIZONTAL_PADDING_MM // Attrs  + padding
          + KCSGEN_PG1_SKILL_PANEL_WIDTH_MM + KCSGEN_WIDE_HORIZONTAL_PADDING_MM; // Skills + padding
        const float startY_mm = KCSGEN_PAGE_HEADER_HEIGHT_MM + 1;

        putSectionHeader(startX_mm,
                         startY_mm,
                         30,
                         "Specializations");

        putRect(startX_mm,
                startY_mm + KCSGEN_SECTION_TITLE_HEIGHT_MM + 5 /* custom */,
                30,
                (4 * KCSGEN_PG1_ATTR_PANEL_HEIGHT_MM) + (3 * 2),
                KCSGEN_DEFAULT_THICKNESS_PX);
      }
    }

    // BOTTOM HALF //////////////////////////////

    // HALVES DIVIDER ///////////////////////////
    putHalvesDivider();
  }
};

} // namespace CSGen

#endif

void MainBody() {
  using namespace CSGen;

  std::cout << "Initializing page...\n";
  Framework::Page page{
    static_cast<Framework::PZInt>(TurboPragmaticus::PAGE_WIDTH_MM),
    static_cast<Framework::PZInt>(TurboPragmaticus::PAGE_HEIGHT_MM),
    12 // Pixels per mm (12 => 304.8 ppi)
  };

  std::cout << "Drawing elements...\n";
  TurboPragmaticus::DrawPage1(page);

  std::cout << "Rendering...\n";
  page.render();

  std::cout << "Saving to file...\n";
  page.dumpToFile("out.png");

  std::cout << "Done!" << std::endl;
}

#define CATCH_ALL_EXCEPTIONS_FROM_MAIN 1

int main(int argc, char* argv[])
#if CATCH_ALL_EXCEPTIONS_FROM_MAIN
try
#endif
{
  MainBody();
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
