#include <CL/cl.h>
#include <math.h>
#include <assert.h>
#include "clguetzli_test.h"
#include "clguetzli.h"
#include "ocl.h"

bool floatCompare(const float* a, const float* b, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		if (fabs(a[i] - b[i]) > 0.001)
		{
			return false;
		}
	}
	return true;
}

void clMaskHighIntensityChange(const float* r, const float* g, const float* b,
	const float* r2, const float* g2, const float* b2,
	size_t xsize, size_t ysize,
	const float* result_r, const float* result_g, const float* result_b,
	const float* result_r2, const float* result_g2, const float* result_b2)
{
	size_t channel_size = xsize * ysize * sizeof(float);
	cl_int err = 0;
	ocl_args_d_t &ocl = getOcl();
	ocl_channels xyb0 = ocl.allocMemChannels(channel_size);
	ocl_channels xyb1 = ocl.allocMemChannels(channel_size);

	clEnqueueWriteBuffer(ocl.commandQueue, xyb0.r, CL_FALSE, 0, channel_size, r, 0, NULL, NULL);
	clEnqueueWriteBuffer(ocl.commandQueue, xyb0.g, CL_FALSE, 0, channel_size, g, 0, NULL, NULL);
	clEnqueueWriteBuffer(ocl.commandQueue, xyb0.b, CL_FALSE, 0, channel_size, b, 0, NULL, NULL);
	clEnqueueWriteBuffer(ocl.commandQueue, xyb1.r, CL_FALSE, 0, channel_size, r2, 0, NULL, NULL);
	clEnqueueWriteBuffer(ocl.commandQueue, xyb1.g, CL_FALSE, 0, channel_size, g2, 0, NULL, NULL);
	clEnqueueWriteBuffer(ocl.commandQueue, xyb1.b, CL_FALSE, 0, channel_size, b2, 0, NULL, NULL);

	err = clFinish(ocl.commandQueue);

	clMaskHighIntensityChangeEx(xyb0, xyb1, xsize, ysize);

	cl_float *r0_r = (cl_float *)clEnqueueMapBuffer(ocl.commandQueue, xyb0.r, true, CL_MAP_READ, 0, channel_size, 0, NULL, NULL, &err);
	cl_float *r0_g = (cl_float *)clEnqueueMapBuffer(ocl.commandQueue, xyb0.g, true, CL_MAP_READ, 0, channel_size, 0, NULL, NULL, &err);
	cl_float *r0_b = (cl_float *)clEnqueueMapBuffer(ocl.commandQueue, xyb0.b, true, CL_MAP_READ, 0, channel_size, 0, NULL, NULL, &err);
	cl_float *r1_r = (cl_float *)clEnqueueMapBuffer(ocl.commandQueue, xyb1.r, true, CL_MAP_READ, 0, channel_size, 0, NULL, NULL, &err);
	cl_float *r1_g = (cl_float *)clEnqueueMapBuffer(ocl.commandQueue, xyb1.g, true, CL_MAP_READ, 0, channel_size, 0, NULL, NULL, &err);
	cl_float *r1_b = (cl_float *)clEnqueueMapBuffer(ocl.commandQueue, xyb1.b, true, CL_MAP_READ, 0, channel_size, 0, NULL, NULL, &err);

	floatCompare(result_r, r0_r, xsize * ysize);
	floatCompare(result_g, r0_g, xsize * ysize);
	floatCompare(result_b, r0_b, xsize * ysize);
	floatCompare(result_r2, r1_r, xsize * ysize);
	floatCompare(result_g2, r1_g, xsize * ysize);
	floatCompare(result_b2, r1_b, xsize * ysize);

	ocl.releaseMemChannels(xyb0);
	ocl.releaseMemChannels(xyb1);
}

// strong to
void clEdgeDetectorMap(void)
{

}

// strong todo
void clBlockDiffMap(void)
{

}

// strong to
void clEdgeDetectorLowFreq(void)
{

}

void clMask(const float* r, const float* g, const float* b,
	const float* r2, const float* g2, const float* b2,
	size_t xsize, size_t ysize,
	const float* mask_r, const float* mask_g, const float* mask_b,
	const float* maskdc_r, const float* maskdc_g, const float* maskdc_b)
{
	size_t channel_size = xsize * ysize * sizeof(float);
	cl_int err = 0;
	ocl_args_d_t &ocl = getOcl();
	ocl_channels rgb = ocl.allocMemChannels(channel_size);
	ocl_channels rgb2 = ocl.allocMemChannels(channel_size);

	ocl_channels mask = ocl.allocMemChannels(channel_size);
	ocl_channels mask_dc = ocl.allocMemChannels(channel_size);

	clEnqueueWriteBuffer(ocl.commandQueue, rgb.r, CL_FALSE, 0, channel_size, r, 0, NULL, NULL);
	clEnqueueWriteBuffer(ocl.commandQueue, rgb.g, CL_FALSE, 0, channel_size, g, 0, NULL, NULL);
	clEnqueueWriteBuffer(ocl.commandQueue, rgb.b, CL_FALSE, 0, channel_size, b, 0, NULL, NULL);
	clEnqueueWriteBuffer(ocl.commandQueue, rgb2.r, CL_FALSE, 0, channel_size, r2, 0, NULL, NULL);
	clEnqueueWriteBuffer(ocl.commandQueue, rgb2.g, CL_FALSE, 0, channel_size, g2, 0, NULL, NULL);
	clEnqueueWriteBuffer(ocl.commandQueue, rgb2.b, CL_FALSE, 0, channel_size, b2, 0, NULL, NULL);

	err = clFinish(ocl.commandQueue);

	clMaskEx(rgb, rgb2, xsize, ysize, mask/*out*/, mask_dc/*out*/);

	cl_float *r0_r = (cl_float *)clEnqueueMapBuffer(ocl.commandQueue, mask.r, true, CL_MAP_READ, 0, channel_size, 0, NULL, NULL, &err);
	cl_float *r0_g = (cl_float *)clEnqueueMapBuffer(ocl.commandQueue, mask.g, true, CL_MAP_READ, 0, channel_size, 0, NULL, NULL, &err);
	cl_float *r0_b = (cl_float *)clEnqueueMapBuffer(ocl.commandQueue, mask.b, true, CL_MAP_READ, 0, channel_size, 0, NULL, NULL, &err);
	cl_float *r1_r = (cl_float *)clEnqueueMapBuffer(ocl.commandQueue, mask_dc.r, true, CL_MAP_READ, 0, channel_size, 0, NULL, NULL, &err);
	cl_float *r1_g = (cl_float *)clEnqueueMapBuffer(ocl.commandQueue, mask_dc.g, true, CL_MAP_READ, 0, channel_size, 0, NULL, NULL, &err);
	cl_float *r1_b = (cl_float *)clEnqueueMapBuffer(ocl.commandQueue, mask_dc.b, true, CL_MAP_READ, 0, channel_size, 0, NULL, NULL, &err);

	floatCompare(mask_r, r0_r, xsize * ysize);
	floatCompare(mask_g, r0_g, xsize * ysize);
	floatCompare(mask_b, r0_b, xsize * ysize);
	floatCompare(maskdc_r, r1_r, xsize * ysize);
	floatCompare(maskdc_g, r1_g, xsize * ysize);
	floatCompare(maskdc_b, r1_b, xsize * ysize);

	ocl.releaseMemChannels(rgb);
	ocl.releaseMemChannels(rgb2);
	ocl.releaseMemChannels(mask);
	ocl.releaseMemChannels(mask_dc);
}

// ian todo
void clCombineChannels(void)
{

}

// ian todo
void clCalculateDiffmapEx(void)
{

}
