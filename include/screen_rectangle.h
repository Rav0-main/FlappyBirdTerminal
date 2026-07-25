#ifndef SCREEN_RECTANGLE_H
#define SCREEN_RECTANGLE_H

#include "idrawable.h"
#include "iscreen2d.h"

template <typename Color>
class ScreenRectangle : public IDrawable
{
   private:
    IScreen2D<Color> &main_screen_;
    const IScreen2D<Color> &rectangle_screen_;
    const Color color_;

   public:
    ScreenRectangle(IScreen2D<Color> &main_screen,
                    const IScreen2D<Color> &screen_for_rectangle,
                    const Color &color)
        : main_screen_(main_screen), rectangle_screen_(screen_for_rectangle), color_(color)
    {
    }

    void Draw() const noexcept override
    {
        main_screen_.SetForegroundColor(color_);

        typename IScreen2D<Color>::Coordinate start_x =
            static_cast<typename IScreen2D<Color>::Coordinate>(rectangle_screen_.delta_x()) +
            main_screen_.start_val_x();
        typename IScreen2D<Color>::Coordinate start_y =
            static_cast<typename IScreen2D<Color>::Coordinate>(rectangle_screen_.delta_y()) +
            main_screen_.start_val_y();

        if (start_y > 0)
        {
            if (start_x > 0)
            {
                main_screen_.SetCursor(start_x - 1, start_y - 1);
                main_screen_.Draw('+');
            }
            main_screen_.SetCursor(start_x, start_y - 1);
            for (typename IScreen2D<Color>::SizeParam i = 0; i < rectangle_screen_.width(); ++i)
            {
                main_screen_.Draw('-');
            }
            main_screen_.Draw('+');
        }
        for (typename IScreen2D<Color>::SizeParam i = 0; i < rectangle_screen_.height(); ++i)
        {
            if (start_x > 0)
            {
                main_screen_.SetCursor(start_x - 1, start_y + i);
                main_screen_.Draw('|');
            }

            main_screen_.SetCursor(start_x + rectangle_screen_.width(), start_y + i);
            main_screen_.Draw('|');
        }

        if (start_x > 0)
        {
            main_screen_.SetCursor(start_x - 1, start_y + rectangle_screen_.height());
            main_screen_.Draw('+');
        }
        main_screen_.SetCursor(start_x, start_y + rectangle_screen_.height());
        for (typename IScreen2D<Color>::SizeParam i = 0; i < rectangle_screen_.width(); ++i)
        {
            main_screen_.Draw('-');
        }
        main_screen_.Draw('+');
    }
    ~ScreenRectangle() override = default;
};

#endif  // !SCREEN_RECTANGLE_H
