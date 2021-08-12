#include "mbed.h"
#include "SPIFBlockDevice.h"
#include "LittleFileSystem.h"

#include "web_page_variables.h"
#include "my_http_server.h"

#include <inttypes.h>

#define USE_FIXED_IP 1      // 1 for fixed IP, 0 for DHCP

DigitalOut led(LED1);

#if defined(TARGET_NUMAKER_PFM_M487) || defined(TARGET_NUMAKER_IOT_M487)
/* We needn't write-protect and hold functions. Configure /WP and /HOLD pins to high. */
DigitalOut onboard_spi_wp(PC_5, 1);
DigitalOut onboard_spi_hold(PC_4, 1);
#endif

const char *device_para_file = "/dev_fs/device_para.xml";
BlockDevice *bd = new SPIFBlockDevice(MBED_CONF_SPIF_DRIVER_SPI_MOSI,
                                      MBED_CONF_SPIF_DRIVER_SPI_MISO,
                                      MBED_CONF_SPIF_DRIVER_SPI_CLK,
                                      MBED_CONF_SPIF_DRIVER_SPI_CS);

LittleFileSystem fs("dev_fs");

#ifdef MBED_ALL_STATS_ENABLED
    #include "mbed_mem_trace.h"
    Thread thread_mem_info(osPriorityNormal,OS_STACK_SIZE,nullptr,"thread_mem_info");
    void SYS_status();
    void GET_THREAD_info();
    void THREAD_heap_stack_info(); 
#endif 

#define MY_HTTPRESPONSEBUILDER   1

#ifdef MY_HTTPRESPONSEBUILDER
    #include "my_http_response_builder.h"
#else
    //#include "http_response_builder.h"
#endif    

// Requests come in here
void request_handler(ParsedHttpRequest* request, TCPSocket* socket) {
    long size;
    printf("Request came in: %s %s\n", http_method_str(request->get_method()), request->get_url().c_str());

    if (request->get_method() == HTTP_GET && request->get_url() == "/login") 
    {
        #ifdef MY_HTTPRESPONSEBUILDER
            my_HttpResponseBuilder builder;
            builder.send_header_with_200(socket, "Content-Type", "text/html; charset=utf-8" ,sizeof(Kc_LOGIN)); 
            builder.send(socket, Kc_LOGIN, sizeof(Kc_LOGIN));         
        #else
            HttpResponseBuilder builder(200);
            builder.set_header("Content-Type", "text/html; charset=utf-8"); 
            builder.set_header("Connection", "close"); 
            builder.send(socket, Kc_LOGIN, sizeof(Kc_LOGIN));  
        #endif     
    }
    else if (request->get_method() == HTTP_GET && request->get_url() == "/") 
    {
        #ifdef MY_HTTPRESPONSEBUILDER        
            my_HttpResponseBuilder builder;
            builder.send_header_with_200(socket, "Content-Type", "text/html; charset=utf-8" ,sizeof(Kc_INDEX)); 
            builder.send(socket, Kc_INDEX, sizeof(Kc_INDEX));
        #else        
            HttpResponseBuilder builder(200);
            builder.set_header("Content-Type", "text/html; charset=utf-8"); 
            builder.set_header("Connection", "close"); 
            builder.send(socket, Kc_INDEX, sizeof(Kc_INDEX));     
        #endif   
    } 
    else if (request->get_method() == HTTP_GET && request->get_url() == "/general.html") 
    {
        #ifdef MY_HTTPRESPONSEBUILDER        
            my_HttpResponseBuilder builder;
            builder.send_header_with_200(socket, "Content-Type", "text/html; charset=utf-8" ,sizeof(Kc_GENERAL)); 
            builder.send(socket, Kc_GENERAL, sizeof(Kc_GENERAL));
        #else        
            HttpResponseBuilder builder(200);
            builder.set_header("Content-Type", "text/html; charset=utf-8");
            builder.set_header("Connection", "close");  
            builder.send(socket, Kc_GENERAL, sizeof(Kc_GENERAL));     
        #endif         
        
    }
    else if (request->get_method() == HTTP_GET && request->get_url() == "/communication.html") 
    {
        #ifdef MY_HTTPRESPONSEBUILDER        
            my_HttpResponseBuilder builder;
            builder.send_header_with_200(socket, "Content-Type", "text/html; charset=utf-8" ,sizeof(Kc_COMMUNICATION)); 
            builder.send(socket, Kc_COMMUNICATION, sizeof(Kc_COMMUNICATION));
        #else        
            HttpResponseBuilder builder(200);
            builder.set_header("Content-Type", "text/html; charset=utf-8");
            builder.set_header("Connection", "close");  
            builder.send(socket, Kc_COMMUNICATION, sizeof(Kc_COMMUNICATION));     
        #endif  
    }
    else if (request->get_method() == HTTP_GET && request->get_url() == "/mqtt.html") 
    {
        #ifdef MY_HTTPRESPONSEBUILDER        
            my_HttpResponseBuilder builder;
            builder.send_header_with_200(socket, "Content-Type", "text/html; charset=utf-8" ,sizeof(Kc_MQTT)); 
            builder.send(socket, Kc_MQTT, sizeof(Kc_MQTT));
        #else        
            HttpResponseBuilder builder(200);
            builder.set_header("Content-Type", "text/html; charset=utf-8");
            builder.set_header("Connection", "close");  
            builder.send(socket, Kc_MQTT, sizeof(Kc_MQTT));     
        #endif  
    }
    else if (request->get_method() == HTTP_GET && request->get_url() == "/in_out.html") 
    {
        #ifdef MY_HTTPRESPONSEBUILDER        
            my_HttpResponseBuilder builder;
            builder.send_header_with_200(socket, "Content-Type", "text/html; charset=utf-8" ,sizeof(Kc_IN_OUT)); 
            builder.send(socket, Kc_IN_OUT, sizeof(Kc_IN_OUT));
        #else        
            HttpResponseBuilder builder(200);
            builder.set_header("Content-Type", "text/html; charset=utf-8");
            builder.set_header("Connection", "close");  
            builder.send(socket, Kc_IN_OUT, sizeof(Kc_IN_OUT));     
        #endif  
    }    
    else if (request->get_method() == HTTP_GET && request->get_url() == "/hart.html") 
    {
        #ifdef MY_HTTPRESPONSEBUILDER        
            my_HttpResponseBuilder builder;
            builder.send_header_with_200(socket, "Content-Type", "text/html; charset=utf-8" ,sizeof(Kc_HART)); 
            builder.send(socket, Kc_HART, sizeof(Kc_HART));
        #else        
            HttpResponseBuilder builder(200);
            builder.set_header("Content-Type", "text/html; charset=utf-8");
            builder.set_header("Connection", "close");  
            builder.send(socket, Kc_HART, sizeof(Kc_HART));     
        #endif  
    }     
    else if (request->get_method() == HTTP_GET && request->get_url() == "/modbus.html") 
    {
        #ifdef MY_HTTPRESPONSEBUILDER        
            my_HttpResponseBuilder builder;
            builder.send_header_with_200(socket, "Content-Type", "text/html; charset=utf-8" ,sizeof(Kc_MODBUS)); 
            builder.send(socket, Kc_MODBUS, sizeof(Kc_MODBUS));
        #else        
            HttpResponseBuilder builder(200);
            builder.set_header("Content-Type", "text/html; charset=utf-8");
            builder.set_header("Connection", "close");  
            builder.send(socket, Kc_MODBUS, sizeof(Kc_MODBUS));     
        #endif  
    }
    else if (request->get_method() == HTTP_GET && request->get_url() == "/sms.html") 
    {
        #ifdef MY_HTTPRESPONSEBUILDER        
            my_HttpResponseBuilder builder;
            builder.send_header_with_200(socket, "Content-Type", "text/html; charset=utf-8" ,sizeof(Kc_SMS)); 
            builder.send(socket, Kc_SMS, sizeof(Kc_SMS));
        #else        
            HttpResponseBuilder builder(200);
            builder.set_header("Content-Type", "text/html; charset=utf-8");
            builder.set_header("Connection", "close");  
            builder.send(socket, Kc_SMS, sizeof(Kc_SMS));     
        #endif  
    }    
    else if (request->get_method() == HTTP_GET && request->get_url() == "/diag.html") 
    {
        #ifdef MY_HTTPRESPONSEBUILDER        
            my_HttpResponseBuilder builder;
            builder.send_header_with_200(socket, "Content-Type", "text/html; charset=utf-8" ,sizeof(Kc_DIAG)); 
            builder.send(socket, Kc_DIAG, sizeof(Kc_DIAG));
        #else        
            HttpResponseBuilder builder(200);
            builder.set_header("Content-Type", "text/html; charset=utf-8");
            builder.set_header("Connection", "close");  
            builder.send(socket, Kc_DIAG, sizeof(Kc_DIAG));     
        #endif  
    }                                              
    else if (request->get_method() == HTTP_GET && request->get_url() == "/css/style.css") 
    {
        #ifdef MY_HTTPRESPONSEBUILDER        
            my_HttpResponseBuilder builder;
            builder.send_header_with_200(socket, "Content-Type", "text/css; charset=utf-8" ,sizeof(Kc_STYLE)); 
            builder.send(socket, Kc_STYLE, sizeof(Kc_STYLE));
        #else        
            HttpResponseBuilder builder(200);
            builder.set_header("Content-Type", "text/css; charset=utf-8");
            builder.set_header("Connection", "close"); 
            builder.send(socket, Kc_STYLE, sizeof(Kc_STYLE));  
        #endif          
    } 
    else if (request->get_method() == HTTP_GET && request->get_url() == "/images/logo-default.png") 
    {
        #ifdef MY_HTTPRESPONSEBUILDER        
            my_HttpResponseBuilder builder;
            builder.send_header_with_200(socket, "Content-Type", "image/png; charset=utf-8" ,sizeof(Kc_LOGO_DEFAULT)); 
            builder.send(socket, Kc_LOGO_DEFAULT, sizeof(Kc_LOGO_DEFAULT));
        #else        
            HttpResponseBuilder builder(200);
            builder.set_header("Content-Type", "image/png; charset=utf-8");
            builder.set_header("Connection", "close"); 
            builder.send(socket, Kc_LOGO_DEFAULT, sizeof(Kc_LOGO_DEFAULT));   
        #endif          
    }                  
    else if (request->get_method() == HTTP_GET && request->get_url() == "/script/script.js") 
    {
        #ifdef MY_HTTPRESPONSEBUILDER        
            my_HttpResponseBuilder builder;
            builder.send_header_with_200(socket, "Content-Type", "text/javascript; charset=utf-8" ,sizeof(Kc_SCRIPT)); 
            builder.send(socket, Kc_SCRIPT, sizeof(Kc_SCRIPT));
        #else        
            HttpResponseBuilder builder(200);
            builder.set_header("Content-Type", "text/javascript; charset=utf-8");
            builder.set_header("Connection", "close"); 
            builder.send(socket, Kc_SCRIPT, sizeof(Kc_SCRIPT));    
        #endif  
    }  
    else if (request->get_method() == HTTP_GET && request->get_url() == "/device_para.xml") 
    {      
        my_HttpResponseBuilder builder;
        FILE *infile = fopen("/dev_fs/device_para.xml", "r");

        if(infile)
        {
            // uint16_t rdCnt=0;
            // char *buffer = new char[1050];

            // builder.send_header_with_200_without_body_size(socket, "Content-Type", "text/xml; charset=utf-8");
            
            // while ((rdCnt = fread(buffer, sizeof( char ), 1024, infile)) > 0) 
            // {
            //     builder.send(socket, buffer, rdCnt);

            //     debug("size %d\n",rdCnt);
            // }

            // debug("size %d\n",rdCnt);

            // fclose(infile);

            // delete [] buffer;

            char    *buffer;
            uint32_t    numbytes;
            
            FILE *infile = fopen("/dev_fs/device_para.xml", "r");
            
            /* quit if the file does not exist */
            if(infile == NULL)
                printf("no file \n"); 
            
            /* Get the number of bytes */
            fseek(infile, 0L, SEEK_END);
            numbytes = ftell(infile);
            
            /* reset the file position indicator to 
            the beginning of the file */
            fseek(infile, 0L, SEEK_SET);    
            
            /* grab sufficient memory for the 
            buffer to hold the text */
            buffer = new char[numbytes];
            
            /* memory error */
            if(buffer == NULL)
                printf("error - 1 %d\n",numbytes);
            else
                printf("size is %u\n",numbytes); 
                        
            /* copy all the text into the buffer */
            fread(buffer, sizeof(char), numbytes, infile);
            fclose(infile);
            
            /* confirm we have read the file by
            outputing it to the console */
            //printf("The file called test.dat contains this text\n\n%s\n%u\n", buffer, strlen(buffer));
            
            #ifdef MY_HTTPRESPONSEBUILDER        
                my_HttpResponseBuilder builder;
                builder.send_header_with_200(socket, "Content-Type", "text/xml; charset=utf-8" , numbytes); 
                builder.send(socket, buffer, numbytes);
            #else        
                HttpResponseBuilder builder(200);
                builder.set_header("Content-Type", "text/xml; charset=utf-8");
                builder.set_header("Connection", "close");          
                builder.send(socket, buffer, numbytes);   
            #endif     
            
            /* free the memory we used for the buffer */
            free(buffer);
        }
        else
        {
            builder.send_404(socket);
            builder.send(socket, NULL, 0);             
        }    
    }
    else if (request->get_method() == HTTP_POST && request->get_url() == "/device_para.xml") 
    {
        int i=0;
        
        const char *str = (char *)request->get_body();
               
        // printf("\r\n ... post body start ..... \r\n");
        // while(i < request->get_body_length())
        // {
        //     printf("%c", *str++);
        //     i++;
        // } 
        // printf("\r\n ... post body ends ..... \r\n");
        
        FILE *f = fopen("/dev_fs/device_para.xml","w");

        if(f && request->get_body_length()<= HTTP_RECEIVE_BUFFER_SIZE)
        {
            while(i<request->get_body_length())
            {
                fputc(*str++, f);
                i++;
            }            

            printf("\n\n\n WRITING : %d \n\n", i);
            
            fclose(f);
        }
        else
        {
            
        }
        
        #ifdef MY_HTTPRESPONSEBUILDER        
            my_HttpResponseBuilder builder;
            builder.send_200_only(socket);
            builder.send(socket, NULL, 0); 
            
            printf("\r\n ... MY_HTTPRESPONSEBUILDER ..... \r\n");
        #else        
            HttpResponseBuilder builder(200);        
            builder.set_header("Connection", "close"); 
            builder.send(socket, NULL, 0);   
            
             printf("\r\n ... HTTPRESPONSEBUILDER ..... \r\n");
        #endif         
    }                         
    else 
    {
        #ifdef MY_HTTPRESPONSEBUILDER        
            my_HttpResponseBuilder builder;
            builder.send_404(socket);
            builder.send(socket, NULL, 0); 
        #else        
            HttpResponseBuilder builder(404);
            builder.set_header("Connection", "close"); 
            builder.send(socket, NULL, 0); 
        #endif         
    }
}

int8_t mount_file_system()
{
    int8_t err = fs.mount(bd);

    if (err!=0) {
        printf("FS : reformating \n");
        err = fs.reformat(bd);
        if(err!=0) {
            printf("FS : mounting failed \n");
            return -1;
        }
    }
    printf("FS : mounting done \n");

    return 0;
}

int main() {

    printf(
        "Mbed OS version %d.%d.%d\n",
        MBED_MAJOR_VERSION,
        MBED_MINOR_VERSION,
        MBED_PATCH_VERSION
    );
    
    mount_file_system();

    // Connect to the network (see mbed_app.json for the connectivity method used)
    NetworkInterface *network = NetworkInterface::get_default_instance();
    if (!network) {
        printf("Cannot connect to the network, see serial output\n");
        return 1;
    }

#if USE_FIXED_IP
    // This fixed IP address is convenient to test with Windows PC
    SocketAddress ip_addr("192.168.100.110");
    SocketAddress ip_mask("255.255.255.0");
    SocketAddress ip_gwaddr("169.168.100.254");
    network->set_network(ip_addr, ip_mask, ip_gwaddr);
#endif

    network->connect();

    my_HttpServer server(network);
    nsapi_error_t res = server.start(80, &request_handler);

    if (res == NSAPI_ERROR_OK) {
        SocketAddress ip_addr;
        network->get_ip_address(&ip_addr);
        printf("Server is listening at http://%s\n", ip_addr.get_ip_address());
    }
    else {
        printf("Server could not be started... %d\n", res);
    }

    #ifdef  MBED_ALL_STATS_ENABLED
        // GET_THREAD_info();
        // thread_mem_info.start(THREAD_heap_stack_info);
    #endif 

    while(1) 
    {
        ThisThread::sleep_for(50000);   
    }
}

#ifdef MBED_ALL_STATS_ENABLED
void SYS_status()
{
    mbed_stats_sys_t stats;
    mbed_stats_sys_get(&stats);

    printf("Mbed OS Version: %" PRId32 "\n", stats.os_version); 
    printf("Compiler ID ARM:1,GCC_ARM:2,IAR:3 : %d \n", stats.compiler_id);
    
   /* RAM / ROM memory start and size information */
    for (int i = 0; i < MBED_MAX_MEM_REGIONS; i++) {
        if (stats.ram_size[i] != 0) {
            printf("RAM%d: Start 0x%" PRIx32 " Size: 0x%" PRIx32 "\n", i, stats.ram_start[i], stats.ram_size[i]);
        }
    }
    for (int i = 0; i < MBED_MAX_MEM_REGIONS; i++) {
        if (stats.rom_size[i] != 0) {
            printf("ROM%d: Start 0x%" PRIx32 " Size: 0x%" PRIx32 "\n", i, stats.rom_start[i], stats.rom_size[i]);
        }
    }    
}

void GET_THREAD_info()
{
    uint8_t MAX_THREAD_STATS=20;
    
    mbed_stats_thread_t *stats = new mbed_stats_thread_t[MAX_THREAD_STATS];
    int count = mbed_stats_thread_get_each(stats, MAX_THREAD_STATS);

    for (int i = 0; i < count; i++) 
    {
        printf("ID: 0x%" PRIx32 "\n", stats[i].id);
        printf("Name: %s \n", stats[i].name);
        printf("State: %" PRId32 "\n", stats[i].state);
        printf("Priority: %" PRId32 "\n", stats[i].priority);
        printf("Stack Size: %" PRId32 "\n", stats[i].stack_size);
        printf("Stack Space: %" PRId32 "\n", stats[i].stack_space);
        printf("\n");
    }    
}

void THREAD_heap_stack_info() 
{
    uint8_t MAX_THREAD_INFO=20;

    mbed_stats_heap_t heap_info;
    mbed_stats_stack_t stack_info[ MAX_THREAD_INFO ];    
    
    while(1)
    {   
        printf("\nMemoryStats:");
        mbed_stats_heap_get(&heap_info);
        printf("\n\tBytes allocated currently: %u", heap_info.current_size);
        printf("\n\tMax bytes allocated at a given time: %u", heap_info.max_size);
        printf("\n\tCumulative sum of bytes ever allocated: %u", heap_info.total_size);
        printf("\n\tCurrent number of bytes allocated for the heap: %u", heap_info.reserved_size);
        printf("\n\tCurrent number of allocations: %u", heap_info.alloc_cnt);
        printf("\n\tNumber of failed allocations: %u", heap_info.alloc_fail_cnt);
    
        mbed_stats_stack_get(&stack_info[0]);
        printf("\nCumulative Stack Info:");
        printf("\n\tMaximum number of bytes used on the stack: %u", stack_info[0].max_size);
        printf("\n\tCurrent number of bytes allocated for the stack: %u", stack_info[0].reserved_size);
        printf("\n\tNumber of stacks stats accumulated in the structure: %u", stack_info[0].stack_cnt);
    
        mbed_stats_stack_get_each(stack_info, MAX_THREAD_INFO);
        printf("\nThread Stack Info:");
        for (int i = 0; i < MAX_THREAD_INFO; i++) {
            if (stack_info[i].thread_id != 0) {
                printf("\n\tThread: %d", i);
                printf("\n\t\tThread Id: 0x%08lX", stack_info[i].thread_id);
                printf("\n\t\tMaximum number of bytes used on the stack: %u", stack_info[i].max_size);
                printf("\n\t\tCurrent number of bytes allocated for the stack: %u", stack_info[i].reserved_size);
                printf("\n\t\tNumber of stacks stats accumulated in the structure: %u", stack_info[i].stack_cnt);
            }
        }
    
        printf("\nDone...\n\n");   
        
        ThisThread::sleep_for(5000);      
    }
}

#endif


