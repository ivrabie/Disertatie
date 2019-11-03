/*
 * etl_profile.h
 *
 *  Created on: Aug 14, 2019
 *      Author: ion
 */

#ifndef MAIN_CUSTOM_PROFILE_H_
#define MAIN_CUSTOM_PROFILE_H_

#define ETL_TARGET_DEVICE_GENERIC
#define ETL_TARGET_OS_NONE
#define ETL_COMPILER_GENERIC
#define ETL_NO_CHECKS
#define ETL_CHECK_PUSH_POP

#define ETL_CPP17_SUPPORTED 1
#define ETL_LOG_ERRORS

#define ETL_TARGET_DEVICE_GENERIC
#define ETL_TARGET_OS_NONE
#define ETL_COMPILER_GENERIC
#define ETL_CPP11_SUPPORTED 1
#define ETL_CPP14_SUPPORTED 1
#define ETL_CPP17_SUPPORTED 1
#define ETL_NO_NULLPTR_SUPPORT 0
#define ETL_NO_LARGE_CHAR_SUPPORT 0
#define ETL_CPP11_TYPE_TRAITS_IS_TRIVIAL_SUPPORTED 0
#define ETL_NO_STL #error Must provide a profile header file when buiding ETL.
#endif /* MAIN_CUSTOM_PROFILE_H_ */
