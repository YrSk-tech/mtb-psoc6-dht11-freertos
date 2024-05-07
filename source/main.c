#include "main.h"
#include <dht_task.h>
#include <print_task.h>

/******************************************************************************
* Global Variables
******************************************************************************/
/* This enables RTOS aware debugging. */
volatile int uxTopUsedPriority;


int main(void)
{
    cy_rslt_t result;

    /* This enables RTOS aware debugging in OpenOCD. */
    uxTopUsedPriority = configMAX_PRIORITIES - 1;

    /********* Variable to store the queue handle */
    QueueHandle_t print_queue;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    CY_ASSERT(result == CY_RSLT_SUCCESS);


    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);
    /* If retarget-io init failed stop program execution */
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    /* Initialize the DATA pin. The pin used can be changed in main.h file */
    result = cyhal_gpio_init(DATA_PIN, CYHAL_GPIO_DIR_BIDIRECTIONAL, CYHAL_GPIO_DRIVE_PULLUP, 1);
    if (result != CY_RSLT_SUCCESS)
	{
		CY_ASSERT(0);
	}

    /* Enable global interrupts */
    __enable_irq();

    /* Create a queue to store the sensor readings */
    print_queue = xQueueCreate(5, sizeof(struct readings));

    /* Create tasks */
    xTaskCreate(DHT_Task, "DHT Task", configMINIMAL_STACK_SIZE, (void*) print_queue, 2, NULL);
    xTaskCreate(Print_Task, "Print Task", 3*configMINIMAL_STACK_SIZE, (void*) print_queue, 1, NULL);

    /* If the task creation failed stop the program execution */
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    /* To avoid compiler warning */
    (void) result;

    /* Start the scheduler */
    vTaskStartScheduler();

    for (;;)
    {

    }
}

/* [] END OF FILE */
