#ifndef WATIMER_H
#define WATIMER_H

#include <stdint.h>
#ifdef USE_RTC_WATIMER
#include <time.h>
#endif //USE_RTC_WATIMER

extern volatile time_t watimer_time;


typedef enum{RUN_SINGLE_ABSOLUTE, RUN_SINGLE_RELATIVE, RUN_CONTINUOSLY_RELATIVE} watimer_run_mode_en;


struct watimer_callback_st;


typedef void(*watimer_callback_func)(struct watimer_callback_st *desc);


struct watimer_callback_st
{
	watimer_run_mode_en	level;
	time_t 		period;
	time_t		timer;
	watimer_callback_func func;
	uint8_t 	timeout;
};


#define MAXCALLBACKS    20


#define SECONDS(x) ((time_t)(x) * 1000)



typedef struct
{
  void (* __global_irq_enable)(void);
  void (* __global_irq_disable)(void);
  void (* __cc_irq_enable)(uint8_t chan);
  void (* __cc_irq_disable)(uint8_t chan);
  void (* __loop_irq_enable)(void);
  void (* __loop_irq_disable)(void);
#ifdef USE_RTC_WATIMER
  void (* __cc_set)(uint8_t chan, time_t data);
  time_t (* __cc_get)(uint8_t chan);
  time_t (* __cnt_get)(uint8_t chan);
#else
  void (* __cc_set)(uint8_t chan, uint16_t data);
  uint16_t (* __cc_get)(uint8_t chan);
  uint16_t (* __cnt_get)(uint8_t chan);
#endif //USE_RTC_WATIMER
  uint8_t (* __check_cc_irq)(uint8_t chan);
}watimer_HAL_st;



extern watimer_HAL_st *watimer_hal;

void watimer_set_HAL(watimer_HAL_st *);

void watimer_init(void);
//void watimer_reg_func(uint8_t name, void *fn);
void watimer_irq(void);
void watimer_run_callbacks();
void watimer_add_callback(struct watimer_callback_st *desc, watimer_callback_func cb, watimer_run_mode_en run_level, time_t period);
void watimer_remove_callback(struct watimer_callback_st *desc);
_Bool watimer_check_callback(struct watimer_callback_st *desc);
_Bool watimer_can_sleep();
time_t watimer_update_time();


#endif //WATIMER_H