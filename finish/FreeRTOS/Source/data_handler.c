#include <cJSON.h>
#include <stdio.h>
#include "data_handler.h"
#include "bsp.h"

char* uart_data_packet(char* payload)
{    
    cJSON* root_json = NULL;
	  cJSON* child_json = NULL;
	  char* format_data = NULL;
	  char buffer[100] = {0};
		sprintf(buffer,"%s", payload);
		
	  root_json = cJSON_CreateObject();
	  child_json = cJSON_CreateObject();
		
		
	  cJSON_AddStringToObject(child_json, "day", buffer);
	  cJSON_AddItemToObject(root_json, "hex", child_json);
	
	  format_data = cJSON_PrintUnformatted(root_json);
	  cJSON_Delete(root_json);
	
	  return format_data;
}





