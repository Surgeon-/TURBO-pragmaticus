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
  float toCanvasCoord(PZInt aCoord_mm) const;
  sf::Vector2f toCanvasCoord(PZInt aX_mm, PZInt aY_mm) const;

  void putHorizontal1pxLine(PZInt aX_mm, PZInt aY_mm, PZInt aWidth_mm);
  void putVertical1pxLine(PZInt aX_mm, PZInt aY_mm, PZInt aHeight_mm);
  void putRect(PZInt aX_mm, PZInt aY_mm, PZInt aBBoxWidth_mm,
               PZInt aBBoxHeight_mm, PZInt aThickness_px);

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

float GeneratorBase::toCanvasCoord(PZInt aCoord_mm) const {
  return static_cast<float>(aCoord_mm * _pixelsPerMillimetre);
}

sf::Vector2f GeneratorBase::toCanvasCoord(PZInt aX_mm, PZInt aY_mm) const {
  return sf::Vector2f{toCanvasCoord(aX_mm), toCanvasCoord(aY_mm)};
}

void GeneratorBase::putHorizontal1pxLine(PZInt aX_mm, PZInt aY_mm, PZInt aWidth_mm) {
  sf::RectangleShape rect;
  rect.setPosition(toCanvasCoord(aX_mm, aY_mm));
  rect.setSize({toCanvasCoord(aWidth_mm), 0.f});

  rect.setFillColor(sf::Color::Transparent);
  rect.setOutlineColor(_outlineColour);
  rect.setOutlineThickness(1.f);

  _canvas.draw(rect);
}

void GeneratorBase::putVertical1pxLine(PZInt aX_mm, PZInt aY_mm, PZInt aHeight_mm) {
  sf::RectangleShape rect;
  rect.setFillColor(sf::Color::Transparent);
  rect.setOutlineColor(_outlineColour);
  rect.setOutlineThickness(1.f);

  rect.setPosition(toCanvasCoord(aX_mm, aY_mm));
  rect.setSize({1.f, toCanvasCoord(aHeight_mm)});

  _canvas.draw(rect);
}

void GeneratorBase::putRect(PZInt aX_mm, PZInt aY_mm, PZInt aBBoxWidth_mm,
                            PZInt aBBoxHeight_mm, PZInt aThickness_px) {
  sf::RectangleShape rect;
  rect.setFillColor(_fillColour);
  rect.setOutlineColor(_outlineColour);
  rect.setOutlineThickness(1.f);

  for (PZInt i = 0; i < aThickness_px; i += 1) {
    rect.setPosition({toCanvasCoord(aX_mm) + static_cast<float>(i),
                      toCanvasCoord(aY_mm) + static_cast<float>(i)
                     });
    rect.setSize({toCanvasCoord(aBBoxWidth_mm)  - static_cast<float>(2 * i),
                  toCanvasCoord(aBBoxHeight_mm) - static_cast<float>(2 * i)
                 });
    _canvas.draw(rect);
  }
}

void GeneratorBase::finalizeCanvas() {
    _canvas.display();
}

} // namespace CSGen

#endif // !CSGEN_GENERATORBASE_HPP
