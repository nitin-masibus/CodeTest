#ifndef _MY_HTTP_RESPONSE_BUILDER_
#define _MY_HTTP_RESPONSE_BUILDER_

#include "mbed.h"

const char *str_http_ok = "HTTP/1.1 200 OK\r\n"; 
const char *str_404 = "HTTP/1.1 404 Not Found\r\n"; 
const char *str_cont_len = "Content-Length: ";
const char *str_conn_close = "Connection: close\r\n";

class my_HttpResponseBuilder 
{
public:

    void send_200_only(TCPSocket* socket)
    {
        memset(header ,0, sizeof(header));
        snprintf(header, sizeof(header), "%s%s%s%d\r\n\r\n", str_http_ok, str_conn_close, str_cont_len,0);   
                                                                                
        send(socket, header, strlen(header));                                                                             
    }  
    
    void send_404(TCPSocket* socket)
    {
        memset(header ,0, sizeof(header));
        snprintf(header, sizeof(header), "%s%s%s%d\r\n\r\n", str_404, str_conn_close, str_cont_len,0);   
                                                                                
        send(socket, header, strlen(header));                                                                             
    }          
       
    void send_header_with_200(TCPSocket* socket ,const char *s1, const char *s2, uint32_t body_size)
    {
        memset(header ,0, sizeof(header));

        snprintf(header, sizeof(header), "%s%s%s%d\r\n%s: %s\r\n\r\n", str_http_ok, str_conn_close,
                                                                        str_cont_len,body_size, s1, s2);   
                                                                                
        send(socket, header, strlen(header));                                                                             
    }

    void send_header_with_200_without_body_size(TCPSocket* socket ,const char *s1, const char *s2)
    {
        memset(header ,0, sizeof(header));

        snprintf(header, sizeof(header), "%s%s%s: %s\r\n\r\n", str_http_ok, str_conn_close,s1, s2);  
                                                                                                       
        send(socket, header, strlen(header));                                                                             
    }    
    
    nsapi_error_t send(TCPSocket* socket, const void* html_data, size_t html_data_size) 
    {
        if (!socket) return NSAPI_ERROR_NO_SOCKET;
                
        nsapi_error_t r = socket->send(html_data, html_data_size);
                
        return r;
    }
    
private:
    char tmp_buff[5];
    char header[128];
};

#endif // _MY_HTTP_RESPONSE_BUILDER_
