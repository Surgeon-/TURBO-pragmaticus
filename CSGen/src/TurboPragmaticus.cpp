
#include "TurboPragmaticus.hpp"

#include <string>
#include <vector>

namespace CSGen {
namespace TurboPragmaticus {

constexpr float DEFAULT_THICKNESS_PX = 3.f;
constexpr float SECTION_HEADER_HEIGHT_MM = 5.f;
constexpr float PAGE_HEADER_HEIGHT_MM = 5.f;
constexpr float WIDE_PADDING_MM = 2.f;
constexpr float NARROW_PADDING_MM = 1.f;

constexpr float LOGO_WIDTH_MM = 44.f;
constexpr float LOGO_HEIGHT_MM = 30.f; // TEMP
constexpr float ATTR_PANEL_WIDTH_MM = 20.5f;
constexpr float ATTR_PANEL_HEIGHT_MM = 27.5f;
constexpr float SKILL_PANEL_WIDTH_MM = 80.f;
constexpr float SKILL_PANEL_TOTAL_HEIGHT_MM = 6.5f;
constexpr float SKILL_PANEL_FIRST_ROW_HEIGHT_MM = 4.f;
constexpr float SKILL_PANEL_VERTICAL_SPACING_MM = 0.5f;
constexpr float SKILL_PANEL_NAMEPLATE_WIDTH_MM = 27.f;
constexpr float SKILL_PANEL_SGA_NAMEPLATE_WIDTH_MM = 11.f;
constexpr float SKILL_TOOLTIP_PANEL_HEIGHT_MM = 5.f;

#define FONT_BASE_PATH "resources/fonts/"
#define SECTION_HEADER_FONT_PATH   FONT_BASE_PATH "Spectral-Light.ttf"
#define SECTION_HEADER_FONT_SIZE   44
#define SKILL_NAME_FONT_PATH   FONT_BASE_PATH "SpectralSC-Bold.ttf"
#define SKILL_NAME_FONT_SIZE   30
#define SKILL_SGA_FONT_PATH   FONT_BASE_PATH "SpectralSC-Medium.ttf"
#define SKILL_SGA_FONT_SIZE   20
#define ATTR_PANEL_FONT_PATH   FONT_BASE_PATH "Romanesco-Regular.ttf"
#define ATTR_PANEL_FONT_SIZE   64

#define IMG_BASE_PATH "resources/images/"
#define ATTR_PANEL_IMG_PATH   IMG_BASE_PATH "attribute-panel.png"
#define SKILL_PROG_TRACK_IMG_PATH   IMG_BASE_PATH "skill-prog-track.png"

using Framework::PZInt;
using Framework::BoundingBox;
using Framework::Entity;
using Framework::Page;

using Framework::TEXT_HA_LEFT;
using Framework::TEXT_HA_CENTRE;
using Framework::TEXT_HA_RIGHT;
using Framework::TEXT_VA_TOP;
using Framework::TEXT_VA_CENTRE;
using Framework::TEXT_VA_BOTTOM;

struct Position {
  float x;
  float y;
};

sf::Text MakeBasicText(const Page& aPage,
                       const sf::String& aText,
                       const std::string& aFontPath,
                       int aCharacterSize = 30) {
  sf::Text text{aText, *aPage.loadFont(aFontPath), static_cast<unsigned>(aCharacterSize)};
  text.setFillColor(aPage.getFillColor());
  text.setOutlineColor(aPage.getOutlineColor());
  return text;
}

const char* const ATTRIBUTES_AND_SKILLS[8][9] = {
  {"Might",
    "Athletics", "-",
    "Fortitude", "-",
    "Melee <Armed>", "AGI",
    "Melee <Unarmed>", "AGI"},

  {"Agility",
    "Acrobatics", "-",
    "Escapology", "-",
    "Stealth", "-",
    "Throw", "-"},

  {"Reaction",
    "Marksman", "-",
    "Perception", "-",
    "Reflex", "-",
    "Ride/Drive", "-"},

  {"Equilibrium",
    "Concentration", "-",
    "First Aid", "-",
    "?", "-",
    "?", "-"},

  {"Intelligence",
    "Insight", "-",
    "Lore", "-",
    "Outdoorsman", "-",
    "Science", "-"},

  {"Cunning",
    "Deception", "-",
    "Investigate", "-",
    "Skullduggery", "-",
    "Tinker", "-"},

  {"Presence",
    "Command", "-",
    "Consort", "-",
    "Sway", "-",
    "Willpower", "-"},

  {"",
    "", "",
    "", "",
    "", "",
    "", ""}
};

const char* GetAttribute(PZInt aIndex) {
  return ATTRIBUTES_AND_SKILLS[aIndex][0];
}

const char* GetSkillName(PZInt aAttributeIndex, PZInt aSkillIndex) {
  return ATTRIBUTES_AND_SKILLS[aAttributeIndex][aSkillIndex * 2 + 1];
}

const char* GetSkillSGA(PZInt aAttributeIndex, PZInt aSkillIndex) {
  return ATTRIBUTES_AND_SKILLS[aAttributeIndex][aSkillIndex * 2 + 2];
}

class LogoPanel : public Entity {
public:
  LogoPanel(Page& aPage, const Position& aPos)
    : Entity(BoundingBox(aPos.x, aPos.y, aPage.mm2px(LOGO_WIDTH_MM), aPage.mm2px(LOGO_HEIGHT_MM)))
    , _page(aPage)
  {
  }

  void draw() const override {
    // TODO - Temp.
    _page.pushColors(sf::Color::Black, sf::Color::Transparent);
    _page.putRect(
      getX(),
      getY(),
      getWidth(),
      getHeight(),
      DEFAULT_THICKNESS_PX);
    _page.popColors();
  }

private:
  Page& _page;
};

class PageHeader : public Entity {
public:
  PageHeader(Page& aPage, const Position& aPos, float aWidth, const std::string& aText)
    : Entity(BoundingBox(
      aPos.x,
      aPos.y,
      aPage.mm2px(aWidth),
      aPage.mm2px(PAGE_HEADER_HEIGHT_MM)))
    , _page(aPage)
    , _text(aText)
  {
  }

  void draw() const override {
    // TODO - Temp.
    _page.pushColors(sf::Color::Black, sf::Color::Transparent);
    _page.putRect(
      getX(),
      getY(),
      getWidth(),
      getHeight(),
      DEFAULT_THICKNESS_PX);
    _page.popColors();
  }

private:
  Page& _page;
  std::string _text;
};

class SectionHeader : public Entity {
public:
  SectionHeader(Page& aPage, const Position& aPos, float aWidth, const sf::String& aSectionName)
    : Entity(BoundingBox(
      aPos.x,
      aPos.y,
      aWidth,
      aPage.mm2px(SECTION_HEADER_HEIGHT_MM)))
    , _page(aPage)
    , _sectionName(aSectionName)
  {
  }

  void draw() const override {
    _page.pushColors(sf::Color::Black, sf::Color::Transparent);

    _page.putRect(
      getX(),
      getY(),
      _page.mm2px(SECTION_HEADER_HEIGHT_MM),
      getHeight(),
      DEFAULT_THICKNESS_PX);

    _page.putRect(
      getX() + _page.mm2px(SECTION_HEADER_HEIGHT_MM + NARROW_PADDING_MM),
      getY(),
      getWidth() - _page.mm2px(SECTION_HEADER_HEIGHT_MM + NARROW_PADDING_MM),
      getHeight(),
      DEFAULT_THICKNESS_PX);

    _page.popColors();

    _page.pushColors(sf::Color::Transparent, sf::Color::Black);
    auto text = MakeBasicText(
      _page,
      _sectionName,
      SECTION_HEADER_FONT_PATH,
      SECTION_HEADER_FONT_SIZE);
    text.setLetterSpacing(0.8f);

    _page.putText(
      getX() + _page.mm2px(SECTION_HEADER_HEIGHT_MM + NARROW_PADDING_MM),
      getY(),
      0.f,
      getHeight(),
      text,
      TEXT_HA_LEFT,
      TEXT_VA_TOP,
      _page.mm2px(0.75f),
      _page.mm2px(0.85f));
    _page.popColors();
  }

private:
  Page& _page;
  sf::String _sectionName;
};

class AttributePanel : public Entity {
public:
  AttributePanel(Page& aPage, const Position& aPos, const sf::String& aAttributeName)
    : Entity(BoundingBox(
      aPos.x,
      aPos.y,
      aPage.mm2px(ATTR_PANEL_WIDTH_MM),
      aPage.mm2px(ATTR_PANEL_HEIGHT_MM)))
    , _page(aPage)
    , _attributeName(aAttributeName)
  {
  }

  void draw() const override {
    if (!_attributeName.isEmpty()) {
      auto* icon = _page.loadImage(IMG_BASE_PATH + _attributeName + ".png");
      _page.putImage(getX(), getY(), *icon);

      // The image is 1px too narrow but I don't care
      auto* image = _page.loadImage(ATTR_PANEL_IMG_PATH);
      _page.putImage(getX(), getY(), *image);

      _page.pushColors(sf::Color::Black, sf::Color(220, 220, 220, 255));
      auto text = MakeBasicText(_page, _attributeName, ATTR_PANEL_FONT_PATH, ATTR_PANEL_FONT_SIZE);
      text.setLetterSpacing(1.5f);
      _page.putText(
        getX(),
        getY(),
        getWidth(),
        59.f, // TODO Temp.
        text,
        TEXT_HA_CENTRE,
        TEXT_VA_TOP,
        _page.mm2px(0.f),
        _page.mm2px(-1.25f));
      _page.popColors();
    }
    else {
      // TODO Temp.
      _page.pushColors(sf::Color::Black, sf::Color::Transparent);
      _page.putRect(
        getX(),
        getY(),
        getWidth(),
        getHeight(),
        DEFAULT_THICKNESS_PX);
      _page.popColors();
    }
  }

private:
  Page& _page;
  sf::String _attributeName;
};

class SkillPanel : public Entity {
public:
  SkillPanel(Page& aPage, const Position& aPos, 
             const sf::String& aSkillName, const sf::String& aSkillSGA)
    : Entity(BoundingBox(
      aPos.x,
      aPos.y,
      aPage.mm2px(SKILL_PANEL_WIDTH_MM),
      aPage.mm2px(SKILL_PANEL_TOTAL_HEIGHT_MM)))
    , _page(aPage)
    , _skillName(aSkillName)
    , _skillSGA(aSkillSGA)
  {
  }

  void draw() const override {
    // Main rectangle
    _page.putRect(
      getX(),
      getY(),
      _page.mm2px(SKILL_PANEL_WIDTH_MM),
      _page.mm2px(SKILL_PANEL_FIRST_ROW_HEIGHT_MM),
      DEFAULT_THICKNESS_PX);

    // Name
    _page.pushColors(sf::Color::Transparent, sf::Color::Black);
    _page.putText(
      getX(),
      getY(),
      0.f,
      _page.mm2px(SKILL_PANEL_FIRST_ROW_HEIGHT_MM),
      MakeBasicText(
        _page,
        _skillName,
        SKILL_NAME_FONT_PATH,
        SKILL_NAME_FONT_SIZE),
      TEXT_HA_LEFT,
      TEXT_VA_CENTRE,
      _page.mm2px(0.75f),
      _page.mm2px(0.25f));
    _page.popColors();

    // SGA nameplate
    _page.putRect(
      getX() + _page.mm2px(SKILL_PANEL_NAMEPLATE_WIDTH_MM),
      getY(),
      _page.mm2px(SKILL_PANEL_SGA_NAMEPLATE_WIDTH_MM),
      _page.mm2px(SKILL_PANEL_FIRST_ROW_HEIGHT_MM),
      DEFAULT_THICKNESS_PX);

    _page.pushColors(sf::Color::Transparent, sf::Color::Black);
    _page.putText(
      getX() + _page.mm2px(SKILL_PANEL_NAMEPLATE_WIDTH_MM),
      getY(),
      _page.mm2px(SKILL_PANEL_SGA_NAMEPLATE_WIDTH_MM),
      _page.mm2px(SKILL_PANEL_FIRST_ROW_HEIGHT_MM),
      MakeBasicText(
        _page,
        _skillSGA,
        SKILL_SGA_FONT_PATH,
        SKILL_SGA_FONT_SIZE),
      TEXT_HA_CENTRE,
      TEXT_VA_CENTRE);
    _page.popColors();

    // Exponents
    // TODO

    // ETP
    // TODO

    // PROGs
    // TODO
    auto* image = _page.loadImage(SKILL_PROG_TRACK_IMG_PATH);
    _page.putImage(
      getX2() - image->getSize().x,
      getY() + _page.mm2px(SKILL_PANEL_FIRST_ROW_HEIGHT_MM),
      *image);
  }

private:
  Page& _page;
  sf::String _skillName;
  sf::String _skillSGA;
};

class SkillsAndAttributesTop : public Entity {
public:
  SkillsAndAttributesTop(Page& aPage, const Position& aPos)
    : Entity(BoundingBox(aPos.x, aPos.y, 0.f, 0.f))
    , _page(aPage)
    , _sectionHeader(aPage,
                     Position{getX(),
                              getY()},
                     aPage.mm2px(ATTR_PANEL_WIDTH_MM + SKILL_PANEL_WIDTH_MM + NARROW_PADDING_MM),
                     "Attributes & Skills")
  {
    for (PZInt i = 0; i < 4 /* number of skill groups*/; i += 1) {
      if (i == 0) {
        _attributePanels.emplace_back(
          _page,
          Position{getX(),
                    _sectionHeader.getY2() + _page.mm2px(SKILL_TOOLTIP_PANEL_HEIGHT_MM)},
          GetAttribute(i));
      }
      else {
        _attributePanels.emplace_back(
          _page,
          Position{getX(),
                   (_attributePanels.rbegin()++)->getY2() + _page.mm2px(WIDE_PADDING_MM)},
          GetAttribute(i));
      }

      for (PZInt j = 0; j < 4 /* number of skills per group */; j += 1) {
        const float xx = _attributePanels.back().getX2() + _page.mm2px(NARROW_PADDING_MM);
        if (j == 0) {
          _skillPanels.emplace_back(
            _page,
            Position{xx, _attributePanels.back().getY()},
            GetSkillName(i, j),
            GetSkillSGA(i, j));
        }
        else {
          _skillPanels.emplace_back(
            _page,
            Position{xx, _skillPanels.back().getY2() + _page.mm2px(SKILL_PANEL_VERTICAL_SPACING_MM)},
            GetSkillName(i, j),
            GetSkillSGA(i, j));
        }
      }
    }

    setSize(_skillPanels.back().getX2() - getX(),
            _skillPanels.back().getY2() - getY());
  }

  void draw() const override {
    _sectionHeader.draw();
    for (const auto& panel : _attributePanels) {
      panel.draw();
    }
    for (const auto& panel : _skillPanels) {
      panel.draw();
    }
  }

private:
  Page& _page;
  SectionHeader _sectionHeader;
  std::vector<AttributePanel> _attributePanels;
  std::vector<SkillPanel> _skillPanels;
};

class SpecializationsTop : public Entity {
public:
  SpecializationsTop(Page& aPage, const BoundingBox& aBBox)
    : Entity(aBBox)
    , _page(aPage)
    , _sectionHeader(aPage,
                     Position{getX(),
                              getY()},
                     getWidth(),
                     "Specializations")
  {
  }

  void draw() const override {
    _sectionHeader.draw();

    _page.pushColors(sf::Color::Black, sf::Color::Transparent);
    _page.putRect(
      getX(),
      _sectionHeader.getY2() + _page.mm2px(SKILL_TOOLTIP_PANEL_HEIGHT_MM),
      getWidth(),
      getHeight() - _sectionHeader.getHeight() - _page.mm2px(SKILL_TOOLTIP_PANEL_HEIGHT_MM),
      DEFAULT_THICKNESS_PX);
    _page.popColors();
  }

private:
  Page& _page;
  SectionHeader _sectionHeader;
};

class SkillsAndAttributesBottom : public Entity {
public:
  SkillsAndAttributesBottom(Page& aPage, const Position& aPos)
    : Entity(BoundingBox(aPos.x, aPos.y, 0.f, 0.f))
    , _page(aPage)
  {
    for (PZInt i = 4; i < 8 /* number of skill groups*/; i += 1) {
      if (i == 4) {
        _attributePanels.emplace_back(
          _page,
          Position{getX(), getY()},
          GetAttribute(i));
      }
      else {
        _attributePanels.emplace_back(
          _page,
          Position{getX(),
                   (_attributePanels.rbegin()++)->getY2() + _page.mm2px(WIDE_PADDING_MM)},
          GetAttribute(i));
      }

      for (PZInt j = 0; j < ((i < 7) ? 4 : 6) /* number of skills per group */; j += 1) {
        const float xx = _attributePanels.back().getX2() + _page.mm2px(NARROW_PADDING_MM);
        if (j == 0) {
          _skillPanels.emplace_back(
            _page,
            Position{xx, _attributePanels.back().getY()},
            GetSkillName(i, j),
            GetSkillSGA(i, j));
        }
        else if (j >= 4) {
          _skillPanels.emplace_back(
            _page,
            Position{xx, _skillPanels.back().getY2() + _page.mm2px(SKILL_PANEL_VERTICAL_SPACING_MM)},
            "",
            "");
        }
        else {
          _skillPanels.emplace_back(
            _page,
            Position{xx, _skillPanels.back().getY2() + _page.mm2px(SKILL_PANEL_VERTICAL_SPACING_MM)},
            GetSkillName(i, j),
            GetSkillSGA(i, j));
        }
      }
    }

    setSize(_skillPanels.back().getX2() - getX(),
            _skillPanels.back().getY2() - getY());
  }

  void draw() const override {
    for (const auto& panel : _attributePanels) {
      panel.draw();
    }
    for (const auto& panel : _skillPanels) {
      panel.draw();
    }
  }

private:
  Page& _page;
  std::vector<AttributePanel> _attributePanels;
  std::vector<SkillPanel> _skillPanels;
};

//class Page1 : public Entity {
//public:
//  Page1(Page& aPage) {
//
//  }
//
//};

void DrawPage1(Page& aPage) {
  // TOP HALF ///////////////////////////////////////////////////////////////// 
  // Logo
  LogoPanel logoPanel{aPage, Position{0.f, 0.f}};
  logoPanel.draw();

  // Page header
  PageHeader pageHeader{
    aPage,
    Position{logoPanel.getX2() + aPage.mm2px(WIDE_PADDING_MM), 0.f},
    aPage.mm2px(10.f), // TODO Magic number
    "Page 1"};
  pageHeader.draw();

  // Stuff under the logo
  // TODO

  // Skills & Attributes
  SkillsAndAttributesTop skillsAndAttr{
    aPage,
    Position{logoPanel.getX2() + aPage.mm2px(WIDE_PADDING_MM),
              pageHeader.getY2() + aPage.mm2px(NARROW_PADDING_MM)}};
  skillsAndAttr.draw();

  // Specializations
  SpecializationsTop specs{
    aPage,
    BoundingBox(
      skillsAndAttr.getX2() + aPage.mm2px(WIDE_PADDING_MM),
      pageHeader.getY2() + aPage.mm2px(NARROW_PADDING_MM),
      aPage.mm2px(30.f), // TODO Magic num
      skillsAndAttr.getHeight())};
  specs.draw();
  

  // HALVES DIVIDER ///////////////////////////////////////////////////////////
  aPage.pushColors(sf::Color::Black, sf::Color(200, 200, 200, 255));
  aPage.putRect(
    0.f,
    aPage.getHeight() / 2.f - aPage.mm2px(1.f),
    aPage.getWidth(),
    aPage.mm2px(2.f),
    DEFAULT_THICKNESS_PX);
  aPage.popColors();
  

  // BOTTOM HALF //////////////////////////////////////////////////////////////
  const float bottomStartY = aPage.getHeight() / 2.f + aPage.mm2px(1.f); // TODO Temp.

  SkillsAndAttributesBottom skillsAndAttr2{
    aPage,
    Position{logoPanel.getX2() + aPage.mm2px(WIDE_PADDING_MM),
             bottomStartY + aPage.mm2px(WIDE_PADDING_MM)}};
  skillsAndAttr2.draw();
  


}

} // namespace TurboPragmaticus
} // namespace CSGen