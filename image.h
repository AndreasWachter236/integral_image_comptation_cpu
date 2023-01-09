#pragma once
#ifndef _IMAGE_H_
#define _IMAGE_H_

template <typename T>

class Image
{
public:
	void create(size_t w, size_t h)
	{
		width = w;
		height = h;
		pixels = w * h;
		data = new T[pixels]{};
	}
/*
	Image& operator = (const Image<T>& other)
	{
		if (&other == this)
			return *this;

		if (this->pixels != other.pixels)
		{
			this->data = new T[other.pixels]{};
			this->width = other.width;
			this->height = other.height;
			this->pixels = other.pixels;
		}

		std::copy(&other.data[0], &other.data[0] + this->pixels, &this->data[0]);
		return *this;
	}
*/
	T get(const size_t x, const size_t y) const
	{
		if (x < width && y < height)
			return data[y * width + x];
		else
			return 0;
	}

	void set(const size_t x, const size_t y, const T value)
	{
		if (x < width && y < height)
			data[y * width + x] = value;
	}

	size_t width = 0;
	size_t height = 0;
	size_t pixels = 0;
	T* data = new T[pixels];
};

#endif // !_IMAGE_H_

