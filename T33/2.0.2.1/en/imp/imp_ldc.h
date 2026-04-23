/*
 * IMP ISP header file.
 *
 * Copyright (C) 2022 Ingenic Semiconductor Co.,Ltd
 */

#ifndef __IMP_LDC_H__
#define __IMP_LDC_H__

#include <stdbool.h>
#include "imp_common.h"
#include "imp_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

typedef enum {
	IMPISP_Offline_DISABLE,		/**<Disable the offline LDC module and close the offline LDC device. */
	IMPISP_Offline_ENABLE,		/**<Enable the offline LDC module and open the offline LDC device */
	IMPISP_Offline_BUTT,        /**<effect paramater, parameters have to be less than this value */
} IMPISPLDCOfflineOpsMode;

/**
 * @fn int32_t IMP_ISP_LDC_OfflineInit(IMPISPLDCOfflineOpsMode mode);
 *
 * LDC offline mode initialization interface
 *
 * @param[in] mode      offlineLDC initialization attribute.
 *
 * @retval 0 means success.
 * @retval Other values mean failure, its value is an error code.
 *
 * @attention This function needs to be called after invoking the IMP_ISP_LDC_SetMode interface.。
 */
int32_t IMP_ISP_LDC_OfflineInit(IMPISPLDCOfflineOpsMode mode);

typedef struct {
	int blocksize;
	double strength;
	double d0;
	double d1;
	double d2;
	double d3;
	double fx;
	double fy;
	double cx;
	double cy;
} IMPISPLDCCalibrationParam;  /**<Parameters used for generating the LUT table */

typedef enum {
	IMPISP_LDC_PARAM = 0,  /**<Using the parameter to generate the LUT table mode */
	IMPISP_LDC_LUT,        /**<Direct reading mode of LUT table file */
} IMPISPLDCLutMode;

typedef struct {
	IMPISPLDCFunc                  func;           /**<The offline mode only supports I2D and LDC functions. To create a Job configuration, select all the required functions using | */
	uint16_t                       width;          /**<The current input width */
	uint16_t                       height;         /**<The current input height */
	IMPISPLDCLutBlockSize          size;           /**<The block size in the lut table */
	IMPISPLDCLutMode               ldc_lut_mode;   /**<Choose the mode of directly reading the LUT table file or the mode of generating the LUT table using parameters. */
	union {
		char                       lut_name[128];  /**<The file path of the LUT table, the absolute path, and the mode for directly reading the LUT table file when used */
		IMPISPLDCCalibrationParam  param;          /**<Parameters used for generating the LUT table */
	};
	uint32_t                       job_hander;     /**<Output parameter, Handle of Job */
} IMPISPLDCJobAttr;

/**
 * @fn int32_t IMP_ISP_LDC_OfflineCreateJob(IMPISPLDCJobAttr *attr)
 *
 * Create LDC offline mode Job
 *
 * @param[in] attr  offline LDC attribute.
 *
 * @retval 0 means success.
 * @retval Other values mean failure, its value is an error code.
 *
 * @attention This function needs to be called after invoking the IMP_ISP_LDC_OfflineInit interface.
 */
int32_t IMP_ISP_LDC_OfflineCreateJob(IMPISPLDCJobAttr *attr);

/**
 * @fn int32_t IMP_ISP_LDC_OfflineDestroyJob(uint32_t job_handler)
 *
 * Destroy LDC offline mode Job
 *
 * @param[in] job_handler  The handle output when creating a Job.
 *
 * @retval 0 means success.
 * @retval Other values mean failure, its value is an error code.
 *
 * @attention This function should be called after invoking the IMP_ISP_LDC_OfflineCreateJob interface, ensuring that all tasks have been completed.
 */
int32_t IMP_ISP_LDC_OfflineDestroyJob(uint32_t job_handler);

typedef struct {
	uint32_t job_hander;   /**<Input parameters, the handle of job */
	uint32_t addr_in;      /**<Enter the Y address */
	uint32_t addr_uv_in;   /**<Enter the UV address */
	uint32_t addr_out;     /**<Output Y address */
	uint32_t addr_uv_out;  /**<Output UV address */
	uint16_t stride_in;    /**<Input stride */
	uint16_t stride_out;   /**<Output stride */
	IMPISPLDCFunc      func;   /**<When using I2D, configuration is required and the rotation angle needs to be specified. */
	IMPISPLDCI2dAngle  angle;  /**<Select the functions required for the current task */
} IMPISPLDCTaskAttr;

/**
 * @fn int32_t IMP_ISP_LDC_OfflineCreateJob(IMPISPLDCJobAttr *attr)
 *
 * Submit the offline mode LDC correction task
 *
 * @param[in] attr  Offline LDC task attribute.
 *
 * @retval 0 means success.
 * @retval Other values mean failure, its value is an error code.
 *
 * @attention This function needs to be called after invoking the IMP_ISP_LDC_OfflineCreateJob interface,And it is necessary to ensure that IMP_ISP_EnableSensor has been called.
 */
int32_t IMP_ISP_LDC_OfflineSubmitTask(IMPISPLDCTaskAttr *attr);

typedef struct {
	uint32_t          job_hander;  /**<Input parameters, the handle of job */
	IMPISPLDCLutMode  lut_mode;    /**<Choose the mode of directly reading the LUT table file or the mode of generating the LUT table using parameters. */
	union {
		char                       lut_name[128];  /**<The file path of the LUT table, the absolute path, and the mode for directly reading the LUT table file when used */
		IMPISPLDCCalibrationParam  param;          /**<Parameters used for generating the LUT table */
	};
} IMPISPLDCUpdateLutAttr;

/**
 * @fn int32_t IMP_ISP_LDC_OfflineSetLutToJob(IMPISPLDCUpdateLutAttr *attr)
 *
 * Offline LDC is based on Job replacement of lut table
 *
 * @param[in] attr  Offline LDC replaces the attributes of the lut table.
 *
 * @retval 0 means success.
 * @retval Other values mean failure, its value is an error code.
 *
 * @attention This function needs to be called after invoking the IMP_ISP_LDC_OfflineCreateJob interface.
 */
int32_t IMP_ISP_LDC_OfflineUpdateLutToJob(IMPISPLDCUpdateLutAttr *attr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

/**
 * @}
 */

#endif /* __IMP_ISP_H__ */
