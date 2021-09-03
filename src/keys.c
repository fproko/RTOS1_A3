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

//dbn_t key_n1,key_n2,key_n3,key_n4;

dbn_t key_n1 = {TEC1, STATE_KEY_UP, EVENT_KEY_NONE, KEY_INVALID_TIME, KEY_INVALID_TIME, KEY_INVALID_TIME, LEDB, LED_OFF};
dbn_t key_n2 = {TEC2, STATE_KEY_UP, EVENT_KEY_NONE, KEY_INVALID_TIME, KEY_INVALID_TIME, KEY_INVALID_TIME, LED1, LED_OFF};
dbn_t key_n3 = {TEC3, STATE_KEY_UP, EVENT_KEY_NONE, KEY_INVALID_TIME, KEY_INVALID_TIME, KEY_INVALID_TIME, LED2, LED_OFF};
dbn_t key_n4 = {TEC4, STATE_KEY_UP, EVENT_KEY_NONE, KEY_INVALID_TIME, KEY_INVALID_TIME, KEY_INVALID_TIME, LED3, LED_OFF};

// Punteros de teclas
dbn_t *pkey_n1 = &key_n1;
dbn_t *pkey_n2 = &key_n2;
dbn_t *pkey_n3 = &key_n3;
dbn_t *pkey_n4 = &key_n4;

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementations of public functions]=================================*/

/**
   @brief Inicializa el driver

 */
void keys_init(void)
{
   //Inicializacion de estructuras de teclas
   // key_n1.key_name = TEC1;
   // key_n1.key_state = STATE_KEY_UP;
   // key_n1.key_event = EVENT_KEY_NONE;
   // key_n1.key_time_down = KEY_INVALID_TIME;
   // key_n1.key_time_up = KEY_INVALID_TIME;
   // key_n1.key_time_diff = KEY_INVALID_TIME;
   // key_n1.led_name = LEDB;
   // key_n1.led_state = LED_OFF;

}

// keys_ Update State Function
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

