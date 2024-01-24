

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>


#define STATE_DEPEND        4
#define STATE_OFF           0
#define STATE_HIGH_LIGHT    1
#define STATE_LOW_LIGHT     2
#define STATE_MAX           3

#define EVETN_BTN_OFF       0
#define EVETN_BTN_ON        1
#define EVETN_MAX           2


int last_state = STATE_LOW_LIGHT;
int last_light_state = STATE_LOW_LIGHT;


struct {
  int (*func) ();
  int next_state;
} light_fsm [STATE_MAX][EVETN_MAX] =
{
    //STATE_OFF
    {
        { lfsm_ignore, STATE_MAX }, /*EVETN_BTN_OFF*/
        { lfsm_btn_on, STATE_DEPEND }, /*EVETN_BTN_ON*/
    },
    //STATE_HIGH_LIGHT
    {
        { lfsm_btn_off, STATE_OFF }, /*EVETN_BTN_OFF*/
        { lfsm_ignore, STATE_MAX }, /*EVETN_BTN_ON*/
    },
    //STATE_LOW_LIGHT
    {
        { lfsm_btn_off, STATE_OFF }, /*EVETN_BTN_OFF*/
        { lfsm_ignore, STATE_MAX }, /*EVETN_BTN_ON*/
    },
};


int lfsm_ignore(int cur_stat, int event)
{
    printf("invalid state or event\n");
    return STATE_MAX;

}


int lfsm_btn_on(int cur_stat, int event)
{
    if (last_light_state == STATE_HIGH_LIGHT) {
        return STATE_LOW_LIGHT;
    } else if(last_light_state == STATE_LOW_LIGHT) {
        return STATE_HIGH_LIGHT;
    } else {
        printf("invalid state\n");
        return STATE_MAX;
    }
}


int lfsm_btn_off(int cur_stat, int event)
{
    last_light_state = cur_stat;

    return 0;
}


int light_change_state(int cur_stat, int next_state,int event)
{
    //if light on has special handling
    if (next_state = STATE_HIGH_LIGHT) {
        printf("rejoice, now bright light\n");
    }
    //other state change related handlings, maybe use current state and next state, or event type  
    last_state = cur_stat;
    cur_stat = next_state;

    return 0;
}


int light_event_happen(int event)
{
    //if light on has special handling
    if (event = EVETN_BTN_OFF) {
        printf("someone turn off light\n");
    }
    //other event type related handlings
    return 0;
}


int light_fsm_event(int cur_stat, int event)
{
    int next_state, next_state_tmp;

    next_state_tmp = *(light_fsm[cur_stat][event].func);
    if (next_state_tmp == STATE_MAX) {
        printf("fsm error\n");
        return -1;
    }

    if (light_fsm[cur_stat][event].next_state == STATE_DEPEND) {
        next_state = next_state_tmp;
    } else {
        next_state = light_fsm[cur_stat][event].next_state;       
    }

    light_change_state(next_state, cur_stat, event);
    light_event_happen(event);
}


int main(void)
{
    int light_state = STATE_OFF;  

    light_fsm_event(light_state, EVETN_BTN_OFF);
    light_fsm_event(light_state, EVETN_BTN_ON);
    light_fsm_event(light_state, EVETN_BTN_OFF);
    light_fsm_event(light_state, EVETN_BTN_ON);   
}