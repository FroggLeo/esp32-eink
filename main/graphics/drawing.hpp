#pragma once

#include "graphics/canvas.hpp"

void draw_rect(Canvas& canvas, int x, int y, int width, int height, bool fill = true, bool white = false);
void draw_sample(Canvas& canvas, int x, int y, bool white = false);