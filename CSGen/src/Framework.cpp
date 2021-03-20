
#include "Framework.hpp"

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

} // namespace Framework
} // namespace CSGen