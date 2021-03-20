
#include "TurboPragmaticus.hpp"

#include <string>
#include <vector>

namespace CSGen {
namespace TurboPragmaticus {

#define DEFAULT_THICKNESS_PX 3
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

using Framework::PZInt;
using Framework::BoundingBox;
using Framework::Entity;
using Framework::Page;

struct Position {
  float x;
  float y;
};

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
  SectionHeader(Page& aPage, const Position& aPos, float aWidth, const std::string& aSectionName)
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

    // Text
    // TODO

    _page.popColors();
  }

private:
  Page& _page;
  std::string _sectionName;
};

class AttributePanel : public Entity {
public:
  AttributePanel(Page& aPage, const Position& aPos, const std::string& aAttributeName)
    : Entity(BoundingBox(
      aPos.x,
      aPos.y,
      aPage.mm2px(ATTR_PANEL_WIDTH_MM),
      aPage.mm2px(ATTR_PANEL_HEIGHT_MM)))
    , _page(aPage)
  {
  }

  void draw() const override {
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

private:
  Page& _page;
};

class SkillPanel : public Entity {
public:
  SkillPanel(Page& aPage, const Position& aPos, const std::string& aSkillName)
    : Entity(BoundingBox(
      aPos.x,
      aPos.y,
      aPage.mm2px(SKILL_PANEL_WIDTH_MM),
      aPage.mm2px(SKILL_PANEL_TOTAL_HEIGHT_MM)))
    , _page(aPage)
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

    // SGA nameplate
    _page.putRect(
      getX() + _page.mm2px(SKILL_PANEL_NAMEPLATE_WIDTH_MM),
      getY(),
      _page.mm2px(SKILL_PANEL_SGA_NAMEPLATE_WIDTH_MM),
      _page.mm2px(SKILL_PANEL_FIRST_ROW_HEIGHT_MM),
      DEFAULT_THICKNESS_PX);

    // Exponents
    // TODO

    // ETP
    // TODO

    // PROGs
    // TODO
  }

private:
  Page& _page;
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
    static constexpr const char* ATTRIBUTE_NAMES[4] = {
      "Might", "Agility", "Reaction", "Cunning"
    };

    for (PZInt i = 0; i < 4 /* number of skill groups*/; i += 1) {
      if (i == 0) {
        _attributePanels.emplace_back(
          _page,
          Position{getX(),
                    _sectionHeader.getY2() + _page.mm2px(SKILL_TOOLTIP_PANEL_HEIGHT_MM)},
          ATTRIBUTE_NAMES[i]);
      }
      else {
        _attributePanels.emplace_back(
          _page,
          Position{getX(),
                   (_attributePanels.rbegin()++)->getY2() + _page.mm2px(WIDE_PADDING_MM)},
          ATTRIBUTE_NAMES[i]);
      }

      for (PZInt j = 0; j < 4 /* number of skills per group */; j += 1) {
        const float xx = _attributePanels.back().getX2() + _page.mm2px(NARROW_PADDING_MM);
        if (j == 0) {
          _skillPanels.emplace_back(
            _page,
            Position{xx, _attributePanels.back().getY()},
            "<Placeholder>");
        }
        else {
          _skillPanels.emplace_back(
            _page,
            Position{xx, _skillPanels.back().getY2() + _page.mm2px(SKILL_PANEL_VERTICAL_SPACING_MM)},
            "<Placeholder>");
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
  {
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
  }

  // BOTTOM HALF //////////////////////////////////////////////////////////////
  // TODO

  // HALVES DIVIDER ///////////////////////////////////////////////////////////
  aPage.pushColors(sf::Color::Black, sf::Color(200, 200, 200, 255));
  aPage.putRect(
    0.f,
    aPage.getHeight() / 2.f - aPage.mm2px(1.f),
    aPage.getWidth(),
    aPage.mm2px(2.f),
    DEFAULT_THICKNESS_PX);
  aPage.popColors();
}

} // namespace TurboPragmaticus
} // namespace CSGen