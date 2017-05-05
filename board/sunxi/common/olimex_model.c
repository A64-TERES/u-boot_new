
#include <common.h>
#include <fdt_support.h>
#include <sys_config.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_OLIMEX_MODEL
int has_anx9807_chip(void);
#endif

int set_misc(void)
{
//FDT_PATH_HDEBUG
	
         user_gpio_set_t gpio_init;
        int     ret = 0;
	int 	ret1 = 0;
        int     enabled = 0;
        int nodeoffset;
	__u32  gpio_hd = 0;

        nodeoffset =  fdt_path_offset(working_fdt,FDT_PATH_HDEBUG);
        if(nodeoffset >0)
        {
                fdt_getprop_u32(working_fdt, nodeoffset,"enabled", (uint32_t*)&enabled);
		printf("Headphone DEBUG State :%d\n",enabled);
        }

        memset(&gpio_init, 0, sizeof(user_gpio_set_t));

        ret = fdt_get_one_gpio(FDT_PATH_HDEBUG, "debug_en_gpio",&gpio_init);
        if(!ret)
        {
                if(gpio_init.port)
                {
                        gpio_hd = gpio_request(&gpio_init, 1);
                        if(!gpio_hd)
                        {
                                printf("reuqest gpio for headphone debug failed\n");
                                return 1;
                        } else {
			
				 if (enabled == 0 ) {
                        		ret1 =  gpio_write_one_pin_value(gpio_hd, 1, "debug_en_gpio");
                			} 
			         if (enabled == 1 ) {
					 ret1 = gpio_write_one_pin_value(gpio_hd, 0, "debug_en_gpio");
                       			}		


				}

                       
                }

		 printf("Headphone DEBUG State GPIO:%d\n",ret1);
        }
        return 0;


}
int get_model(char* model)
{

#ifdef CONFIG_OLIMEX_MODEL
	if (has_anx9807_chip()) {
		puts("Teres: has ANX9807 chip\n");
		sprintf(model, "teres");
	} else {
		sprintf(model, "a64-olinuxino");
	}
#endif
		

	return 0;
}

int olimex_set_model(void)
{
	char model[128] = {0}	;
	get_model(model);
	printf("Model: %s\n", model);
	if(setenv("olimex_model", model))
	{
		printf("error:set variable [olimex_model] fail\n");
	}
	set_misc();
	return 0;
}
