void Init_RTC(void);
void set_time_data_day(void);
void SetRTCTimeInfo(unsigned int hour, unsigned int minute,unsigned int second);
void SetRTCDateInfo(unsigned int date,unsigned int month,unsigned  int year);
void SetRTCDay(unsigned int day);
void  get_time_data_day_info(void);
void GetRTCTimeInfo(signed int *hour, signed int*minute,signed int *second);
void GetRTCDateInfo(signed int *date,signed int *month, signed int *year);
void GetRTCDay(signed int *day);
void Display_RTC_Time(unsigned int hour,unsigned int minute,unsigned int second);
void DisplayRTCDay(unsigned int dow);
void DisplayRTCDate(unsigned int date,unsigned int month,unsigned int year);
void disp_time_data_day_info(void );

