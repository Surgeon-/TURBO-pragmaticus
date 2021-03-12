#ifndef CSGEN_GENERATORBASE_HPP
#define CSGEN_GENERATORBASE_HPP

#include "Common.hpp"

#include <stdexcept>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace CSGen {

class GeneratorBase {
public:
  GeneratorBase(PZInt aWidth_mm, PZInt aHeight_mm, PZInt aPixelsPerMillimetre);

  // file name must end with .bmp, .png, .tga or .jpg
  void dumpCanvasToFile(const std::string& aFileName) const;

protected: // Methods
  float toCanvasCoord(float aCoord_mm) const;
  sf::Vector2f toCanvasCoord(float aX_mm, float aY_mm) const;

  void putHorizontal1pxLine(float aX_mm, float aY_mm, float aWidth_mm);
  void putVertical1pxLine(float aX_mm, float aY_mm, float aHeight_mm);
  void putRect(float aX_mm, float aY_mm, float aBBoxWidth_mm,
               float aBBoxHeight_mm, PZInt aThickness_px);

  void setOutlineColour(sf::Color aColour);
  void setFillColour(sf::Color aColour);

  void finalizeCanvas();
  // clearWithColour

private: // Data
  sf::RenderTexture _canvas;
  PZInt _pixelsPerMillimetre;
  sf::Color _outlineColour{sf::Color::Black};
  sf::Color _fillColour{sf::Color::Transparent};

};

GeneratorBase::GeneratorBase(PZInt aWidth_mm, PZInt aHeight_mm, PZInt aPixelsPerMillimetre)
  : _pixelsPerMillimetre(aPixelsPerMillimetre)
{
  _canvas.create(static_cast<unsigned>(aWidth_mm  * _pixelsPerMillimetre),
                 static_cast<unsigned>(aHeight_mm * _pixelsPerMillimetre));
  _canvas.setSmooth(false);
  _canvas.clear(sf::Color::White);
}

void GeneratorBase::dumpCanvasToFile(const std::string& aFileName) const {
  if (!_canvas.getTexture().copyToImage().saveToFile(aFileName)) {
    throw std::runtime_error("Could not save image to file.");
  }
}

float GeneratorBase::toCanvasCoord(float aCoord_mm) const {
  return static_cast<float>(aCoord_mm * _pixelsPerMillimetre);
}

sf::Vector2f GeneratorBase::toCanvasCoord(float aX_mm, float aY_mm) const {
  return sf::Vector2f{toCanvasCoord(aX_mm), toCanvasCoord(aY_mm)};
}

void GeneratorBase::putHorizontal1pxLine(float aX_mm, float aY_mm, float aWidth_mm) {
  sf::Vertex vertices[2];
  vertices[0].position = toCanvasCoord(aX_mm, aY_mm);
  vertices[0].color = _outlineColour;
  vertices[1].position = toCanvasCoord(aX_mm + aWidth_mm, aY_mm);
  vertices[1].color = _outlineColour;

  _canvas.draw(vertices, 2, sf::PrimitiveType::LineStrip);
}

void GeneratorBase::putVertical1pxLine(float aX_mm, float aY_mm, float aHeight_mm) {
  sf::Vertex vertices[2];
  vertices[0].position = toCanvasCoord(aX_mm, aY_mm);
  vertices[0].color = _outlineColour;
  vertices[1].position = toCanvasCoord(aX_mm, aY_mm + aHeight_mm);
  vertices[1].color = _outlineColour;

  _canvas.draw(vertices, 2, sf::PrimitiveType::LineStrip);
}

void GeneratorBase::putRect(float aX_mm, float aY_mm, float aBBoxWidth_mm,
                            float aBBoxHeight_mm, PZInt aThickness_px) {
  sf::Vertex vertices[5];

  for (auto& vertex : vertices) {
    vertex.color = _outlineColour;
  }

  const auto CORRECTION_OFFSET = 1.f;

  for (PZInt i = 0; i <= aThickness_px; i += 1) {
    vertices[0].position = vertices[4].position =
      sf::Vector2f{toCanvasCoord(aX_mm) + static_cast<float>(i) + CORRECTION_OFFSET,
                   toCanvasCoord(aY_mm) + static_cast<float>(i)};
    vertices[1].position =
      sf::Vector2f{toCanvasCoord(aX_mm + aBBoxWidth_mm) - static_cast<float>(i),
                   toCanvasCoord(aY_mm) + static_cast<float>(i)};
    vertices[2].position =
      sf::Vector2f{toCanvasCoord(aX_mm + aBBoxWidth_mm) - static_cast<float>(i),
                   toCanvasCoord(aY_mm + aBBoxHeight_mm) - static_cast<float>(i) - CORRECTION_OFFSET};
    vertices[3].position =
      sf::Vector2f{toCanvasCoord(aX_mm) + static_cast<float>(i),
                   toCanvasCoord(aY_mm + aBBoxHeight_mm) - static_cast<float>(i) - CORRECTION_OFFSET};

    if (i < aThickness_px) {
      _canvas.draw(vertices, 5, sf::PrimitiveType::LineStrip);
    } else {
      for (auto& vertex : vertices) {
        vertex.color = _fillColour;
      }
      _canvas.draw(vertices, 4, sf::PrimitiveType::Quads);
    }
  } // end outer for loop
}

void GeneratorBase::setOutlineColour(sf::Color aColour) {
  _outlineColour = aColour;
}

void GeneratorBase::setFillColour(sf::Color aColour) {
  _fillColour = aColour;
}

void GeneratorBase::finalizeCanvas() {
    _canvas.display();
}

} // namespace CSGen

#endif // !CSGEN_GENERATORBASE_HPP
