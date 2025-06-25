#ifndef COLOUR_H
#define COLOUR_H

#pragma once

namespace emc {
    using Byte = unsigned char;
    struct Colour {
        unsigned int colour;

        Colour() {
            constexpr Byte red = 0, green = 0, blue = 0, alpha = 255;
            colour = (red << 24) | (green << 16) | (blue << 8) | (alpha << 0);
        }

        Colour(const Byte red, const Byte green, const Byte blue, const Byte alpha) {
            colour = (red << 24) | (green << 16) | (blue << 8) | (alpha << 0);
        }

        [[nodiscard]] Byte GetRed() const { return colour >> 24; }
        [[nodiscard]] Byte GetGreen() const { return colour >> 16; }
        [[nodiscard]] Byte GetBlue() const { return colour >> 8; }
        [[nodiscard]] Byte GetAlpha() const { return colour; }

        void SetRed(const Byte red) {
	        colour = colour & ~(0b11111111 << 24);
            colour = colour | (red << 24);
        }

        void SetGreen(const Byte green) {
            colour = colour & ~(0b11111111 << 16);
            colour = colour | (green << 16);
        }

        void SetBlue(const Byte blue) {
            colour = colour & ~(0b11111111 << 8);
            colour = colour | (blue << 8);
        }

        void SetAlpha(const Byte alpha) {
            colour = colour & ~(0b11111111 << 0);
            colour = colour | alpha;
        }

        bool operator==(const Colour& o) const {
            return colour == o.colour;
        }
    };
}

#endif