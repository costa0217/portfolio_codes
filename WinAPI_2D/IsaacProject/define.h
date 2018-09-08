#pragma once

#define PURE = 0


#define SAME_SIGN(x, y) ( (((x) < 0 ) && ((y) < 0))||(((x) > 0 ) && ((y) > 0)) )

#define OUTAGE_X	if(m_fSpeedX > 0 ){--m_fSpeedX;}else if(m_fSpeedX < 0){++m_fSpeedX;}
#define OUTAGE_Y	if(m_fSpeedY > 0 ){--m_fSpeedY;}else if(m_fSpeedY < 0){++m_fSpeedY;}

#define BOMB_SPEED 10.f