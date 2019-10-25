/*
 *  Calendar Library
 *
 *  Created on: Oct 25, 2019
 *
 *  Author: Coskun ERGAN
 */

#ifndef LIB_CALENDAR_H_
#define LIB_CALENDAR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "stdint.h"
#include "stdbool.h"

typedef enum
{
    eCALENDAR_DATA_FRAME,
    eCALENDAR_CONFIG_FRAME,
    eCALENDAR_NUMBEROFTYPE
} Calendar_Frame_t;

typedef enum
{
    eCALENDAR_INIT,
    eCALENDAR_DEINIT,
    eCALENDAR_DISABLE,
    eCALENDAR_ENABLE,
	  eCALENDAR_SETTIME,
		eCALENDAR_ADD_CALLBACK,
		eCALENDAR_DEL_CALLBACK,
    eCALENDAR_NUMBEROFTYPES
} Calendar_Config_t;


typedef enum
{
    eSECOND_PASSED,
    eMINUTE_PASSED,
		eHOUR_PASSED,		
		eDAY_PASSED,
		eWEEK_PASSED,				
		eMONTH_PASSED,
		eYEAR_PASSED,				
} Calendar_Passed_t;

typedef struct
{
    Calendar_Frame_t frame_type;
    Calendar_Config_t config;
		struct calendar_date *date;
    void (*vfPtr)(Calendar_Passed_t passed);
} Calendar_Config_Frame_t;

struct calendar_cb_list_t
{
    struct  calendar_cb_list_t *next;
    void	(*vfPtr)(Calendar_Passed_t passed);
};
typedef struct calendar_cb_list_t Calendar_Cb_List_t;

//! Calendar structure to hold a date
struct calendar_date
{
    uint8_t second;        //!< 0-59
    uint8_t minute;        //!< 0-59
    uint8_t hour;          //!< 0-23
    uint8_t date;          //!< 0-30 \note First day of month is 0, not 1.
    uint8_t month;         //!< 0 January - 11 December
    uint16_t year;         //!< 1970-2105
    uint8_t dayofweek;     //!< 0 Sunday  - 6 Saturday
};

#define CALENDAR_CALLBACK_FUNC(_PASSED) 									 					\
		do{																															\
    static struct calendar_cb_list_t *s; 														\
    for(s = SysList_Head((list_t)&Calendar_CbFunc_List);  					\
            s != NULL; 																							\
            s = SysList_Item_Next(s)) 															\
    {																																\
        s->vfPtr(_PASSED); 																					\
    }																																\
	}while(0); 																												\

bool calendar_is_date_valid(struct calendar_date *date);

void calendar_timestamp_to_date(uint32_t timestamp, struct calendar_date
                                *date_out);

void calendar_timestamp_to_date_tz(uint32_t timestamp, int8_t hour,
                                   uint8_t min, struct calendar_date *date_out);

uint32_t calendar_date_to_timestamp(struct calendar_date *date);

uint32_t calendar_date_to_timestamp_tz(struct calendar_date *date, int8_t hour,
                                       uint8_t min);

void calendar_time_between_dates(struct calendar_date *date_end,
                                 struct calendar_date *date_start, struct calendar_date *date_out);

void calendar_add_second_to_date(struct calendar_date *date);

void Calc_WeekDay(struct calendar_date *date);
uint8_t Find_WeekDay(uint16_t year, uint8_t month, uint8_t date);

osStatus_t SendConfigMsg_Calendar(Calendar_Config_t config, struct calendar_date *date, void *cb_func);

#ifdef __cplusplus
}
#endif

/**
 * \page calendar_quickstart Quick start guide for Calendar service
 *
 * This is the quick start guide for the \ref calendar_group, with
 * step-by-step instructions on how to configure and use the driver in a
 * selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g., the main application function.
 *
 * \section calendar_basic_use_case Basic use case
 * \section calendar_use_cases Calendar use cases
 * - \ref calendar_basic_use_case
 * - \subpage calendar_use_case_1
 *
 * \section calendar_basic_use_case Basic use case - Calculate timestamp
 *
 * The use case will let the user calculate the corresponding timestamp to a
 * date
 *
 * \section calendar_basic_use_case_setup Setup steps
 *
 * \subsection calendar_basic_use_case_setup_prereq Prerequisites
 * For the code of this use case to work, the following must
 * be added to the project:
 * -# A date struct with a date:
 * \code
	struct calendar_date date = {
	    .second = 12,
	    .minute = 1,
	    .hour = 22,
	    .date = 8,
	    .month = 2,
	    .year = 1985
	};
\endcode
 *
 * \subsection calendar_basic_use_case_setup_code Example code
 * No setup code is needed, the service is ready for use as-is.
 *
 * \section calendar_basic_use_case_usage Usage steps
 *
 * \subsection calendar_basic_use_case_usage_code Example code
 * Add to, e.g. the main loop in the application C-file:
 * \code uint32_t timestamp = calendar_date_to_timestamp(&date); \endcode
 *
 * \subsection calendar_basic_use_case_usage_flow Workflow
 * -# Convert date to timestamp:
 *   - \code uint32_t timestamp = calendar_date_to_timestamp(&date); \endcode
 */

/**
 * \page calendar_use_case_1 Calculate time between dates
 *
 * The use case will let the user calculate the time between two dates, by
 * first calculating the dates from two timestamps.
 *
 * \section calendar_use_case_1_setup Setup steps
 *
 * \subsection calendar_use_case_1_setup_prereq Prerequisites
 * For the code of this use case to work, the following must
 * be added to the project:
 * -# Three date structs:
 * \code
	struct calendar_date result;
	struct calendar_date end_date;
	struct calendar_date start_date;
\endcode
 * -# Two timestamps:
 * \code
	uint32_t end_timestamp = 1309174659;
	uint32_t start_timestamp = 123456789;
\endcode
 *
 * \subsection calendar_use_case_1_setup_code Example code
 * No setup code is needed, the service is ready for use as-is.
 *
 * \section calendar_use_case_1_usage Usage steps
 *
 * \subsection calendar_use_case_1_usage_code Example code
 * Add to, e.g. the main loop in the application C-file:
 * \code
	calendar_timestamp_to_date(end_timestamp, &end_date);
	calendar_timestamp_to_date(start_timestamp, &start_date);
	calendar_time_between_dates(&end_date, &start_date, &result);
\endcode
 *
 * \subsection calendar_use_case_1_usage_flow Workflow
 * -# Convert the end timestamp to date:
 *   - \code calendar_timestamp_to_date(end_timestamp, &end_date); \endcode
 * -# Convert the start timestamp to date:
 *   - \code calendar_timestamp_to_date(start_timestamp, &start_date); \endcode
 * -# Calculate the time between the two dates:
 *   - \code calendar_time_between_dates(&end_date, &start_date, &result);
\endcode
 */

#endif /* __LIB_CALENDAR_H */
