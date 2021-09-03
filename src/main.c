/*=============================================================================
 * Author: Fernando Prokopiuk <fernandoprokopiuk@gmail.com>
 * Date: 2021/09/02
 *===========================================================================*/

/*=====[Definition macros of private constants]==============================*/
// typedef enum
// {
//     LED_OFF,
//     LED_ON
// } led_state_t;


// The maximum number of tasks required at any one time during the execution
// of the program. MUST BE ADJUSTED FOR EACH NEW PROJECT
#define SCHEDULER_MAX_TASKS   (10)

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"
#include "seos_pont.h"
#include "../../../RTOS1/A3/inc/keys.h"

/*==================[declaraciones de funciones internas]====================*/
void task_led(void *pkey);
void keys_service_task( void* pkey );


/*==================[declaraciones de funciones externas]====================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main(void)
{
   boardInit();

   /* inicializo el modulo de tecla */
   keys_init();

   // Initialize scheduler
   schedulerInit();

   // Se agrega la tarea tarea pkey1 al planificador
   schedulerAddTask(keys_service_task, // Function that implements the task update.
                    pkey1,             // Parameter passed into the task update.
                    0,                 // Execution offset in ticks.
                    DEBOUNCE_TIME      // Periodicity of task execution in ticks.
   );
   // Se agrega la tarea tarea pkey2 al planificador
   schedulerAddTask(keys_service_task, // Function that implements the task update.
                    pkey2,             // Parameter passed into the task update.
                    0,                 // Execution offset in ticks.
                    DEBOUNCE_TIME      // Periodicity of task execution in ticks.
   );
   // Se agrega la tarea tarea pkey3 al planificador
   schedulerAddTask(keys_service_task, // Function that implements the task update.
                    pkey3,             // Parameter passed into the task update.
                    0,                 // Execution offset in ticks.
                    DEBOUNCE_TIME      // Periodicity of task execution in ticks.
   );
   // Se agrega la tarea tarea pkey4 al planificador
   schedulerAddTask(keys_service_task, // Function that implements the task update.
                    pkey4,             // Parameter passed into the task update.
                    0,                 // Execution offset in ticks.
                    DEBOUNCE_TIME      // Periodicity of task execution in ticks.
   );

   // Initialize task scheduler each 1ms.
   schedulerStart(1);

   while (true)
   {
      // Dispatch (execute) tasks that are mark to be execute by scheduler.
      schedulerDispatchTasks();
   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the
   // case of a PC program.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/
/**
 * @brief Funcion que según led_state
 * 
 * @param pkey 
 */
void task_led(void *pkey) //Funcion que según el estado del LED hace algo
{
   if (pkey->led_state == LED_OFF)
   {
      /* toggle del led */
      gpioToggle(pkey->led_name);

      /* cambio de estado al led */
      pkey->led_state = LED_ON;

      /* planifico el apagado del led con un offset=tiempo que estuvo pulsado el botón*/
      schedulerAddTask(task_led,            // funcion de tarea a agregar
                       0,                   // parametro de la tarea
                       pkey->key_time_diff, // offset de ejecucion en ticks
                       0                    // periodicidad de ejecucion en ticks
      );
   }
   else if (pkey->led_state == LED_ON)
   {
      /* toggle del led */
      gpioToggle(pkey->led_name);

      /* cambio de estado al led */
      pkey->led_state = LED_OFF;

      pkey->key_time_diff = KEY_INVALID_TIME;
   }
}

/**
   @brief Funcion que se ejecuta cada DEBOUNCE_TIME ticks.

   @param pkey
 */
void keys_service_task(void *pkey)
{
   keys_update(pkey); //Actualiza el estado de la FSM

   if (pkey->key_event == EVENT_KEY_DOWN)
   {
      /* no hago nada */
   }
   else if (pkey->key_event == EVENT_KEY_UP) //Al soltarse el botón se agrega tarea task_led // @suppress("Field cannot be resolved")
   {
      /* planifico que la tarea de LED se ejecute en 0 ticks */
      schedulerAddTask(task_led, // funcion de tarea a agregar
                       pkey,     // parametro de la tarea
                       0,        // offset -> 0 = "ejecutate inmediatamente"
                       0         // periodicidad de ejecucion en ticks
      );
   }
}
/*==================[definiciones de funciones externas]=====================*/

