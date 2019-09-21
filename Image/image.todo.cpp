#include "image.h"
#include <stdlib.h>
#include <math.h>
#include <Util/exceptions.h>
#include <algorithm> //ADDED TO USE MAX
#include <iostream> //ADDED FOR TESTING
////////////////////////////
// Image processing stuff //
////////////////////////////
Pixel::Pixel( const Pixel32& p )
{
	Util::Throw( "Pixel::Pixel undefined" );
}
Pixel32::Pixel32( const Pixel& p )
{
	Util::Throw( "Pixel32::Pixel32 undefined" );
}

//MY OWN CODE:
//Program to generate random float number between two limits
float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}
Image32 Image32::addRandomNoise( float noise ) const
{
	if (noise < 0 || noise > 1) {
		Util::Throw("Image32::addRandomNoise undefined for noise outside of range 0 and 1");
	}
	float max_noise = noise;
	float min_noise = 0 - noise;
	Image32 output = Image32(*this);
	for (int x = 0; x < this->width(); x++) 
	{
		for (int y = 0; y < this->height(); y++) 
		{
			Pixel32 temp_in = this->operator() (x,y);
			Pixel32 &temp_out = output.operator() (x,y);
			float rand_noise = RandomFloat(min_noise, max_noise);
			int final_noise = (int)(255 * rand_noise);
			if (final_noise < 0) 
			{
				temp_out.b = (unsigned char)std::max(0, temp_in.b + final_noise);
				temp_out.g = (unsigned char)std::max(0, temp_in.g + final_noise);
				temp_out.r = (unsigned char)std::max(0, temp_in.r + final_noise);
			}
			else 
			{
				temp_out.b = (unsigned char)std::min(255, temp_in.b + final_noise);
				temp_out.g = (unsigned char)std::min(255, temp_in.g + final_noise);
				temp_out.r = (unsigned char)std::min(255, temp_in.r + final_noise);
			}
		}
	}
	return output;
}
Image32 Image32::brighten( float brightness ) const
{
	if (brightness < 0)
	{
		Util::Throw("Image32::brighten undefined for brightness less than 0");
	}
	Image32 output = Image32(*this);
	for (int x = 0; x < this->width(); x++)
	{
		for (int y = 0; y < this->height(); y++)
		{
			Pixel32 temp_in = this->operator() (x, y);
			Pixel32& temp_out = output.operator() (x, y);
			temp_out.b = (unsigned char)std::min(255, (int) (temp_in.b * brightness));
			temp_out.g = (unsigned char)std::min(255, (int) (temp_in.g * brightness));
			temp_out.r = (unsigned char)std::min(255, (int) (temp_in.r * brightness));	
		}
	}
	return output;
}

Image32 Image32::luminance( void ) const
{
	Image32 output = Image32(*this);
	for (int x = 0; x < this->width(); x++)
	{
		for (int y = 0; y < this->height(); y++)
		{
			Pixel32 temp_in = this->operator() (x, y);
			Pixel32& temp_out = output.operator() (x, y);
			unsigned char gray = (unsigned char)((0.30 * temp_in.r) + (0.59 * temp_in.g) + (0.11 * temp_in.b));
			temp_out.b = (unsigned char)std::min(255, (int)gray);
			temp_out.g = (unsigned char)std::min(255, (int)gray);
			temp_out.r = (unsigned char)std::min(255, (int)gray);
		}
	}
	return output;
}

Image32 Image32::contrast( float contrast ) const
{
	if (contrast < 0)
	{
		Util::Throw("Image32::contrast undefined for contrast less than 0");
	}
	Image32 output = Image32(*this);
	int Lp = 0;
	for (int x = 0; x < this->width(); x++)
	{
		for (int y = 0; y < this->height(); y++)
		{
			Pixel32 temp_in = this->operator() (x, y);
			Lp += (int)((.30 * temp_in.r) + (.59 * temp_in.g) + (0.11 * temp_in.b));
		}
	}

	Lp = Lp / (this->width() * this->height());
	for (int x = 0; x < this->width(); x++)
	{
		for (int y = 0; y < this->height(); y++)
		{
			Pixel32 temp_in = this->operator() (x, y);
			Pixel32& temp_out = output.operator() (x, y);
			temp_out.b = (unsigned char)std::max(0, std::min(255, (int)((temp_in.b - Lp) * contrast + Lp)));
			temp_out.g = (unsigned char)std::max(0, std::min(255, (int)((temp_in.g - Lp) * contrast + Lp)));
			temp_out.r = (unsigned char)std::max(0, std::min(255, (int)((temp_in.r - Lp) * contrast + Lp)));
		}
	}
	return output;
}

Image32 Image32::saturate( float saturation ) const
{
	if (saturation < 0)
	{
		Util::Throw("Image32::saturation undefined for saturation less than 0");
	}
	Image32 output = Image32(*this);
	for (int x = 0; x < this->width(); x++)
	{
		for (int y = 0; y < this->height(); y++)
		{
			Pixel32 temp_in = this->operator() (x, y);
			Pixel32& temp_out = output.operator() (x, y);
			int Lp = (int)((.30 * temp_in.r) + (.59 * temp_in.g) + (0.11 * temp_in.b));
			temp_out.b = (unsigned char)std::max(0, std::min(255, (int)((temp_in.b - Lp) * saturation + Lp)));
			temp_out.g = (unsigned char)std::max(0, std::min(255, (int)((temp_in.g - Lp) * saturation + Lp)));
			temp_out.r = (unsigned char)std::max(0, std::min(255, (int)((temp_in.r - Lp) * saturation + Lp)));
		}
	}
	return output;
}

Image32 Image32::quantize( int bits ) const
{
	if (bits < 1)
	{
		Util::Throw("Image32::quantize undefined for bits less than 1");
	}
	Image32 output = Image32(*this);
	for (int x = 0; x < this->width(); x++)
	{
		for (int y = 0; y < this->height(); y++)
		{
			Pixel32 temp_in = this->operator() (x, y);
			Pixel32& temp_out = output.operator() (x, y);
			double mult = pow(2, bits);
			int tempb = (int)(floor((temp_in.b / 255.0) * mult) * 255.0);
			int tempg = (int)(floor((temp_in.g / 255.0) * mult) * 255.0);
			int tempr = (int)(floor((temp_in.r / 255.0) * mult) * 255.0);
			temp_out.b = (unsigned char)std::max(0, std::min(255, tempb));
			temp_out.g = (unsigned char)std::max(0, std::min(255, tempg));
			temp_out.r = (unsigned char)std::max(0, std::min(255, tempr));
		}
	}
	return output;
}

Image32 Image32::randomDither(int bits) const
{
	if (bits < 1)
	{
		Util::Throw("Image32::randomDither undefined for bits < 1");
	}
	Image32 output = Image32(*this);
	for (int x = 0; x < this->width(); x++)
	{
		for (int y = 0; y < this->height(); y++)
		{
			Pixel32 temp_in = this->operator() (x, y);
			Pixel32& temp_out = output.operator() (x, y);
			double mult = pow(2, bits);
			double rand_noise = RandomFloat(-1, 1) / mult;
			int final_noise = (int)(255 * rand_noise);
			int tempb = (int)(floor((((double)temp_in.b + final_noise) / 255.0) * mult) * 255.0);
			int tempg = (int)(floor((((double)temp_in.g + final_noise) / 255.0) * mult) * 255.0);
			int tempr = (int)(floor((((double)temp_in.r + final_noise) / 255.0) * mult) * 255.0);

			temp_out.b = (unsigned char)std::max(0, std::min(255, tempb));
			temp_out.g = (unsigned char)std::max(0, std::min(255, tempg));
			temp_out.r = (unsigned char)std::max(0, std::min(255, tempr));
		}
	}
	return output;
}

Image32 Image32::orderedDither2X2( int bits ) const
{
	if (bits < 1) 
	{
		Util::Throw("Image32::orderedDither2x2 undefined");
	}
	int D2[2][2] = { {1,3} , {4,2} };
	int n = 2;
	Image32 output = Image32(*this);
	for (int x = 0; x < this->width(); x++)
	{
		for (int y = 0; y < this->height(); y++)
		{
			Pixel32 temp_in = this->operator() (x, y);
			Pixel32& temp_out = output.operator() (x, y);
			int i = x % n;
			int j = y % n;
			float rc = ((float)temp_in.r / 255.0) * (pow(2, bits) - 1);
			float gc = ((float)temp_in.g / 255.0) * (pow(2, bits) - 1);
			float bc = ((float)temp_in.b / 255.0) * (pow(2, bits) - 1);
			float re = rc - floor(rc);
			float ge = gc - floor(gc);
			float be = bc - floor(bc);
			if (re > ((float)D2[i][j] / ((float)pow(n, 2) + 1))) {
				temp_out.r = ceil(rc) * 255;
			}
			else {
				temp_out.r = floor(rc) * 255;
			}
			if (ge > ((float)D2[i][j] / ((float)pow(n, 2) + 1))) {
				temp_out.g = ceil(gc) * 255;
			}
			else {
				temp_out.g = floor(gc) * 255;
			}
			if (be > ((float)D2[i][j] / ((float)pow(n, 2) + 1))) {
				temp_out.b = ceil(bc) * 255;
			}
			else {
				temp_out.b = floor(bc) * 255;
			}
		}
	}
	return output;
}

Image32 Image32::floydSteinbergDither( int bits ) const
{
	if (bits < 1) 
	{
		Util::Throw("Image32::floydSteinbergDither undefined for bits < 1");
	}
	Image32 copy = Image32(*this);
	Image32 output = Image32(*this);
	float a = 7.0 / 16.0;
	float b = 3.0 / 16.0;
	float c = 5.0 / 16.0;
	float d = 1.0 / 16.0;
	for (int x = 0; x < this->width(); x++)
	{
		for (int y = 0; y < this->height(); y++)
		{
			Pixel32 temp_in = copy.operator() (x, y);
			Pixel32& temp_out = output.operator() (x, y);
			double mult = pow(2, bits);
			int tempb = (int)(floor((temp_in.b / 255.0) * mult) * 255.0);
			int tempg = (int)(floor((temp_in.g / 255.0) * mult) * 255.0);
			int tempr = (int)(floor((temp_in.r / 255.0) * mult) * 255.0);
			temp_out.b = (unsigned char)std::max(0, std::min(255, tempb));
			temp_out.g = (unsigned char)std::max(0, std::min(255, tempg));
			temp_out.r = (unsigned char)std::max(0, std::min(255, tempr));
			int errb = temp_in.b - temp_out.b;
			int errg = temp_in.g - temp_out.g;
			int errr = temp_in.r - temp_out.r;
			if (x != this->width() - 1)
			{
				Pixel32& temp_1 = copy.operator() (x + 1, y);
				temp_1.r = (unsigned char)std::max(0, std::min(255, (int) (temp_1.r + errr * a)));
				temp_1.b = (unsigned char)std::max(0, std::min(255, (int) (temp_1.b + errb * a)));
				temp_1.g = (unsigned char)std::max(0, std::min(255, (int) (temp_1.g + errg * a)));
				if (y != this->height() - 1)
				{
					Pixel32& temp_2 = copy.operator() (x + 1, y + 1);
					temp_2.r = (unsigned char)std::max(0, std::min(255, (int)(temp_2.r + errr * d)));
					temp_2.b = (unsigned char)std::max(0, std::min(255, (int)(temp_2.b + errb * d)));
					temp_2.g = (unsigned char)std::max(0, std::min(255, (int)(temp_2.g + errg * d)));
				}
			}
			if (y != this->height() - 1)
			{
				Pixel32& temp_3 = copy.operator() (x , y + 1);
				temp_3.r = (unsigned char)std::max(0, std::min(255, (int)(temp_3.r + errr * c)));
				temp_3.b = (unsigned char)std::max(0, std::min(255, (int)(temp_3.b + errb * c)));
				temp_3.g = (unsigned char)std::max(0, std::min(255, (int)(temp_3.g + errg * c)));
				if (x != 0)
				{
					Pixel32& temp_4 = copy.operator() (x - 1, y + 1);
					temp_4.r = (unsigned char)std::max(0, std::min(255, (int)(temp_4.r + errr * b)));
					temp_4.b = (unsigned char)std::max(0, std::min(255, (int)(temp_4.b + errb * b)));
					temp_4.g = (unsigned char)std::max(0, std::min(255, (int)(temp_4.g + errg * b)));
				}
			}
		}
	}
	return output;
}

Image32 Image32::blur3X3( void ) const
{
	//Util::Throw( "Image32::blur3X3 undefined" );
	Image32 output = Image32(*this);
	for (int x = 0; x < this->width(); x++)
	{
		for (int y = 0; y < this->height(); y++)
		{
			Pixel32 temp_in = this->operator() (x, y);
			Pixel32& temp_out = output.operator() (x, y);
			if (x == 0 && y == 0) 
			{ 
				Pixel32 temp_r = this->operator() (x + 1, y);
				Pixel32 temp_br = this->operator() (x + 1, y + 1);
				Pixel32 temp_b = this->operator() (x, y + 1);
				temp_out.r = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 9.0 * temp_in.r) + (2.0 / 9.0 * temp_r.r) + (2.0 / 9.0 * temp_b.r) + (1.0 / 9.0 * temp_br.r))));
				temp_out.b = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 9.0 * temp_in.b) + (2.0 / 9.0 * temp_r.b) + (2.0 / 9.0 * temp_b.b) + (1.0 / 9.0 * temp_br.b))));
				temp_out.g = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 9.0 * temp_in.g) + (2.0 / 9.0 * temp_r.g) + (2.0 / 9.0 * temp_b.g) + (1.0 / 9.0 * temp_br.g))));
			}
			else if (x == this->width() - 1 && y == this->height() - 1 ) 
			{
				Pixel32 temp_l = this->operator() (x - 1, y);
				Pixel32 temp_tl = this->operator() (x - 1, y - 1);
				Pixel32 temp_t = this->operator() (x, y - 1);
				temp_out.r = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 9.0 * temp_in.r) + (2.0 / 9.0 * temp_l.r) + (2.0 / 9.0 * temp_t.r) + (1.0 / 9.0 * temp_tl.r))));
				temp_out.b = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 9.0 * temp_in.b) + (2.0 / 9.0 * temp_l.b) + (2.0 / 9.0 * temp_t.b) + (1.0 / 9.0 * temp_tl.b))));
				temp_out.g = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 9.0 * temp_in.g) + (2.0 / 9.0 * temp_l.g) + (2.0 / 9.0 * temp_t.g) + (1.0 / 9.0 * temp_tl.g))));
			}
			else if (x == 0 && y == this->height() - 1 ) 
			{
				Pixel32 temp_r = this->operator() (x + 1, y);
				Pixel32 temp_tr = this->operator() (x + 1, y - 1);
				Pixel32 temp_t = this->operator() (x, y - 1);
				temp_out.r = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 9.0 * temp_in.r) + (2.0 / 9.0 * temp_r.r) + (2.0 / 9.0 * temp_t.r) + (1.0 / 9.0 * temp_tr.r))));
				temp_out.b = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 9.0 * temp_in.b) + (2.0 / 9.0 * temp_r.b) + (2.0 / 9.0 * temp_t.b) + (1.0 / 9.0 * temp_tr.b))));
				temp_out.g = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 9.0 * temp_in.g) + (2.0 / 9.0 * temp_r.g) + (2.0 / 9.0 * temp_t.g) + (1.0 / 9.0 * temp_tr.g))));
			}
			else if (x == this->width() - 1 && y == 0 )
			{
				Pixel32 temp_l = this->operator() (x - 1, y);
				Pixel32 temp_bl = this->operator() (x - 1, y + 1);
				Pixel32 temp_b = this->operator() (x, y + 1);
				temp_out.r = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 9.0 * temp_in.r) + (2.0 / 9.0 * temp_l.r) + (2.0 / 9.0 * temp_b.r) + (1.0 / 9.0 * temp_bl.r))));
				temp_out.b = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 9.0 * temp_in.b) + (2.0 / 9.0 * temp_l.b) + (2.0 / 9.0 * temp_b.b) + (1.0 / 9.0 * temp_bl.b))));
				temp_out.g = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 9.0 * temp_in.g) + (2.0 / 9.0 * temp_l.g) + (2.0 / 9.0 * temp_b.g) + (1.0 / 9.0 * temp_bl.g))));
			}
			else if (x == 0)
			{
				Pixel32 temp_t = this->operator() (x , y - 1);
				Pixel32 temp_tr = this->operator() (x + 1, y - 1);
				Pixel32 temp_r = this->operator() (x + 1, y);
				Pixel32 temp_br = this->operator() (x + 1, y + 1);
				Pixel32 temp_b = this->operator() (x, y + 1);
				temp_out.r = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 12.0 * temp_in.r) + (2.0 / 12.0 * temp_r.r) + (2.0 / 12.0 * temp_b.r) + (2.0 / 12.0 * temp_t.r) + (1.0 / 12.0 * temp_tr.r) + (1.0 / 12.0 * temp_br.r))));
				temp_out.b = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 12.0 * temp_in.b) + (2.0 / 12.0 * temp_r.b) + (2.0 / 12.0 * temp_b.b) + (2.0 / 12.0 * temp_t.b) + (1.0 / 12.0 * temp_tr.b) + (1.0 / 12.0 * temp_br.b))));
				temp_out.g = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 12.0 * temp_in.g) + (2.0 / 12.0 * temp_r.g) + (2.0 / 12.0 * temp_b.g) + (2.0 / 12.0 * temp_t.g) + (1.0 / 12.0 * temp_tr.g) + (1.0 / 12.0 * temp_br.g))));
			}
			else if (x == this->width() - 1)
			{
				Pixel32 temp_t = this->operator() (x, y - 1);
				Pixel32 temp_tl = this->operator() (x - 1, y - 1);
				Pixel32 temp_l = this->operator() (x - 1, y);
				Pixel32 temp_bl = this->operator() (x - 1, y + 1);
				Pixel32 temp_b = this->operator() (x, y + 1);
				temp_out.r = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 12.0 * temp_in.r) + (2.0 / 12.0 * temp_l.r) + (2.0 / 12.0 * temp_b.r) + (2.0 / 12.0 * temp_t.r) + (1.0 / 12.0 * temp_tl.r) + (1.0 / 12.0 * temp_bl.r))));
				temp_out.b = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 12.0 * temp_in.b) + (2.0 / 12.0 * temp_l.b) + (2.0 / 12.0 * temp_b.b) + (2.0 / 12.0 * temp_t.b) + (1.0 / 12.0 * temp_tl.b) + (1.0 / 12.0 * temp_bl.b))));
				temp_out.g = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 12.0 * temp_in.g) + (2.0 / 12.0 * temp_l.g) + (2.0 / 12.0 * temp_b.g) + (2.0 / 12.0 * temp_t.g) + (1.0 / 12.0 * temp_tl.g) + (1.0 / 12.0 * temp_bl.g))));
			}
			else if (y == 0)
			{
				Pixel32 temp_l = this->operator() (x - 1, y );
				Pixel32 temp_br = this->operator() (x + 1, y + 1);
				Pixel32 temp_r = this->operator() (x + 1, y);
				Pixel32 temp_bl = this->operator() (x - 1, y + 1);
				Pixel32 temp_b = this->operator() (x, y + 1);
				temp_out.r = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 12.0 * temp_in.r) + (2.0 / 12.0 * temp_r.r) + (2.0 / 12.0 * temp_b.r) + (2.0 / 12.0 * temp_b.r) + (1.0 / 12.0 * temp_bl.r) + (1.0 / 12.0 * temp_br.r))));
				temp_out.b = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 12.0 * temp_in.b) + (2.0 / 12.0 * temp_r.b) + (2.0 / 12.0 * temp_b.b) + (2.0 / 12.0 * temp_b.b) + (1.0 / 12.0 * temp_bl.b) + (1.0 / 12.0 * temp_br.b))));
				temp_out.g = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 12.0 * temp_in.g) + (2.0 / 12.0 * temp_r.g) + (2.0 / 12.0 * temp_b.g) + (2.0 / 12.0 * temp_b.g) + (1.0 / 12.0 * temp_bl.g) + (1.0 / 12.0 * temp_br.g))));
			}
			else if (y == this->height() - 1)
			{
				Pixel32 temp_l = this->operator() (x - 1, y);
				Pixel32 temp_tr = this->operator() (x + 1, y - 1);
				Pixel32 temp_r = this->operator() (x + 1, y);
				Pixel32 temp_tl = this->operator() (x - 1, y - 1);
				Pixel32 temp_t = this->operator() (x, y - 1);
				temp_out.r = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 12.0 * temp_in.r) + (2.0 / 12.0 * temp_r.r) + (2.0 / 12.0 * temp_t.r) + (2.0 / 12.0 * temp_l.r) + (1.0 / 12.0 * temp_tl.r) + (1.0 / 12.0 * temp_tr.r))));
				temp_out.b = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 12.0 * temp_in.b) + (2.0 / 12.0 * temp_r.b) + (2.0 / 12.0 * temp_t.b) + (2.0 / 12.0 * temp_l.b) + (1.0 / 12.0 * temp_tl.b) + (1.0 / 12.0 * temp_tr.b))));
				temp_out.g = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 12.0 * temp_in.g) + (2.0 / 12.0 * temp_r.g) + (2.0 / 12.0 * temp_t.g) + (2.0 / 12.0 * temp_l.g) + (1.0 / 12.0 * temp_tl.g) + (1.0 / 12.0 * temp_tr.g))));
			}
			else {
				Pixel32 temp_l = this->operator() (x - 1, y);
				Pixel32 temp_tr = this->operator() (x + 1, y - 1);
				Pixel32 temp_r = this->operator() (x + 1, y);
				Pixel32 temp_tl = this->operator() (x - 1, y - 1);
				Pixel32 temp_t = this->operator() (x, y - 1);
				Pixel32 temp_br = this->operator() (x + 1, y + 1);
				Pixel32 temp_b = this->operator() (x , y + 1);
				Pixel32 temp_bl = this->operator() (x - 1, y + 1);
				temp_out.r = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 16.0 * temp_in.r) + (2.0 / 16.0 * temp_r.r) + (2.0 / 16.0 * temp_l.r) + (2.0 / 16.0 * temp_t.r) + (2.0 / 16.0 * temp_b.r) + (1.0 / 12.0 * temp_tl.r) + (1.0 / 12.0 * temp_bl.r) + (1.0 / 12.0 * temp_tr.r) + (1.0 / 12.0 * temp_br.r))));
				temp_out.g = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 16.0 * temp_in.g) + (2.0 / 16.0 * temp_r.g) + (2.0 / 16.0 * temp_l.g) + (2.0 / 16.0 * temp_t.g) + (2.0 / 16.0 * temp_b.g) + (1.0 / 12.0 * temp_tl.g) + (1.0 / 12.0 * temp_bl.g) + (1.0 / 12.0 * temp_tr.g) + (1.0 / 12.0 * temp_br.g))));
				temp_out.b = (unsigned char)std::max(0, std::min(255, (int)((4.0 / 16.0 * temp_in.b) + (2.0 / 16.0 * temp_r.b) + (2.0 / 16.0 * temp_l.b) + (2.0 / 16.0 * temp_t.b) + (2.0 / 16.0 * temp_b.b) + (1.0 / 12.0 * temp_tl.b) + (1.0 / 12.0 * temp_bl.b) + (1.0 / 12.0 * temp_tr.b) + (1.0 / 12.0 * temp_br.b))));
			}
		}
	}
	return output;
}

Image32 Image32::edgeDetect3X3( void ) const
{	
	//SOMETHING WAY TOO DARK HERE - NEED HELP
	//Util::Throw( "Image32::edgeDetect3X3 undefined" );
	
	Image32 output = Image32(*this);
	for (int x = 0; x < this->width(); x++)
	{
		for (int y = 0; y < this->height(); y++)
		{
			Pixel32 temp_in = this->operator() (x, y);
			Pixel32& temp_out = output.operator() (x, y);
			if (x == 0 && y == 0)
			{
				Pixel32 temp_r = this->operator() (x + 1, y);
				Pixel32 temp_br = this->operator() (x + 1, y + 1);
				Pixel32 temp_b = this->operator() (x, y + 1);
				temp_out.r = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.r) + (-1.0 / 3.0 * temp_r.r) + (-1.0 / 3.0 * temp_b.r) + (-1.0 / 3.0 * temp_br.r))));
				temp_out.b = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.b) + (-1.0 / 3.0 * temp_r.b) + (-1.0 / 3.0 * temp_b.b) + (-1.0 / 3.0 * temp_br.b))));
				temp_out.g = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.g) + (-1.0 / 3.0 * temp_r.g) + (-1.0 / 3.0 * temp_b.g) + (-1.0 / 3.0 * temp_br.g))));
			}
			else if (x == this->width() - 1 && y == this->height() - 1)
			{
				Pixel32 temp_l = this->operator() (x - 1, y);
				Pixel32 temp_tl = this->operator() (x - 1, y - 1);
				Pixel32 temp_t = this->operator() (x, y - 1);
				temp_out.r = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.r) + (-1.0 / 3.0 * temp_l.r) + (-1.0 / 3.0 * temp_t.r) + (-1.0 / 3.0 * temp_tl.r))));
				temp_out.b = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.b) + (-1.0 / 3.0 * temp_l.b) + (-1.0 / 3.0 * temp_t.b) + (-1.0 / 3.0 * temp_tl.b))));
				temp_out.g = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.g) + (-1.0 / 3.0 * temp_l.g) + (-1.0 / 3.0 * temp_t.g) + (-1.0 / 3.0 * temp_tl.g))));
			}
			else if (x == 0 && y == this->height() - 1)
			{
				Pixel32 temp_r = this->operator() (x + 1, y);
				Pixel32 temp_tr = this->operator() (x + 1, y - 1);
				Pixel32 temp_t = this->operator() (x, y - 1);
				temp_out.r = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.r) + (-1.0 / 3.0 * temp_r.r) + (-1.0 / 3.0 * temp_t.r) + (-1.0 / 3.0 * temp_tr.r))));
				temp_out.b = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.b) + (-1.0 / 3.0 * temp_r.b) + (-1.0 / 3.0 * temp_t.b) + (-1.0 / 3.0 * temp_tr.b))));
				temp_out.g = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.g) + (-1.0 / 3.0 * temp_r.g) + (-1.0 / 3.0 * temp_t.g) + (-1.0 / 3.0 * temp_tr.g))));
			}
			else if (x == this->width() - 1 && y == 0)
			{
				Pixel32 temp_l = this->operator() (x - 1, y);
				Pixel32 temp_bl = this->operator() (x - 1, y + 1);
				Pixel32 temp_b = this->operator() (x, y + 1);
				temp_out.r = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.r) + (-1.0 / 3.0 * temp_l.r) + (-1.0 / 3.0 * temp_b.r) + (-1.0 / 3.0 * temp_bl.r))));
				temp_out.b = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.b) + (-1.0 / 3.0 * temp_l.b) + (-1.0 / 3.0 * temp_b.b) + (-1.0 / 3.0 * temp_bl.b))));
				temp_out.g = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.g) + (-1.0 / 3.0 * temp_l.g) + (-1.0 / 3.0 * temp_b.g) + (-1.0 / 3.0 * temp_bl.g))));
			}
			else if (x == 0)
			{
				Pixel32 temp_t = this->operator() (x, y - 1);
				Pixel32 temp_tr = this->operator() (x + 1, y - 1);
				Pixel32 temp_r = this->operator() (x + 1, y);
				Pixel32 temp_br = this->operator() (x + 1, y + 1);
				Pixel32 temp_b = this->operator() (x, y + 1);
				temp_out.r = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.r) + (-1.0 / 5.0 * temp_r.r) + (-1.0 / 5.0 * temp_b.r) + (-1.0 / 5.0 * temp_t.r) + (-1.0 / 5.0 * temp_tr.r) + (-1.0 / 5.0 * temp_br.r))));
				temp_out.b = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.b) + (-1.0 / 5.0 * temp_r.b) + (-1.0 / 5.0 * temp_b.b) + (-1.0 / 5.0 * temp_t.b) + (-1.0 / 5.0 * temp_tr.b) + (-1.0 / 5.0 * temp_br.b))));
				temp_out.g = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.g) + (-1.0 / 5.0 * temp_r.g) + (-1.0 / 5.0 * temp_b.g) + (-1.0 / 5.0 * temp_t.g) + (-1.0 / 5.0 * temp_tr.g) + (-1.0 / 5.0 * temp_br.g))));
			}
			else if (x == this->width() - 1)
			{
				Pixel32 temp_t = this->operator() (x, y - 1);
				Pixel32 temp_tl = this->operator() (x - 1, y - 1);
				Pixel32 temp_l = this->operator() (x - 1, y);
				Pixel32 temp_bl = this->operator() (x - 1, y + 1);
				Pixel32 temp_b = this->operator() (x, y + 1);
				temp_out.r = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.r) + (-1.0 / 5.0 * temp_l.r) + (-1.0 / 5.0 * temp_b.r) + (-1.0 / 5.0 * temp_t.r) + (-1.0 / 5.0 * temp_tl.r) + (-1.0 / 5.0 * temp_bl.r))));
				temp_out.b = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.b) + (-1.0 / 5.0 * temp_l.b) + (-1.0 / 5.0 * temp_b.b) + (-1.0 / 5.0 * temp_t.b) + (-1.0 / 5.0 * temp_tl.b) + (-1.0 / 5.0 * temp_bl.b))));
				temp_out.g = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.g) + (-1.0 / 5.0 * temp_l.g) + (-1.0 / 5.0 * temp_b.g) + (-1.0 / 5.0 * temp_t.g) + (-1.0 / 5.0 * temp_tl.g) + (-1.0 / 5.0 * temp_bl.g))));
			}
			else if (y == 0)
			{
				Pixel32 temp_l = this->operator() (x - 1, y);
				Pixel32 temp_br = this->operator() (x + 1, y + 1);
				Pixel32 temp_r = this->operator() (x + 1, y);
				Pixel32 temp_bl = this->operator() (x - 1, y + 1);
				Pixel32 temp_b = this->operator() (x, y + 1);
				temp_out.r = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.r) + (-1.0 / 5.0 * temp_r.r) + (-1.0 / 5.0 * temp_b.r) + (-1.0 / 5.0 * temp_b.r) + (-1.0 / 5.0 * temp_bl.r) + (-1.0 / 5.0 * temp_br.r))));
				temp_out.b = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.b) + (-1.0 / 5.0 * temp_r.b) + (-1.0 / 5.0 * temp_b.b) + (-1.0 / 5.0 * temp_b.b) + (-1.0 / 5.0 * temp_bl.b) + (-1.0 / 5.0 * temp_br.b))));
				temp_out.g = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.g) + (-1.0 / 5.0 * temp_r.g) + (-1.0 / 5.0 * temp_b.g) + (-1.0 / 5.0 * temp_b.g) + (-1.0 / 5.0 * temp_bl.g) + (-1.0 / 5.0 * temp_br.g))));
			}
			else if (y == this->height() - 1)
			{
				Pixel32 temp_l = this->operator() (x - 1, y);
				Pixel32 temp_tr = this->operator() (x + 1, y - 1);
				Pixel32 temp_r = this->operator() (x + 1, y);
				Pixel32 temp_tl = this->operator() (x - 1, y - 1);
				Pixel32 temp_t = this->operator() (x, y - 1);
				temp_out.r = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.r) + (-1.0 / 5.0 * temp_r.r) + (-1.0 / 5.0 * temp_t.r) + (-1.0 / 5.0 * temp_l.r) + (-1.0 / 5.0 * temp_tl.r) + (-1.0 / 5.0 * temp_tr.r))));
				temp_out.b = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.b) + (-1.0 / 5.0 * temp_r.b) + (-1.0 / 5.0 * temp_t.b) + (-1.0 / 5.0 * temp_l.b) + (-1.0 / 5.0 * temp_tl.b) + (-1.0 / 5.0 * temp_tr.b))));
				temp_out.g = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.g) + (-1.0 / 5.0 * temp_r.g) + (-1.0 / 5.0 * temp_t.g) + (-1.0 / 5.0 * temp_l.g) + (-1.0 / 5.0 * temp_tl.g) + (-1.0 / 5.0 * temp_tr.g))));
			}
			else {
				Pixel32 temp_l = this->operator() (x - 1, y);
				Pixel32 temp_tr = this->operator() (x + 1, y - 1);
				Pixel32 temp_r = this->operator() (x + 1, y);
				Pixel32 temp_tl = this->operator() (x - 1, y - 1);
				Pixel32 temp_t = this->operator() (x, y - 1);
				Pixel32 temp_br = this->operator() (x + 1, y + 1);
				Pixel32 temp_b = this->operator() (x, y + 1);
				Pixel32 temp_bl = this->operator() (x - 1, y + 1);
				temp_out.r = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.r) + (-1.0 / 8.0 * temp_r.r) + (-1.0 / 8.0 * temp_l.r) + (-1.0 / 8.0 * temp_t.r) + (-1.0 / 8.0 * temp_b.r) + (-1.0 / 8.0 * temp_tl.r) + (-1.0 / 8.0 * temp_bl.r) + (-1.0 / 8.0 * temp_tr.r) + (-1.0 / 8.0 * temp_br.r))));
				temp_out.g = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.g) + (-1.0 / 8.0 * temp_r.g) + (-1.0 / 8.0 * temp_l.g) + (-1.0 / 8.0 * temp_t.g) + (-1.0 / 8.0 * temp_b.g) + (-1.0 / 8.0 * temp_tl.g) + (-1.0 / 8.0 * temp_bl.g) + (-1.0 / 8.0 * temp_tr.g) + (-1.0 / 8.0 * temp_br.g))));
				temp_out.b = (unsigned char)std::max(0, std::min(255, (int)((1.0 * temp_in.b) + (-1.0 / 8.0 * temp_r.b) + (-1.0 / 8.0 * temp_l.b) + (-1.0 / 8.0 * temp_t.b) + (-1.0 / 8.0 * temp_b.b) + (-1.0 / 8.0 * temp_tl.b) + (-1.0 / 8.0 * temp_bl.b) + (-1.0 / 8.0 * temp_tr.b) + (-1.0 / 8.0 * temp_br.b))));
			}
		}
	}
	return output.brighten(10);
}
Image32 Image32::scaleNearest( float scaleFactor ) const
{
	if (scaleFactor <= 0) {
		Util::Throw("Image32::scaleNearest undefined");
	}
	Image32 output = Image32();
	output.setSize((int)(floor(scaleFactor * (float)(this->width()) + 0.5)), (int)(floor(scaleFactor * (float)(this->height()) + 0.5)));
	for (int x = 0; x < output.width() ; x++)
	{
		for (int y = 0; y < output.height() ; y++)
		{
			Pixel32& temp_out = output.operator() (x, y);
			Pixel32 temp = this->nearestSample(x / scaleFactor - 0.5, y / scaleFactor - 0.5);
			temp_out = temp;
		}
	}
	return output;
	
}

Image32 Image32::scaleBilinear( float scaleFactor ) const
{
	if (scaleFactor <= 0) {
		Util::Throw("Image32::scaleBilinear undefined");
	}
	Image32 output = Image32();
	output.setSize((int)(floor(scaleFactor * (float)(this->width()) + 0.5)), (int)(floor(scaleFactor * (float)(this->height()) + 0.5)));
	for (int x = 0; x < output.width(); x++)
	{
		for (int y = 0; y < output.height(); y++)
		{
			Pixel32& temp_out = output.operator() (x, y);
			Pixel32 temp = this->bilinearSample(x / scaleFactor - 0.5, y / scaleFactor - 0.5);
			temp_out = temp;
		}
	}
	return output;
}

Image32 Image32::scaleGaussian( float scaleFactor ) const
{
	Util::Throw( "Image32::scaleGaussian undefined" );
	return Image32();
}

Image32 Image32::rotateNearest( float angle ) const
{
	Util::Throw( "Image32::rotateNearest undefined" );
	return Image32();
}

Image32 Image32::rotateBilinear( float angle ) const
{
	Util::Throw( "Image32::rotateBilinear undefined" );
	return Image32();
}
	
Image32 Image32::rotateGaussian( float angle ) const
{
	Util::Throw( "Image32::rotateGaussian undefined" );
	return Image32();
}

void Image32::setAlpha( const Image32& matte )
{
	Util::Throw( "Image32::setAlpha undefined" );
	return;
}

Image32 Image32::composite( const Image32& overlay ) const
{
	Util::Throw( "Image32::composite undefined" );
	return Image32();
}

Image32 Image32::CrossDissolve( const Image32& source , const Image32& destination , float blendWeight )
{
	Util::Throw( "Image32::CrossDissolve undefined" );
	return Image32();
}
Image32 Image32::warp( const OrientedLineSegmentPairs& olsp ) const
{
	Util::Throw( "Image32::warp undefined" );
	return Image32();
}

Image32 Image32::funFilter( void ) const
{
	Util::Throw( "Image32::funFilter undefined" );
	return Image32();
}
Image32 Image32::crop( int x1 , int y1 , int x2 , int y2 ) const
{
	if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0 || x1 > this->width() || x2 > this->width() || y1 > this->height() || y2 > this->height() || ((x2 < x1) && (y2 > y1)) || ((x2 < x1) && (y2 < y1)))
	{
		Util::Throw("Image32::crop undefined for pixels of these coordinates");
	}
	Image32 output;
	if (x2 < x1) {
		int tempx = x2;
		int tempy = y2;
		x2 = x1;
		x1 = tempx;
		y2 = y1;
		y1 = tempy;
	}
	output.setSize(abs(x2 - x1 ) + 1, abs(y2 - y1) + 1);
	for (int x = x1; x <= x2; x++)
	{
		for (int y = y1; y <= y2; y++)
		{
			Pixel32 temp_in = this->operator() (x, y);
			Pixel32& temp_out = output.operator() (x - x1, y - y1);
			temp_out = temp_in;
		}
	}
	return output;
}

Pixel32 Image32::nearestSample( float u , float v ) const
{
	if (u < -0.5 || u > this->width() - 0.5 || v < -0.5 || v > this->height() - 0.5 ) {
	//if ( u > this->width() - 0.5 ||  v > this->height() - 0.5) {
		//std::cout << u << " " << v << std::endl;
		Util::Throw("Image32::nearestSample undefined");

	}
	int iu = floor(u + 0.5);
	int iv = floor(v + 0.5);
	Pixel32 input = this->operator() (iu, iv);
	return input;
}


Pixel32 Image32::bilinearSample( float u , float v ) const
{
	if (u < -0.5 || u > this->width() - 0.5|| v < -0.5 || v > this->height() - 0.5) {
		//Util::Throw("Image32::bilinearSample undefined");
		//Return a black pixel if out of bounds to accomodate for edges when doing the scaling
		Pixel32 output = Pixel32();
		output.r = 0;
		output.g = 0;
		output.b = 0;
		return output;
	}
	//NOTE: I Return a black pixel if off of image - HELPS WITH bilinearScale
	Pixel32 black = Pixel32();
	black.r = 0;
	black.g = 0;
	black.b = 0;
	float u1 = floor(u);
	float u2 = u1 + 1;
	float v1 = floor(v);
	float v2 = v1 + 1;
	float du = u - u1;
	float dv = v - v1;
	Pixel32 pix1;
	Pixel32 pix2;
	Pixel32 pix3;
	Pixel32 pix4;
	if (u1 < -0.5 || u1 > this->width() - 0.5 || v1 < -0.5 || v1 > this->height() - 0.5) {
		//Return a black pixel if out of bounds to accomodate for edges when doing the scaling
		pix1 = black;
	}
	else
	{
		pix1 = this->operator() (u1, v1);
	}
	if (u2 < -0.5 || u2 > this->width() - 0.5 || v1 < -0.5 || v1 > this->height() - 0.5) {
		//Return a black pixel if out of bounds to accomodate for edges when doing the scaling
		pix2 = black;
	}
	else
	{
		pix2 = this->operator() (u2, v1);
	}
	if (u1 < -0.5 || u1 > this->width() - 0.5 || v2 < -0.5 || v2 > this->height() - 0.5) {
		//Return a black pixel if out of bounds to accomodate for edges when doing the scaling
		pix3 = black;
	}
	else
	{
		pix3 = this->operator() (u1, v2);
	}
	if (u2 < -0.5 || u2 > this->width() - 0.5 || v2 < -0.5 || v2 > this->height() - 0.5) {
		//Return a black pixel if out of bounds to accomodate for edges when doing the scaling
		pix4 = black;
	}
	else
	{
		pix4 = this->operator() (u2, v2);
	}
	//get a and b for rgb
	float a_red = (pix1.r * (1 - du)) + (pix2.r * (du));
	float a_green = (pix1.g * (1 - du)) + (pix2.g * (du));
	float a_blue = (pix1.b * (1 - du)) + (pix2.b * (du));
	float b_red = (pix3.r * (1 - du)) + (pix4.r * (du));
	float b_green = (pix3.g * (1 - du)) + (pix4.g * (du));
	float b_blue = (pix3.b * (1 - du)) + (pix4.b * (du));
	Pixel32 output = Pixel32();
	output.r = (unsigned char)std::max(0, std::min(255, (int)((a_red * (1 - dv) + (b_red * dv)))));
	output.b = (unsigned char)std::max(0, std::min(255, (int)((a_blue * (1 - dv) + (b_blue * dv)))));
	output.g = (unsigned char)std::max(0, std::min(255, (int)((a_green * (1 - dv) + (b_green * dv)))));
	return output;
}
Pixel32 Image32::gaussianSample( float x , float y , float variance , float radius ) const
{
	Util::Throw( "Image32::gaussianSample undefined" );
	return Pixel32();
}
