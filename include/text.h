#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <utility>
#include "idrawable.h"
#include "iscreen2d.h"

template <typename Color>
class Text : public IDrawable<Color>
{
   private:
    bool is_coords_inited_;
    IScreen2D<Color>::Coordinate x_, y_;
    std::string content_;

    bool is_colors_inited_;
    Color fg_color_, bg_color_;

   public:
    Text() : Text("") {}
    Text(const char *content)
        : is_coords_inited_(false), content_(content), is_colors_inited_(false)
    {
    }

    Text(const char *content, const std::pair<Color, Color> colors)
        : is_coords_inited_(false),
          content_(content),
          is_colors_inited_(true),
          fg_color_(colors.first),
          bg_color_(colors.second)
    {
    }

    Text(const char *content,
         const std::pair<Color, Color> colors,
         const std::pair<typename IScreen2D<Color>::Coordinate,
                         typename IScreen2D<Color>::Coordinate> &coords)
        : is_coords_inited_(true),
          x_(coords.first),
          y_(coords.second),
          content_(content),
          is_colors_inited_(true),
          fg_color_(colors.first),
          bg_color_(colors.second)
    {
    }

    void DrawOn(IScreen2D<Color> &screen) const override
    {
        if (is_coords_inited_)
        {
            screen.SetCursor(x_, y_);
        }
        if (is_colors_inited_)
        {
            screen.SetForegroundColor(fg_color_);
            screen.SetBackgroundColor(bg_color_);
        }
        for (const char ch : content_)
        {
            screen.Draw(ch);
        }
    }

    ~Text() override = default;
};

#endif  // !TEXT_H
