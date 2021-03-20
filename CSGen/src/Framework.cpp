
#include "Framework.hpp"

#include <cmath>

namespace CSGen {
namespace Framework {

Page::Page(PZInt aWidth_mm, PZInt aHeight_mm, PZInt aPixelsPerMillimetre, sf::Color aBackgroundColor)
  : _pixelsPerMillimetre(aPixelsPerMillimetre)
{
  _canvas.create(static_cast<unsigned>(aWidth_mm * _pixelsPerMillimetre),
                 static_cast<unsigned>(aHeight_mm * _pixelsPerMillimetre));
  _canvas.setSmooth(false);
  _canvas.clear(sf::Color::White);
}

float Page::mm2px(float aLength_mm) const {
  return aLength_mm * _pixelsPerMillimetre;
}

float Page::getWidth() const {
  return static_cast<float>(_canvas.getSize().x);
}

float Page::getHeight() const {
  return static_cast<float>(_canvas.getSize().y);
}

void Page::pushColors(sf::Color aOutlineColor, sf::Color aFillColor) {
  _colors.push_back(Colors{aOutlineColor, aFillColor});
}

void Page::popColors() {
  _colors.pop_back();
}

sf::Color Page::getOutlineColor() const {
  if (!_colors.empty()) {
    return _colors.back().outlineColor;
  }
  else {
    return sf::Color::Black;
  }
}

sf::Color Page::getFillColor() const {
  if (!_colors.empty()) {
    return _colors.back().fillColor;
  }
  else {
    return sf::Color::Transparent;
  }
}

void Page::clearWithColor(sf::Color aColor) {
  _canvas.clear(aColor);
}

void Page::render() {
  _canvas.display();
}

const sf::Font* Page::loadFont(const std::string& aPath) const {
  const auto iter = std::find_if(std::begin(_fontCache), std::end(_fontCache),
               [&aPath](const std::pair<std::string, sf::Font>& aElem) {
                 return (aElem.first == aPath);
               });

  if (iter != std::end(_fontCache)) {
    return &(iter->second);
  }

  _fontCache.emplace_back();

  if (!_fontCache.back().second.loadFromFile(aPath)) {
    _fontCache.pop_back();
    throw std::runtime_error("Could not load font " + aPath);
  }

  _fontCache.back().first = aPath;

  return &(_fontCache.back().second);
}

const sf::Image* Page::loadImage(const std::string& aPath) const {
  const auto iter = std::find_if(std::begin(_imageCache), std::end(_imageCache),
                                 [&aPath](const std::pair<std::string, sf::Image>& aElem) {
                                   return (aElem.first == aPath);
                                 });

  if (iter != std::end(_imageCache)) {
    return &(iter->second);
  }

  _imageCache.emplace_back();

  if (!_imageCache.back().second.loadFromFile(aPath)) {
    _imageCache.pop_back();
    throw std::runtime_error("Could not load image " + aPath);
  }

  _imageCache.back().first = aPath;

  return &(_imageCache.back().second);
}

void Page::dumpToFile(const std::string& aFileName) const {
  if (!_canvas.getTexture().copyToImage().saveToFile(aFileName)) {
    throw std::runtime_error("Could not save image to file.");
  }
}

void Page::putHorizontal1pxLine(float aX_px, float aY_px, float aWidth_px) {
  sf::Vertex vertices[2];
  vertices[0].position = sf::Vector2f{aX_px, aY_px};
  vertices[0].color = getOutlineColor();
  vertices[1].position = sf::Vector2f{aX_px + aWidth_px, aY_px};
  vertices[1].color = getOutlineColor();

  _canvas.draw(vertices, 2, sf::PrimitiveType::LineStrip);
}

void Page::putVertical1pxLine(float aX_px, float aY_px, float aHeight_px) {
  sf::Vertex vertices[2];
  vertices[0].position = sf::Vector2f{aX_px, aY_px};
  vertices[0].color = getOutlineColor();
  vertices[1].position = sf::Vector2f{aX_px, aY_px + aHeight_px};
  vertices[1].color = getOutlineColor();

  _canvas.draw(vertices, 2, sf::PrimitiveType::LineStrip);
}

void Page::putRect(float aX_px, float aY_px, float aBBoxWidth_px,
                   float aBBoxHeight_px, PZInt aThickness_px) {
  sf::Vertex vertices[5];

  for (auto& vertex : vertices) {
    vertex.color = getOutlineColor();
  }

  const auto CORRECTION_OFFSET = 1.f;

  for (PZInt i = 0; i <= aThickness_px; i += 1) {
    vertices[0].position = vertices[4].position =
      sf::Vector2f{aX_px + static_cast<float>(i) + CORRECTION_OFFSET,
                   aY_px + static_cast<float>(i)};
    vertices[1].position =
      sf::Vector2f{aX_px + aBBoxWidth_px - static_cast<float>(i),
                   aY_px + static_cast<float>(i)};
    vertices[2].position =
      sf::Vector2f{aX_px + aBBoxWidth_px - static_cast<float>(i),
                   aY_px + aBBoxHeight_px - static_cast<float>(i) - CORRECTION_OFFSET};
    vertices[3].position =
      sf::Vector2f{aX_px + static_cast<float>(i),
                   aY_px + aBBoxHeight_px - static_cast<float>(i) - CORRECTION_OFFSET};

    if (i < aThickness_px) {
      _canvas.draw(vertices, 5, sf::PrimitiveType::LineStrip);
    }
    else {
      for (auto& vertex : vertices) {
        vertex.color = getFillColor();
      }
      _canvas.draw(vertices, 4, sf::PrimitiveType::Quads);
    }
  } // end outer for loop
}

void Page::putText(float aBBoxX_px, float aBBoxY_px, float aBBoxWidth_px, float aBBoxHeight_px,
                   sf::Text aText, int aHorAlign, int aVerAlign,
                   float aHorOffset_px, float aVerOffset_px) {
  const auto localBounds = aText.getLocalBounds();
  sf::Vector2f origin;
  sf::Vector2f pos;

  switch (aHorAlign) {
  default:
  case TEXT_HA_LEFT:
    origin.x = 0.f;
    pos.x = aBBoxX_px;
    break;

  case TEXT_HA_CENTRE:
    origin.x = localBounds.left + localBounds.width * 0.5f;
    pos.x = aBBoxX_px + (aBBoxWidth_px * 0.5f);
    break;

  case TEXT_HA_RIGHT:
    origin.x = localBounds.left + localBounds.width;
    pos.x = aBBoxX_px + aBBoxWidth_px;
    break;
  }

  switch (aVerAlign) {
  default:
  case TEXT_VA_TOP:
    origin.y = 0.f;
    pos.y = aBBoxY_px;
    break;

  case TEXT_VA_CENTRE:
    origin.y = localBounds.top + localBounds.height / 2.f;
    pos.y = aBBoxY_px + (aBBoxHeight_px * 0.5f);
    break;

  case TEXT_VA_BOTTOM:
    origin.y = localBounds.top + localBounds.height;
    pos.y = aBBoxY_px + aBBoxHeight_px;
    break;
  
  }

  pos.x += aHorOffset_px;
  pos.y += aVerOffset_px;

  origin.x = std::round(origin.x);
  origin.y = std::round(origin.y);
  pos.x = std::round(pos.x);
  pos.y = std::round(pos.y);

  aText.setOrigin(origin);
  aText.setPosition(pos);

  _canvas.draw(aText);

  //pushColors(sf::Color::Red, sf::Color::Transparent);
  //putRect(aBBoxX_px, aBBoxY_px, aBBoxWidth_px + 1.f, aBBoxHeight_px + 1.f, 1);
  //popColors();
}

void Page::putImage(float aBBoxX_px, float aBBoxY_px, const sf::Image& aImage) {
  sf::Texture texture;
  texture.loadFromImage(aImage);

  sf::Sprite sprite;
  sprite.setTexture(texture, true);

  sprite.setPosition({aBBoxX_px, aBBoxY_px});

  _canvas.draw(sprite);
}

} // namespace Framework
} // namespace CSGen