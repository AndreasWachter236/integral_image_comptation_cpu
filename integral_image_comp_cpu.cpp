// integral_image_comp.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.

#include <iostream>
#include <chrono>
#include <omp.h>
#include <ppl.h>
#include "image.h"

int const image_width = 10000;
int const image_height = 10000;

using namespace std;

template <typename T>
void createRandomImage(Image<T>& rndImage)
{
	for (int j = 0; j < rndImage.pixels; j++)
	{
		rndImage.data[j] = 1;// (T) rand() % 256);
	}
}

// single core //
template <typename T>
void calculateSingleThread(Image<T>& iImage)
{
	for (size_t j = 0; j < iImage.height; j++)
	{
		for (size_t i = 0; i < iImage.width; i++)
		{
			T value = iImage.get(i, j) + iImage.get(i - 1, j) - iImage.get(i - 1, j - 1) + iImage.get(i, j - 1);
			iImage.set(i, j, value);
		}
	}
}

// multi core//
template <typename T>
void calculateMultiThread(Image<T>& iImage)
{
	# ifdef _OPENMP
	{
		// OpenMP //
		cout << "Running with OpenMP" << endl;
		#pragma omp parallel for shared(iImage)
		for (int j = 0; j < iImage.height; j++)
		{
			T value = iImage.get(0, j);
			for (size_t i = 1; i < iImage.width; i++)
			{
				value += iImage.get(i, j);
				iImage.set(i, j, value);
			}
		}
		#pragma omp parallel for shared(iImage)
		for (int i = 0; i < iImage.width; i++)
		{
			T value = iImage.get(i, 0);
			for (size_t j = 1; j < iImage.height; j++)
			{
				value += iImage.get(i, j);
				iImage.set(i, j, value);
			}
		}
	}

	#else
	{
		// parallel_for concurrency runtime C++11 //
		concurrency::parallel_for(size_t(0), iImage.height, [&](size_t j)
		{
			T value = iImage.get(0, j);
			for (size_t i = 1; i < iImage.width; i++)
			{
				value += iImage.get(i, j);
				iImage.set(i, j, value);
			}
		});

		concurrency::parallel_for(size_t(0), iImage.width, [&](size_t i)
		{
			T value = iImage.get(i, 0);
			for (size_t j = 1; j < iImage.height; j++)
			{
				value += iImage.get(i, j);
				iImage.set(i, j, value);
			}
		});
	}
	#endif
}

template <typename T>
T sum(Image<T>& iImage, const size_t x, const size_t y, const size_t w, const size_t h)
{
	T a = iImage.get(x, y);
	T b = iImage.get(x + w, y);
	T c = iImage.get(x, y + h);
	T d = iImage.get(x + w, y + h);

	return d - b - c + a;
}

int main(int argc, char* argv[])
{
	Image<int> image;
	image.create(image_width, image_height);
	createRandomImage(image);

	Image<int> iImageSingle;
	iImageSingle.create(image_width, image_height);
	std::copy(&image.data[0], &image.data[0] + image.pixels, &iImageSingle.data[0]);

	auto tstart = chrono::high_resolution_clock::now();

	calculateSingleThread(iImageSingle);

	cout << iImageSingle.get(image_width - 1, image_height - 1) << endl;

	auto tend = chrono::high_resolution_clock::now();
	auto runtime = chrono::duration_cast<chrono::nanoseconds>(tend - tstart);

	printf("Time measured: %.3f seconds.\n", runtime.count() * 1e-9);

	Image<int> iImageMulti;
	iImageMulti.create(image_width, image_height);
	std::copy(&image.data[0], &image.data[0] + image.pixels, &iImageMulti.data[0]);

	tstart = chrono::high_resolution_clock::now();

	calculateMultiThread(iImageMulti);

	cout << iImageMulti.get(image_width - 1, image_height - 1) << endl;

	tend = chrono::high_resolution_clock::now();
	runtime = chrono::duration_cast<chrono::nanoseconds>(tend - tstart);

	printf("Time measured: %.3f seconds.\n", runtime.count() * 1e-9);
}


