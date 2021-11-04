/*=============================================================================
 * Author: Fernando Prokopiuk <fernandoprokopiuk@gmail.com>
 * Date: 2021/09/02
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __KEYS_H__
#define __KEYS_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include "sapi.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/
#define KEY_INVALID_TIME   -1
#define DEBOUNCE_TIME       40
#define MAX_KEYS 4

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/
typedef enum
{
    STATE_KEY_UP,
    STATE_KEY_DOWN,
    STATE_KEY_FALLING,
    STATE_KEY_RISING
} key_State_t;

typedef enum
{
    EVENT_KEY_NONE,
    EVENT_KEY_DOWN,
    EVENT_KEY_UP
} key_Event_t;

typedef enum
{
    LED_OFF,
    LED_ON
} led_state_t;

typedef struct
{
    gpioMap_t key_name;
    key_State_t key_state;
    key_Event_t key_event;
    tick_t key_time_down; //timestamp of the last High to Low transition of the key
    tick_t key_time_up;   //timestamp of the last Low to High transition of the key
    tick_t key_time_diff; //las time difference
    gpioMap_t led_name;
    led_state_t led_state;

} dbn_t;

typedef struct
{
    uartMap_t uart;                        ///< Nombre de la UART del LPC4337 a utilizar.
    uint32_t baudRate;                     ///< BaudRate seleccionado para la comunicacion serie.
    uint8_t *buffer;                       ///< Buffer para la recepción de bytes.
    uint32_t cantidad;                     ///< Cantidad de bytes recibidos.
    bool SOM;                              ///< Flag para indicar si llego el SOM.
    bool EOM;                              ///< Flag para indicar si llego el EOM.
    SemaphoreHandle_t sem_ISR;             ///< Semaforo que la ISR usa para indicarle a la tarea que tiene un paquete listo.
    SemaphoreHandle_t sem_bloque_liberado; ///< Semaforo que le indica al driver que se libero un bloque de memoria.
    tObjeto *ptr_objeto1;                  ///< Puntero al objeto usado para enviar el mensaje del driver a la aplicacion.
    tObjeto *ptr_objeto2;                  ///< Puntero al objeto usado para enviar el mensaje de la aplicacion al driver.
    tMensaje *ptr_mensaje;                 ///< Puntero al mensaje a enviarse a través del objeto.
} sf_t;

/*=====[Prototypes (declarations) of public functions]=======================*/
void keys_init(void);
void keys_update(dbn_t *pkey);
/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __KEYS_H__ */
