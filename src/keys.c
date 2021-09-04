/*=============================================================================
 * Author: Fernando Prokopiuk <fernandoprokopiuk@gmail.com>
 * Date: 2021/09/02
 *===========================================================================*/

/*=====[Inclusion of own header]=============================================*/

#include "../../../RTOS1/A3/inc/keys.h"

/*=====[Inclusions of private function dependencies]=========================*/

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

dbn_t keys_struct[MAX_KEYS]; //Array de estructuras de teclas
dbn_t *pkey;                 //Puntero a estructuras de teclas

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementations of public functions]=================================*/

/**
 * @brief Inicializa el driver
 * 
 */
void keys_init(void)
{
   gpioMap_t keys_names[]={TEC1,TEC2,TEC3,TEC4};
   gpioMap_t leds_names[]={LEDB,LED1,LED2,LED3};
   int32_t i = 0;

   for(i=0;i<MAX_KEYS;i++)
   {
      keys_struct[i].key_name = keys_names[i];
      keys_struct[i].key_state = STATE_KEY_UP;
      keys_struct[i].key_event = EVENT_KEY_NONE;
      keys_struct[i].key_time_down = KEY_INVALID_TIME;
      keys_struct[i].key_time_up = KEY_INVALID_TIME;
      keys_struct[i].key_time_diff = KEY_INVALID_TIME;
      keys_struct[i].led_name = leds_names[i];
      keys_struct[i].led_state = LED_OFF;
   }
}

// keys_update State Function
/**
 * @brief Keys SM for polled operation
 * 
 * @param pkey 
 */
void keys_update(dbn_t *pkey)
{
   switch (pkey->key_state)
   {
   case STATE_KEY_UP:
      /* CHECK TRANSITION CONDITIONS */
      if (!gpioRead(pkey->key_name))
      {
         pkey->key_state = STATE_KEY_FALLING;
      }
      break;

   case STATE_KEY_FALLING:
      /* ENTRY */

      /* CHECK TRANSITION CONDITIONS */
      if (!gpioRead(pkey->key_name))
      {
         pkey->key_state = STATE_KEY_DOWN;

         /* ACCION DEL EVENTO !*/
         pkey->key_event = EVENT_KEY_DOWN;

         pkey->key_time_down = tickRead();
      }
      else
      {
         pkey->key_state = STATE_KEY_UP;
      }

      /* LEAVE */
      break;

   case STATE_KEY_DOWN:
      /* CHECK TRANSITION CONDITIONS */
      if (gpioRead(pkey->key_name))
      {
         pkey->key_state = STATE_KEY_RISING;
      }
      break;

   case STATE_KEY_RISING:
      /* ENTRY */

      /* CHECK TRANSITION CONDITIONS */

      if (gpioRead(pkey->key_name))
      {
         pkey->key_state = STATE_KEY_UP;

         /* ACCION DEL EVENTO ! */
         pkey->key_event = EVENT_KEY_UP;

         tick_t current_tick_count = tickRead();

         /* calculo el tiempo de pulsacion */
         if (pkey->key_time_down < current_tick_count)
         {
            pkey->key_time_diff = current_tick_count - pkey->key_time_down;
         }
         else
         {
            pkey->key_time_diff = current_tick_count - pkey->key_time_down + 0xFFFFFFFF;
         }
      }
      else
      {
         pkey->key_state = STATE_KEY_DOWN;
      }

      /* LEAVE */
      break;
   }
}
/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/

