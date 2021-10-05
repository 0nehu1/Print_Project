
#include "IppFourier.h"
#include <math.h>
#include <vcruntime_string.h>
#include <vcruntime_string.h>
#include <algorithm>

const double PI = 3.14159265358979323846;

void DFT1d(double* re, double* im, int N, int dir) // re와 im 배열에 저장된 값을 이용하여 이산푸리에 변환 또는 역변환을 수행한다.
												// N은 배열의 크기를 나타낸다. dir 값이 1이면 순방향 변환, -1이면 역변환 변환을 수행한다.
												// 푸리에 변환 결과는 다시 re와 im 배열에 저장된다.
{
	double* tr = new double[N];
	double* ti = new double[N];
	memcpy(tr, re, sizeof(double) * N);
	memcpy(ti, im, sizeof(double) * N);

	register int i, x;
	double sum_re, sum_im, temp;

	for (i = 0; i < N; i++)
	{
		sum_re = sum_im = 0;

		for (x = 0; x < N; x++)
		{
			temp = 2 * dir * PI * ((double)i * x / N);

			sum_re += (tr[x] * cos(temp) + ti[x] * sin(temp));
			sum_im += (ti[x] * cos(temp) + tr[x] * sin(temp));
		}

		re[i] = sum_re;
		im[i] = sum_im;
	}
	if (dir == -1) //IDFT
	{
		for (i = 0; i < N; i++)
		{
			re[i] /= (double)N;
			im[i] /= (double)N;
		}
	}

	delete[] tr;
	delete[] ti;
}

void FFT1d(double* re, double* im, int N, int dir)	// 고속 이산 푸리에 변환 또는 역변환을 수행한다. 인자에 대한 설명은 DFT1d와 동일하다. 
{
	register int i, j, k;

	//----------------------------------------------------------------------
	// 입력 데이터 순서 바꾸기
	//----------------------------------------------------------------------

	int n2 = N >> 1;
	int nb = 0;

	while (N != (1 << nb))
		nb++;

	for (i = 0, j=0;i < N - 1; i++)
	{
		if (i < j)
		{
			std::swap(re[i], re[j]);
			std::swap(im[i], im[j]);
		}

		k = n2;

		while (k <= j)
		{
			j -= k;
			k >>= 1;
		}

		j += k;
	}


	//----------------------------------------------------------------------
	// 버터플라이(Butterfly) 알고리즘
	//----------------------------------------------------------------------

	int i1, l, l1, l2;
	double c1, c2, t1, t2, u1, u2, z;

	c1 = -1.0;
	c2 = 0.0;
	l2 = 1;

	for (l = 0; l < nb; l++)
	{
		l1 = l2;
		l2 <<= 1;
		u1 = 1.0;
		u2 = 0.0;

		for (j = 0; j < l1; j++)
		{
			for (i = j; i < N; i += l2)
			{
				i1 = i + l1;
				t1 = u1 * re[i1] - u2 * im[i1];
				t2 = u1 * im[i1] + u2 * re[i1];
				re[i1] = re[i] - t1;
				im[i1] = im[i] - t2;
				re[i] += t1;
				im[i] += t2;
			}
		

		z = u1 * c1 - u2 * c2;
		u2 = u1 * c2 + u2 * c1;
		u1 = z;
	
		}

	c2 = sqrt((1.0 - c1) / 2.0);

	if (dir == 1) // forward
		c2 = -c2;

	c1 = sqrt((1.0 + c1) / 2.0);

	}

	if (dir == -1) // IDFT
	{
		for (i = 0; i < N; i++)
		{
			re[i] /= static_cast<double>(N);
			im[i] /= static_cast<double>(N);
		}
	}

}

bool IsPowerOf2(int n)	// n이 2의 승수로 표현되는 수이면 true를 반환하고, 아니면 false를 반환한다. FFT1d 함수에서 사용된다.
{
	int ref = 1;

	while (ref < n)
		ref <<= 1;

	if (ref == n)
		return true;
	else
		return false;

}

IppFourier::IppFourier(void)	// 생성자 함수, 멤버 변수의 값들을 초기화한다.
	:width(0),height(0)
{
}

void IppFourier::SetImage(IppByteImage& img)	// width, height 멤버 변숫값을 설정하고 입력영상을
{												// real 영상에 복사한다. imag 영상은 항상 0으로 초기화한다.
	width = img.GetWidth();
	height = img.GetHeight();

	real.Convert(img);					// 실수부에 영상의 그레이 스케일 값 복사
	imag.CreateImage(width, height);	// 허수부는 모두 0으로 초기화
}

void IppFourier::GetImage(IppByteImage& img) // real 영상을 IppByteImage타입의 영상을 img로 복사한다.
{
	if (!real.IsValid())
		return;

	// 실수부의 값을 이용하여 출력 영상을 생성
	img.CreateImage(width, height);

	double* pRe = real.GetPixels();
	BYTE* ptr = img.GetPixels();

	int size = real.GetSize();
	for (int i = 0; i < size; i++)
		ptr[i] = static_cast<BYTE>(limit(pRe[i] + 0.5));
}

void IppFourier::GetSpectrumImage(IppByteImage& img)	// 푸리에 변환 후 호출되어, 푸리에 스펙트럼 영상을 생성한다.
{
	register int i, j;

	img.CreateImage(width, height);
	BYTE** ptr = img.GetPixels2D();

	double** pRe = real.GetPixels2D();
	double** pIm = imag.GetPixels2D();

	// 스펙트럼 값을 저장할 임시 버퍼
	IppDoubleImage spectrum(width, height);
	double** pSpec = spectrum.GetPixels2D();

	//-----------------------------------------------------------------------------------------
	// 복소수 값의 절댓값을 로그 변환하여 저장.
	// 정규화된 결과 이미지 생성을 위하여 스펙트럼의 최댓값 계산.
	//-----------------------------------------------------------------------------------------

	double mag, max_value = 0;
	for(j=0; j <height;j++)
		for (i = 0; i < width; i++)
		{
			mag = sqrt(pRe[j][i] * pRe[j][i] + pIm[j][i] * pIm[j][i]);
			pSpec[j][i] = log(mag + 1);

			if (pSpec[j][i] > max_value)
				max_value = pSpec[j][i];
	}

	//-----------------------------------------------------------------------------------------
	// 출력 영상 생성. 푸리에 변환 결과를 SHIFT하고,
	// 출력 영상의 최댓값이 255가 되도록 그레이스케일 값을 정규화.
	//-----------------------------------------------------------------------------------------

	int x, y;
	for(j = 0; j < height;j++)
		for (i = 0; i < width; i++)
		{
			x = i + width / 2;
			y = j + height / 2;

			if (x >= width) x -= width;
			if (y >= height) y -= height;

			ptr[j][i] = static_cast<BYTE>(limit(pSpec[y][x] * 255 / max_value));
		}

}

void IppFourier::GetPhaseImage(IppByteImage& img)
{
	register int i, j;

	img.CreateImage(width, height);
	BYTE** ptr = img.GetPixels2D();

	double** pRe = real.GetPixels2D();
	double** pIm = imag.GetPixels2D();

	// 위상각을 저장할 임시 버퍼
	IppDoubleImage phase(width, height);
	double** pPhase = phase.GetPixels2D();

	//-----------------------------------------------------------------------------------------
	// 복소수 값의 위상각을 저장. atan2 함수의 반환 값은 -PI ~ PI이다.
	//-----------------------------------------------------------------------------------------

	
	for(j=0;j<height;j++)
		for (i = 0; i < width; i++)
		{
			pPhase[j][i] = atan2(pIm[j][i], pRe[j][i]); // 래디언 단위
		}


	//-----------------------------------------------------------------------------------------
	// 출력 영상 생성, 푸리에 변환 결과를 SHIFT하고
	// 출력 영상의 최댓값이 255가 되도록 그레이스케일 값을 정규화.
	//-----------------------------------------------------------------------------------------
	int x, y;
	for (j = 0; j < height; j++)
		for (i = 0; i < width; i++)
		{
			x = i + width / 2;
			y = j + height / 2;

			if (x >= width) x -= width;
			if (y >= height) y -= height;

			ptr[j][i] = static_cast<BYTE>(limit(pPhase[y][x] * 128 / PI + 128));
		}
	

}

void IppFourier::DFT(int dir)	// 영상의 이산 푸리에 변환 또는 역변환을 수행한다. dir값이 1이면 순방향 변환, -1이면 역방향 변환을 수행한다. 
{
	if (!real.IsValid())
		return;

	//-----------------------------------------------------------------------------------------
	// real, imag 복사본 생성
	//-----------------------------------------------------------------------------------------

	IppDoubleImage real_cpy = real;
	IppDoubleImage imag_cpy = imag;

	double** pRe = real.GetPixels2D();
	double** pIm = imag.GetPixels2D();

	double** pReCpy = real_cpy.GetPixels2D();
	double** pImCpy = imag_cpy.GetPixels2D();

	//-----------------------------------------------------------------------------------------
	// 이산 푸리에 변환 (또는 역변환)
	//-----------------------------------------------------------------------------------------

	register int i, j, x, y;
	double sum_re, sum_im, temp;

	for(j=0;j<height;j++)
		for (i = 0; i < width; i++)
		{
			sum_re = sum_im = 0;

			for(y=0;y<height;y++)
				for (x = 0; x < width; x++)
				{
					temp = 2 * dir * PI * (static_cast<double>(i) * x / width + static_cast<double>(j) * y / height);
					sum_re += (pReCpy[y][x] * cos(temp) - pImCpy[y][x] * sin(temp));
					sum_im += (pReCpy[y][x] * sin(temp) + pImCpy[y][x] * sin(temp));
				}

			pRe[j][i] = sum_re;
			pIm[j][i] = sum_im;

			if (dir == -1) // 역변환인 경우
			{
				pRe[j][i] /= (width * height);
				pIm[j][i] /= (width * height);
			}
		}

}



void IppFourier::DFTRC(int dir) // 영상의 행과 열을 분리하여 영상의이산 푸리에 변환 또는 역변환을 수행한다.
{
	if (!real.IsValid())
		return;

	//-------------------------------------------------------------------------
	// 행(Row) 단위 이산 푸리에 변환 (또는 역변환)
	//-------------------------------------------------------------------------

	IppDoubleImage real_tmp, imag_tmp;

	real_tmp.CreateImage(width, 1);
	imag_tmp.CreateImage(width, 1);

	double* re = real_tmp.GetPixels();
	double* im = imag_tmp.GetPixels();

	double** pRe = real.GetPixels2D();
	double** pIm = imag.GetPixels2D();

	register int i, j;

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			re[i] = pRe[j][i];
			im[i] = pIm[j][i];
		}

		DFT1d(re, im, width, dir);

		for (i = 0; i < width; i++)
		{
			pRe[j][i] = re[i];
			pIm[j][i] = im[i];
		}
	}

	//-------------------------------------------------------------------------
	// 열(Column) 단위 이산 푸리에 변환 (또는 역변환)
	//-------------------------------------------------------------------------

	real_tmp.CreateImage(height, 1);
	imag_tmp.CreateImage(height, 1);

	re = real_tmp.GetPixels();
	im = imag_tmp.GetPixels();

	memset(re, 0, sizeof(double) * height);
	memset(im, 0, sizeof(double) * height);

	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			re[j] = pRe[j][i];
			im[j] = pIm[j][i];
		}

		DFT1d(re, im, height, dir);

		for (j = 0; j < height; j++)
		{
			pRe[j][i] = re[j];
			pIm[j][i] = im[j];
		}
	}
}


void IppFourier::FFT(int dir)	// 영상의 고속 이산 푸리에 변환 또는 역변환을 수행한다.
{
	if(!real.IsValid())
		return;

	if (!IsPowerOf2(width) || !IsPowerOf2(height))
		return;

	//-------------------------------------------------------------------------
	// 행(Row) 단위 이산 푸리에 변환 (또는 역변환)
	//-------------------------------------------------------------------------

	IppDoubleImage real_tmp, imag_tmp;

	real_tmp.CreateImage(width, 1);
	imag_tmp.CreateImage(width, 1);

	double* re = real_tmp.GetPixels();
	double* im = imag_tmp.GetPixels();

	double** pRe = real.GetPixels2D();
	double** pIm = imag.GetPixels2D();

	register int i, j;

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			re[i] = pRe[j][i];
			im[i] = pIm[j][i];
		}
		FFT1d(re, im, width, dir);

		for (i = 0; i < width; i++)
		{
			pRe[j][i] = re[i];
			pIm[j][i] = im[i];
		}
	}

	//-------------------------------------------------------------------------
	// 열(Column) 단위 이산 푸리에 변환 (또는 역변환)
	//-------------------------------------------------------------------------

	real_tmp.CreateImage(height, 1);
	imag_tmp.CreateImage(height, 1);

	re = real_tmp.GetPixels();
	im = imag_tmp.GetPixels();

	memset(re, 0, sizeof(double) * height);
	memset(im, 0, sizeof(double) * height);

	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			re[j] = pRe[j][i];
			im[j] = pIm[j][i];
		}

		FFT1d(re, im, height, dir);

		for (j = 0; j < height; j++)
		{
			pRe[j][i] = re[j];
			pIm[j][i] = im[j];
		}
	}

}

void IppFourier::LowPassIdeal(int cutoff)	// 주파수 공간에서 이상적 저역 통과필터를 수행한다.
{
	register int i, j, x, y;

	int cx = width / 2;
	int cy = height / 2;

	double** pRe = real.GetPixels2D();
	double** pIm = real.GetPixels2D();

	for(j=0;j<height;j++)
		for (i = 0; i < width; i++)
		{
			x = i + cx;
			y = j + cy;

			if (x >= width) x -= width;
			if (y >= height) y -= height;

			if ((x - cx) * (x - cx) + (y - cy) * (y - cy) > (cutoff * cutoff))
				pRe[j][i] = pIm[j][i] = 0;
		}
}

void IppFourier::HighPassIdeal(int cutoff)	// 주파수 공간에서 이상적 고역 통과 필터를 수행한다.
{
	register int i, j, x, y;

	int cx = width / 2;
	int cy = height / 2;

	double** pRe = real.GetPixels2D();
	double** pIm = real.GetPixels2D();

	for (j = 0; j < height; j++)
		for (i = 0; i < width; i++)
		{
			x = i + cx;
			y = j + cy;

			if (x >= width) x -= width;
			if (y >= height) y -= height;

			if ((x - cx) * (x - cx) + (y - cy) * (y - cy) < (cutoff * cutoff))
				pRe[j][i] = pIm[j][i] = 0;
		}
}

void IppFourier::LowPassGaussian(int cutoff) // 주파수 공간에서 가우시안 저역 통과 필터를 수행한다.
{
	register int i, j, x, y;
	double dist2, hval;

	int cx = width / 2;
	int cy = height / 2;

	double** pRe = real.GetPixels2D();
	double** pIm = imag.GetPixels2D();

	for(j=0;j<height;j++)
		for (i = 0; i < width; i++)
		{
			x = i + cx;
			y = j + cy;

			if (x >= width) x -= width;
			if (y >= height) y -= height;

			dist2 = static_cast<double>((x - cx) * (x - cx) + (y - cy) * (y - cy));

			hval = exp(-dist2 / (2 * cutoff * cutoff));

			pRe[j][i] *= hval;
			pIm[j][i] *= hval;
		}
}

void IppFourier::HighPassGaussian(int cutoff) // 주파수 공간에서 가우시간 고역 통과 필터를 수행한다.
{
	register int i, j, x, y;
	double dist2, hval;

	int cx = width / 2;
	int cy = height / 2;

	double** pRe = real.GetPixels2D();
	double** pIm = imag.GetPixels2D();

	for (j = 0; j < height; j++)
		for (i = 0; i < width; i++)
		{
			x = i + cx;
			y = j + cy;

			if (x >= width) x -= width;
			if (y >= height) y -= height;

			dist2 = static_cast<double>((x - cx) * (x - cx) + (y - cy) * (y - cy));

			hval = 1.0 - exp(-dist2 / (2 * cutoff * cutoff));

			pRe[j][i] *= hval;
			pIm[j][i] *= hval;
		}
}
