#include "print_task.h"

/*******************************************************************************
* Function Name: Print_Task
****************************************************************************//**
*
* Task function to print the temperature and humidity values on to a serial
* terminal after reading from the queue.
*
* \param pvParameters
* Void pointer which points to the queue handle
*
*******************************************************************************/

void Print_Task(void* pvParameters)
{
	/* Variable to store the queue handle */
	QueueHandle_t print_queue;
	print_queue = (QueueHandle_t) pvParameters;

	/* Variable to check if connection error message is already displayed */
	bool conn_err_displayed = false;

	/* Variable to store temperature and humidity values */
	struct readings DHT_reading = {0, 0};    /* Variables to store temperature and humidity values */

	for(;;)
	{
		/* *************************************************************
		 * Receive the temperature and humidity values from the queue.
		 * If the queue is empty, enter blocked state and wait for the
		 * DHT_Task to send the value to the queue. As Print_Task is
		 * given the least priority, it prints data only when all the
		 * other tasks are in blocked state.
		 * *************************************************************/
		xQueueReceive(print_queue, &DHT_reading, portMAX_DELAY);

		/* Print the DHT sensor readings if the values are valid */
		if(DHT_reading.result_code == SUCCESS)
		{
			printf("\r\nHumidity  =   %.2f\r\n",DHT_reading.humidity);
			printf("\r\nTemperature  =   %.2f\r\n",DHT_reading.temperature);
			conn_err_displayed = false;
		}

		/* *************************************************************
		 * If there is a connection error notify the user once that the
		 * sensor is not connected. The printing resumes once connection
		 * is established.
		 * *************************************************************/
		else if(DHT_reading.result_code == DHT_CONNECTION_ERROR)
		{
			if(conn_err_displayed == false)
			{
				printf("\r\nDHT Sensor Connection Failed\r\n");
				conn_err_displayed = true;
			}
		}
		else
		{
			/* **************************************************************
			 * This application does not do anything if the value read from
			 * the sensor is incorrect. The printing resumes once valid
			 * values are obtained. But any error handling code can be placed
			 * here based on the use case.
			 * **************************************************************/
		}
	}

}


