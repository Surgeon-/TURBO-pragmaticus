#ifndef CSGEN_FRAMEWORK_HPP
#define CSGEN_FRAMEWORK_HPP

#include <list>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace CSGen {
namespace Framework {

using PZInt = int;

constexpr int TEXT_HA_LEFT   = 0;
constexpr int TEXT_HA_CENTRE = 1;
constexpr int TEXT_HA_RIGHT  = 2;
constexpr int TEXT_VA_TOP    = 0;
constexpr int TEXT_VA_CENTRE = 1;
constexpr int TEXT_VA_BOTTOM = 2;

class Page {
public:
  Page(PZInt aWidth_mm, PZInt aHeight_mm, PZInt aPixelsPerMillimetre,
       sf::Color aBackgroundColor = sf::Color::White);

  void reset(PZInt aWidth_mm, PZInt aHeight_mm, PZInt aPixelsPerMillimetre,
             sf::Color aBackgroundColor = sf::Color::White);

  float mm2px(float aLength_mm) const;
  float getWidth() const;
  float getHeight() const;

  void pushColors(sf::Color aOutlineColor, sf::Color aFillColor);
  void popColors();
  sf::Color getOutlineColor() const;
  sf::Color getFillColor() const;
  void clearWithColor(sf::Color aColor);

  void render();

  const sf::Font* loadFont(const std::string& aPath) const;
  const sf::Image* loadImage(const std::string& aPath) const;

  // file name must end with .bmp, .png, .tga or .jpg
  void dumpToFile(const std::string& aFileName) const;

  void putHorizontal1pxLine(float aX_px, float aY_px, float aWidth_px);
  void putVertical1pxLine(float aX_px, float aY_px, float aHeight_px);
  void putRect(float aX_px, float aY_px, float aBBoxWidth_px,
               float aBBoxHeight_px, PZInt aThickness_px);

  void putText(float aBBoxX_px, float aBBoxY_px, float aBBoxWidth_px, float aBBoxHeight_px,
               sf::Text aText, int aHorAlign = TEXT_HA_LEFT, int aVerAlign = TEXT_VA_TOP,
               float aHorOffset_px = 0.f, float aVerOffset_px = 0.f);

  void putImage(float aBBoxX_px, float aBBoxY_px, const sf::Image& aImage);

private:
  struct Colors {
    sf::Color outlineColor;
    sf::Color fillColor;
  };

  sf::RenderTexture _canvas;
  std::vector<Colors> _colors;
  mutable std::list<std::pair<std::string, sf::Font>> _fontCache;
  mutable std::list<std::pair<std::string, sf::Image>> _imageCache;
  PZInt _pixelsPerMillimetre;
};



struct BoundingBox {
  BoundingBox(float aX = 0.f, float aY = 0.f, float aWidth = 0.f, float aHeight = 0.f)
    : x(aX), y(aY), width(aWidth), height(aHeight)
  {
  }

  float x;
  float y;
  float width;
  float height;
};



class Entity {
public:
  explicit Entity(const BoundingBox& aBBox)
    : _bbox{aBBox}
  {
  }

  virtual void draw() const = 0;

  const BoundingBox& getBBox() const {
    return _bbox;
  }

  float getX() const {
    return _bbox.x;
  }

  float getY() const {
    return _bbox.y;
  }

  float getX2() const {
    return _bbox.x + _bbox.width;
  }

  float getY2() const {
    return _bbox.y + _bbox.height;
  }

  float getWidth() const {
    return _bbox.width;
  }

  float getHeight() const {
    return _bbox.height;
  }

protected:
  void setSize(float aWidth, float aHeight) {
    _bbox.width = aWidth;
    _bbox.height = aHeight;
  }

private:
  BoundingBox _bbox;
};

} // namespace Framework
} // namespace CSGen

#endif // !CSGEN_FRAMEWORK_HPP