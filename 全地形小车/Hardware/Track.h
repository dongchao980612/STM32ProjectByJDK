#ifndef __TRACK_H
#define __TRACK_H

void Tracking_Init(void);
uint8_t Read_Sensors(void);
void TrackData_Processing(void);
void TrackData_Processing2(void);
extern int Track;
#endif
