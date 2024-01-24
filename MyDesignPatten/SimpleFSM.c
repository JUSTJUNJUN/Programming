

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>


#define STATE_OFF           0
#define STATE_LIGHT_ON      1
#define STATE_MAX           2
#define EVETN_BTN_OFF       0
#define EVETN_BTN_ON        1
#define EVETN_MAX           2

int light_fsm_simple_table[STATE_MAX][EVETN_MAX] =
{
    [STATE_OFF][EVETN_BTN_OFF]      = STATE_OFF,
    [STATE_OFF][EVETN_BTN_ON]       = STATE_LIGHT_ON,
    [STATE_LIGHT_ON][EVETN_BTN_OFF] = STATE_OFF,
    [STATE_LIGHT_ON][EVETN_BTN_ON]  = STATE_LIGHT_ON,
};

int light_fsm_event(int cur_stat, int event)
{
    int next_state;

    next_state = light_fsm_simple_table[cur_stat][event];
}

int main(void)
{
    int light_state = STATE_OFF;  
    int eve1 = EVETN_BTN_OFF; 
    int eve2 = EVETN_BTN_ON;

    light_state = light_fsm_event(light_state, eve1);
    printf("now light state is %d\n", light_state);

    light_state = light_fsm_event(light_state, eve2);
    printf("now light state is %d\n", light_state);
}