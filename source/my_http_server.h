/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _MY_HTTP_SERVER_
#define _MY_HTTP_SERVER_

#include "mbed.h"
#include "http_request_parser.h"
#include "http_response.h"
//#include "http_response_builder.h"

#ifndef MY_HTTP_SERVER_MAX_CONCURRENT
#define MY_HTTP_SERVER_MAX_CONCURRENT      1
#endif

#define DBG_HTTP 1

typedef HttpResponse ParsedHttpRequest;

/**
 * \brief HttpServer implements the logic for setting up an HTTP server.
 */
class my_HttpServer {
public:

    my_HttpServer(NetworkInterface* network) 
    {
        _network = network;
    }

    nsapi_error_t start(uint16_t port, Callback<void(ParsedHttpRequest* request, TCPSocket* socket)> a_handler) 
    {
        server = new TCPSocket();

        nsapi_error_t ret;

        ret = server->open(_network);
        if (ret != NSAPI_ERROR_OK) {
            return ret;
        }

        ret = server->bind(port);
        if (ret != NSAPI_ERROR_OK) {
            return ret;
        }

        server->listen(MY_HTTP_SERVER_MAX_CONCURRENT); // max. concurrent connections...

        handler = a_handler;

        main_thread.start(callback(this, &my_HttpServer::http_main));

        return NSAPI_ERROR_OK;
    }

private:

    void http_main() 
    {
        while (1) 
        {
            TCPSocket* socket; // = new TCPSocket(); // Q: when should these be cleared? When not connected anymore?
            nsapi_error_t accept_res;
            
            debug_if(DBG_HTTP, "HTTP : waiting for ACCEPT \n");
            
            socket = server->accept(&accept_res);
            
            if (accept_res == NSAPI_ERROR_OK) 
            {
                SocketAddress a;
                socket->getpeername(&a);
                debug_if(DBG_HTTP,"HTTP ACCEPT IP: %s PORT : %d \n", a.get_ip_address() , a.get_port());  
                while (1) 
                {
                    ParsedHttpRequest* response = new ParsedHttpRequest();
                    HttpParser* parser = new HttpParser(response, HTTP_REQUEST);

                    // Set up a receive buffer (on the heap)
                    uint8_t* recv_buffer = (uint8_t*)malloc(HTTP_RECEIVE_BUFFER_SIZE);

                    // TCPSocket::recv is called until we don't have any data anymore
                    nsapi_size_or_error_t recv_ret;
                    while ((recv_ret = socket->recv(recv_buffer, HTTP_RECEIVE_BUFFER_SIZE)) > 0) 
                    {
                        // Pass the chunk into the http_parser
                        size_t nparsed = parser->execute((const char*)recv_buffer, recv_ret);
                        if (nparsed != recv_ret) {
                            debug_if(DBG_HTTP,"Parsing failed... parsed %d bytes, received %d bytes\n", nparsed, recv_ret);
                            recv_ret = -2101;
                            break;
                        }

                        if (response->is_message_complete()) {
                            break;
                        }
                    }
                    
                    // When done, call parser.finish()
                    parser->finish();

                    // Free the receive buffer
                    free(recv_buffer);

                    // Let user application handle the request, if user needs a handle to response they need to memcpy themselves
                    if (recv_ret > 0) {
                        handler(response, socket);
                    }

                    // Free the response and parser
                    delete response;
                    delete parser;

                    // error?
                    if (recv_ret <= 0) 
                    {
                        debug_if(DBG_HTTP, "HTTP : Closing.. recv_ret ---->> %d\n", recv_ret);
                        break;
                    }
                }

                socket->close();
            }
            else 
            {
                debug_if(DBG_HTTP, "HTTP : accept failed \n");
            }
        }
    }

    TCPSocket* server;
    NetworkInterface* _network;
    Thread main_thread;
    Callback<void(ParsedHttpRequest* request, TCPSocket* socket)> handler;
};

#endif // _MY_HTTP_SERVER
